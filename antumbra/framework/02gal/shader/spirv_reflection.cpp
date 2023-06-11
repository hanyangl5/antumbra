#include "spirv_reflection.h"

#include <dxcompiler/dxcapi.h>
#include <spirv_cross.hpp>

#include "framework/01core/memory/memory.h"

namespace ant::gal {

ShaderReflection ReflectFromSpirv(CompiledShader *shader_blob) {
    IDxcBlob *blob = reinterpret_cast<IDxcBlob *>(shader_blob->byte_code);
    spirv_cross::Compiler compiler((u32 *)blob->GetBufferPointer(), blob->GetBufferSize() / sizeof(u32));
    ShaderReflection ret;
    auto &entry = compiler.get_entry_point({shader_blob->entry}, compiler.get_execution_model());

    ret.entry = shader_blob->entry;
    if (entry.model == spv::ExecutionModelGLCompute) {
        ret.work_group_size[0] = entry.workgroup_size.x;
        ret.work_group_size[1] = entry.workgroup_size.y;
        ret.work_group_size[2] = entry.workgroup_size.z;
    }

    if (entry.model == spv::ExecutionModelTessellationControl) {
        ret.tesc_control_point = entry.output_vertices;
    }

    auto resources = compiler.get_shader_resources();
    u32 count = 0;

    // stage input/output are used to reflect pipeline vertex attributes
    count += static_cast<u32>(resources.stage_inputs.size());  // inputs
    count += static_cast<u32>(resources.stage_outputs.size()); // outputs

    // shader resources
    count += static_cast<u32>(resources.uniform_buffers.size());   // constant buffers
    count += static_cast<u32>(resources.storage_buffers.size());   // uav buffers
    count += static_cast<u32>(resources.separate_images.size());   // textures
    count += static_cast<u32>(resources.separate_samplers.size()); // samplers
    count += static_cast<u32>(resources.storage_images.size());    // uav textures

    // push constants
    count += static_cast<u32>(resources.push_constant_buffers.size()); // push constants
    // count += (uint32_t)resources.acceleration_structures.size(); // raytracing structures

    ret.resources.resize(count);

    u32 current_resource = 0;

    // stage inputs
    for (auto &input : resources.stage_inputs) {
        auto &resource = ret.resources[current_resource++];
        resource.binding =
            compiler.get_decoration(input.id, spv::DecorationLocation); // location is the binding point for inputs
        resource.resource_type = ShaderResourceType::INPUT;
        spirv_cross::SPIRType type = compiler.get_type(input.type_id);
        // bit width * vecsize = size
        resource.size = (type.width / 8) * type.vecsize;
    }
    // stage output
    for (u32 i = 0; i < static_cast<u32>(resources.stage_outputs.size()); ++i) {
        spirv_cross::Resource const &input = resources.stage_outputs[i];
        auto &resource = ret.resources[current_resource++];

        resource.binding =
            compiler.get_decoration(input.id, spv::DecorationLocation); // location is the binding point for inputs
        resource.resource_type = ShaderResourceType::OUTPUT;
        spirv_cross::SPIRType type = compiler.get_type(input.type_id);
        // bit width * vecsize = size
        resource.size = (type.width / 8) * type.vecsize;
    }

    // push constants
    for (auto &input : resources.push_constant_buffers) {
        auto &resource = ret.resources[current_resource++];

        resource.descriptor_type = gal_descriptor_type::PUSH_CONSTANT;
        resource.resource_type = ShaderResourceType::PUSH_CONSTANT;
        spirv_cross::SPIRType type = compiler.get_type(input.type_id);
        resource.size = static_cast<u32>(compiler.get_declared_struct_size(type));

        resource.name = std::move(input.name);
    }

    // resources
    auto reflect_shader_resources = [&](auto &resources, gal_descriptor_type type, gal_shader_stage stage) {
        for (auto &input : resources) {
            auto &resource = ret.resources[current_resource++];

            resource.descriptor_type = type;
            resource.resource_type = ShaderResourceType::RESOURCE;
            resource.used_stages = stage;

            resource.set = compiler.get_decoration(input.id, spv::DecorationDescriptorSet);
            resource.binding = compiler.get_decoration(input.id, spv::DecorationBinding);

            //resource.dim;

            spirv_cross::SPIRType resource_type = compiler.get_type(input.type_id);
            resource.size = static_cast<u32>(compiler.get_declared_struct_size(resource_type));

            resource.name = std::move(input.name);
        }
    };

    auto utils_to_gal_shader_stage = [](spv::ExecutionModel model) {
        switch (model) {
        case spv::ExecutionModelVertex:
            return gal_shader_stage::VERT;
        case spv::ExecutionModelTessellationControl:
            return gal_shader_stage::TESC;
        case spv::ExecutionModelTessellationEvaluation:
            return gal_shader_stage::TESE;
        case spv::ExecutionModelGeometry:
            return gal_shader_stage::GEOM;
        case spv::ExecutionModelFragment:
            return gal_shader_stage::FRAG;
        case spv::ExecutionModelGLCompute:
            return gal_shader_stage::COMP;
        default:
            return gal_shader_stage::UNDEFINED;
        }
    };

    gal_shader_stage stage = utils_to_gal_shader_stage(entry.model);
    reflect_shader_resources(resources.uniform_buffers, gal_descriptor_type::CONSTANT_BUFFER, stage);
    reflect_shader_resources(resources.storage_buffers, gal_descriptor_type::RW_BUFFER, stage);
    reflect_shader_resources(resources.separate_samplers, gal_descriptor_type::SAMPLER, stage);
    reflect_shader_resources(resources.separate_images, gal_descriptor_type::TEXTURE, stage);
    reflect_shader_resources(resources.storage_images, gal_descriptor_type::RW_TEXTURE, stage);

    return ret;
}
} // namespace ant::gal