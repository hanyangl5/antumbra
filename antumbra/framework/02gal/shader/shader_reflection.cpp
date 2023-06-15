#include "shader_compiler.h"

#include <dxcompiler/dxcapi.h>
#include <spirv_cross.hpp>

#include "framework/01core/memory/memory.h"

namespace ant::gal {

inline void read_resource_vec_size(const spirv_cross::Compiler &compiler, const spirv_cross::Resource &resource,
                                   ShaderResource &shader_resource) {
    const auto &spirv_type = compiler.get_type_from_variable(resource.id);

    shader_resource.vec_size = spirv_type.vecsize;
    shader_resource.columns = spirv_type.columns;
}

inline void read_resource_array_size(const spirv_cross::Compiler &compiler, const spirv_cross::Resource &resource,
                                     ShaderResource &shader_resource) {
    const auto &spirv_type = compiler.get_type_from_variable(resource.id);
    shader_resource.array_size = spirv_type.array.size() ? spirv_type.array[0] : 1;
}

inline void read_resource_size(const spirv_cross::Compiler &compiler, const spirv_cross::Resource &resource,
                               ShaderResource &shader_resource) {
    const auto &spirv_type = compiler.get_type_from_variable(resource.id);

    size_t array_size = 1;

    shader_resource.size = static_cast<u32>(compiler.get_declared_struct_size_runtime_array(spirv_type, array_size));
}

inline void read_resource_size(const spirv_cross::Compiler &compiler, const spirv_cross::SPIRConstant &constant,
                               ShaderResource &shader_resource) {
    auto spirv_type = compiler.get_type(constant.constant_type);

    switch (spirv_type.basetype) {
    case spirv_cross::SPIRType::BaseType::Boolean:
    case spirv_cross::SPIRType::BaseType::Char:
    case spirv_cross::SPIRType::BaseType::Int:
    case spirv_cross::SPIRType::BaseType::UInt:
    case spirv_cross::SPIRType::BaseType::Float:
        shader_resource.size = 4;
        break;
    case spirv_cross::SPIRType::BaseType::Int64:
    case spirv_cross::SPIRType::BaseType::UInt64:
    case spirv_cross::SPIRType::BaseType::Double:
        shader_resource.size = 8;
        break;
    default:
        shader_resource.size = 0;
        break;
    }
}
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
void compiled_shader::create_shader_reflection() {
    if (m_type == shader_blob_type::SPIRV) {
        create_shader_reflection_from_spirv();
    }
}

const blob *compiled_shader::byte_code() const { return &m_byte_code; }

const shader_reflection *compiled_shader::reflection() const { return m_reflection ? m_reflection : nullptr; }

const char *compiled_shader::entry() { return m_entry; }

void compiled_shader::create_shader_reflection_from_spirv() {
    spirv_cross::Compiler compiler((u32 *)this->m_byte_code.data(), this->m_byte_code.size() / sizeof(u32));

    auto &spv_entry = compiler.get_entry_point({this->m_entry}, compiler.get_execution_model());

    m_reflection->entry = this->m_entry;

    if (spv_entry.model == spv::ExecutionModelGLCompute) {
        m_reflection->work_group_size[0] = spv_entry.workgroup_size.x;
        m_reflection->work_group_size[1] = spv_entry.workgroup_size.y;
        m_reflection->work_group_size[2] = spv_entry.workgroup_size.z;
    }

    if (spv_entry.model == spv::ExecutionModelTessellationControl) {
        m_reflection->tesc_control_point = spv_entry.output_vertices;
    }

    gal_shader_stage stage = utils_to_gal_shader_stage(spv_entry.model);

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
    // count += (u32)resources.acceleration_structures.size(); // raytracing structures
    auto stack_memory = ant::stack_allocator(128);
    ant::hash_map<u32, u32> used_sets(&stack_memory);
    // stage inputs
    for (auto &input : resources.stage_inputs) {
        ShaderResource resource;
        resource.reg =
            compiler.get_decoration(input.id, spv::DecorationLocation); // location is the binding point for inputs
        resource.resource_type = ShaderResourceType::INPUT;
        resource.used_stages = stage;
        resource.name = std::move(input.name);
        //spirv_cross::SPIRType type = compiler.get_type(input.type_id);
        //resource.size = (type.width / 8) * type.vecsize;
        read_resource_vec_size(compiler, input, resource);
        read_resource_array_size(compiler, input, resource);
        m_reflection->resources.emplace(std::move(resource.name), std::move(resource));
    }
    // stage output
    for (u32 i = 0; i < static_cast<u32>(resources.stage_outputs.size()); ++i) {
        spirv_cross::Resource &input = resources.stage_outputs[i];
        ShaderResource resource;

        resource.reg =
            compiler.get_decoration(input.id, spv::DecorationLocation); // location is the binding point for inputs
        resource.resource_type = ShaderResourceType::OUTPUT;
        resource.used_stages = stage;
        resource.name = std::move(input.name);
        //spirv_cross::SPIRType type = compiler.get_type(input.type_id);
        //resource.size = (type.width / 8) * type.vecsize;
        read_resource_vec_size(compiler, input, resource);
        read_resource_array_size(compiler, input, resource);
        m_reflection->resources.emplace(std::move(resource.name), std::move(resource));
    }

    // push constants
    for (auto &input : resources.push_constant_buffers) {
        ShaderResource resource;

        resource.descriptor_type = gal_descriptor_type::PUSH_CONSTANT;
        resource.resource_type = ShaderResourceType::PUSH_CONSTANT;
        spirv_cross::SPIRType type = compiler.get_type(input.type_id);
        resource.size = static_cast<u32>(compiler.get_declared_struct_size(type));
        resource.name = std::move(input.name);
        m_reflection->resources.emplace(std::move(resource.name), std::move(resource));
    }

    // resources
    for (auto &input : resources.uniform_buffers) {
        ShaderResource resource;

        resource.descriptor_type = gal_descriptor_type::CONSTANT_BUFFER;
        resource.resource_type = ShaderResourceType::RESOURCE;
        resource.used_stages = stage;
        resource.name = std::move(input.name);

        resource.set = compiler.get_decoration(input.id, spv::DecorationDescriptorSet);
        used_sets[resource.set]++;
        resource.reg = compiler.get_decoration(input.id, spv::DecorationBinding);
        read_resource_size(compiler, input, resource);
        read_resource_array_size(compiler, input, resource);
        m_reflection->resources.emplace(std::move(resource.name), std::move(resource));
    }

    for (auto &input : resources.storage_buffers) {
        ShaderResource resource;

        resource.descriptor_type = gal_descriptor_type::RW_BUFFER;
        resource.resource_type = ShaderResourceType::RESOURCE;
        resource.used_stages = stage;
        resource.name = std::move(input.name);

        resource.set = compiler.get_decoration(input.id, spv::DecorationDescriptorSet);
        used_sets[resource.set]++;
        resource.reg = compiler.get_decoration(input.id, spv::DecorationBinding);
        read_resource_size(compiler, input, resource);
        read_resource_array_size(compiler, input, resource);
        m_reflection->resources.emplace(resource.name.c_str(), std::move(resource));
    }

    for (auto &input : resources.separate_images) {
        ShaderResource resource;

        resource.descriptor_type = gal_descriptor_type::TEXTURE;
        resource.resource_type = ShaderResourceType::RESOURCE;
        resource.used_stages = stage;
        resource.name = std::move(input.name);

        resource.set = compiler.get_decoration(input.id, spv::DecorationDescriptorSet);
        used_sets[resource.set]++;
        resource.reg = compiler.get_decoration(input.id, spv::DecorationBinding);
        read_resource_array_size(compiler, input, resource);
        //resource.dim = compiler.get_type(input.id).image.dim;
        m_reflection->resources.emplace(std::move(resource.name), std::move(resource));
    }

    for (auto &input : resources.storage_images) {
        ShaderResource resource;

        resource.descriptor_type = gal_descriptor_type::RW_TEXTURE;
        resource.resource_type = ShaderResourceType::RESOURCE;
        resource.used_stages = stage;
        resource.name = std::move(input.name);

        resource.set = compiler.get_decoration(input.id, spv::DecorationDescriptorSet);
        used_sets[resource.set]++;
        resource.reg = compiler.get_decoration(input.id, spv::DecorationBinding);
        //resource.dim = compiler.get_type(input.id).image.dim;
        read_resource_array_size(compiler, input, resource);
        m_reflection->resources.emplace(std::move(resource.name), std::move(resource));
    }

    for (auto &input : resources.separate_samplers) {
        ShaderResource resource;
        resource.descriptor_type = gal_descriptor_type::SAMPLER;
        resource.resource_type = ShaderResourceType::RESOURCE;
        resource.used_stages = stage;
        resource.name = std::move(input.name);
        resource.set = compiler.get_decoration(input.id, spv::DecorationDescriptorSet);
        used_sets[resource.set]++;
        resource.reg = compiler.get_decoration(input.id, spv::DecorationBinding);
        read_resource_array_size(compiler, input, resource);
        m_reflection->resources.emplace(std::move(resource.name), std::move(resource));
    }
    std::transform(used_sets.begin(), used_sets.end(), std::back_inserter(m_reflection->sets), [](const auto &s) {
        return (s.first << 16) | s.second;
    });
}

void compiled_shader_group::create_pipeline_reflection() {
    //Parameter checks
    if (m_stage_flags == gal_shader_stage::UNDEFINED) {
        LOG_ERROR("Parameter 'stageCount' is 0.");
        return;
    }

    m_pipeline_reflection = ant_alloc<pipeline_reflection>(ant::allocator::default_memory_allocator);

    if (m_b_same_root_signature) {
        // we only need to set from one shader beacuse all shader in one shader group share the same root signature
        shader_reflection *refl;
        refl = m_vert ? const_cast<shader_reflection *>(m_vert->reflection()) : nullptr;
        // vert/comp must exist for a shader group
        refl = m_comp ? const_cast<shader_reflection *>(m_comp->reflection()) : nullptr;

        //m_pipeline_reflection->m_resources =
        //    ant::vector<std::pair<const char *, ShaderResource>>(refl->resources.begin(), refl->resources.end());

        std::transform(refl->resources.begin(), refl->resources.end(),
                       std::back_inserter(m_pipeline_reflection->m_resources), [](const auto &p) { return p.second; });
        m_pipeline_reflection->sets = std::move((refl->sets));
        return;
    }

    //// Sanity check to make sure we don't have repeated stages.
    //ShaderStage combinedShaderStages = (ShaderStage)0;
    //for (u32 i = 0; i < stageCount; ++i) {
    //    if ((combinedShaderStages & pReflection[i].mShaderStage) != 0) {
    //       LOG_ERROR("Duplicate shader stage was detected in shader reflection array.");
    //        return;
    //    }
    //    combinedShaderStages = (ShaderStage)(combinedShaderStages | pReflection[i].mShaderStage);
    //}

    // Combine all shaders
    // this will have a large amount of looping
    // 1. count number of resources
    //u32 vertexStageIndex = ~0u;
    //u32 hullStageIndex = ~0u;
    //u32 domainStageIndex = ~0u;
    //u32 geometryStageIndex = ~0u;
    //u32 pixelStageIndex = ~0u;
    //ShaderResource *pResources = NULL;
    //u32 resourceCount = 0;
    //ShaderVariable *pVariables = NULL;
    //u32 variableCount = 0;

    //
    //
    //    //Should we be using dynamic arrays for these? Perhaps we can add std::vector
    //    // like functionality?
    //    ShaderResource *uniqueResources[512];
    //    ShaderStage shaderUsage[512];
    //    ShaderVariable *uniqueVariable[512];
    //    ShaderResource *uniqueVariableParent[512];
    //    for (u32 i = 0; i < stageCount; ++i) {
    //        ShaderReflection *pSrcRef = pReflection + i;
    //        pOutReflection->mStageReflections[i] = *pSrcRef;
    //
    //        if (pSrcRef->mShaderStage == gal_shader_stage::VERT) {
    //            vertexStageIndex = i;
    //        }
    //#if !defined(METAL)
    //        else if (pSrcRef->mShaderStage == gal_shader_stage::HULL) {
    //            hullStageIndex = i;
    //        } else if (pSrcRef->mShaderStage == gal_shader_stage::DOMN) {
    //            domainStageIndex = i;
    //        } else if (pSrcRef->mShaderStage == gal_shader_stage::GEOM) {
    //            geometryStageIndex = i;
    //        }
    //#endif
    //        else if (pSrcRef->mShaderStage == gal_shader_stage::FRAG) {
    //            pixelStageIndex = i;
    //        }
    //
    //        //Loop through all shader resources
    //        for (u32 j = 0; j < pSrcRef->mShaderResourceCount; ++j) {
    //            bool unique = true;
    //
    //            //Go through all already added shader resources to see if this shader
    //            // resource was already added from a different shader stage. If we find a
    //            // duplicate shader resource, we add the shader stage to the shader stage
    //            // mask of that resource instead.
    //            for (u32 k = 0; k < resourceCount; ++k) {
    //                unique = !ShaderResourceCmp(&pSrcRef->pShaderResources[j], uniqueResources[k]);
    //                if (unique == false) {
    //                    // update shader usage
    //                    // NOT SURE
    //                    //shaderUsage[k] = (ShaderStage)(shaderUsage[k] & pSrcRef->pShaderResources[j].used_stages);
    //                    shaderUsage[k] |= pSrcRef->pShaderResources[j].used_stages;
    //                    break;
    //                }
    //            }
    //
    //            //If it's unique, we add it to the list of shader resourceas
    //            if (unique == true) {
    //                shaderUsage[resourceCount] = pSrcRef->pShaderResources[j].used_stages;
    //                uniqueResources[resourceCount] = &pSrcRef->pShaderResources[j];
    //                resourceCount++;
    //            }
    //        }
    //
    //        //Loop through all shader variables (constant/uniform buffer members)
    //        for (u32 j = 0; j < pSrcRef->mVariableCount; ++j) {
    //            bool unique = true;
    //            //Go through all already added shader variables to see if this shader
    //            // variable was already added from a different shader stage. If we find a
    //            // duplicate shader variables, we don't add it.
    //            for (u32 k = 0; k < variableCount; ++k) {
    //                unique = !ShaderVariableCmp(&pSrcRef->pVariables[j], uniqueVariable[k]);
    //                if (unique == false)
    //                    break;
    //            }
    //
    //            //If it's unique we add it to the list of shader variables
    //            if (unique) {
    //                uniqueVariableParent[variableCount] = &pSrcRef->pShaderResources[pSrcRef->pVariables[j].parent_index];
    //                uniqueVariable[variableCount] = &pSrcRef->pVariables[j];
    //                variableCount++;
    //            }
    //        }
    //    }
    //
    //    //Copy over the shader resources in a dynamic array of the correct size
    //    if (resourceCount) {
    //        pResources = (ShaderResource *)ant_calloc(resourceCount, sizeof(ShaderResource));
    //
    //        for (u32 i = 0; i < resourceCount; ++i) {
    //            pResources[i] = *uniqueResources[i];
    //            pResources[i].used_stages = shaderUsage[i];
    //        }
    //    }
    //
    //    //Copy over the shader variables in a dynamic array of the correct size
    //    if (variableCount) {
    //        pVariables = (ShaderVariable *)ant_malloc(sizeof(ShaderVariable) * variableCount);
    //
    //        for (u32 i = 0; i < variableCount; ++i) {
    //            pVariables[i] = *uniqueVariable[i];
    //            ShaderResource *parentResource = uniqueVariableParent[i];
    //            // look for parent
    //            for (u32 j = 0; j < resourceCount; ++j) {
    //                if (ShaderResourceCmp(&pResources[j], parentResource)) //-V522
    //                {
    //                    pVariables[i].parent_index = j;
    //                    break;
    //                }
    //            }
    //        }
    //    }
    //
    //    // all refection structs should be built now
    //    pOutReflection->mShaderStages = combinedShaderStages;
    //
    //    pOutReflection->mStageReflectionCount = stageCount;
    //
    //    pOutReflection->mVertexStageIndex = vertexStageIndex;
    //    pOutReflection->mHullStageIndex = hullStageIndex;
    //    pOutReflection->mDomainStageIndex = domainStageIndex;
    //    pOutReflection->mGeometryStageIndex = geometryStageIndex;
    //    pOutReflection->mPixelStageIndex = pixelStageIndex;
    //
    //    pOutReflection->pShaderResources = pResources;
    //    pOutReflection->mShaderResourceCount = resourceCount;
    //
    //    pOutReflection->pVariables = pVariables;
    //    pOutReflection->mVariableCount = variableCount;
}

//void destroyPipelineReflection(PipelineReflection *pReflection) {
//    if (pReflection == NULL)
//        return;
//
//    for (u32 i = 0; i < pReflection->mStageReflectionCount; ++i)
//        destroyShaderReflection(&pReflection->mStageReflections[i]);
//
//    ant_free(pReflection->pShaderResources);
//    ant_free(pReflection->pVariables);
//}

} // namespace ant::gal