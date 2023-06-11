#pragma once

#include "framework/02gal/enum.h"

namespace ant::gal {


struct VertexInput {
    // resource name
    const char *name;

    // The size of the attribute
    u32 size;

    // name size
    u32 name_size;
};

enum class ShaderResourceType{
    UNDEFINED,
    INPUT,
    OUTPUT,
    PUSH_CONSTANT,
    RESOURCE
};

struct ShaderResource {
    // resource Type
    gal_descriptor_type descriptor_type = gal_descriptor_type::UNDEFINED;
    ShaderResourceType resource_type = ShaderResourceType::UNDEFINED;

    // The resource set for binding frequency
    u32 set = UINT_MAX;
    u32 binding = UINT_MAX;

    // The resource binding location
    u32 reg = UINT_MAX;

    // The size of the resource. This will be the DescriptorInfo array size for textures
    u32 size = 0;

    // what stages use this resource
    gal_shader_stage used_stages = gal_shader_stage::UNDEFINED;

    // resource name
    // const char *name;
    ant::str name;

    // name size
    //u32 name_size;

    // 1D / 2D / Array / MSAA / ...
    gal_texture_dimension dim = gal_texture_dimension::UNDEFINED;
};

struct ShaderVariable {
    // Variable name
    const char *name;

    // parents resource index
    u32 parent_index;

    // The offset of the Variable.
    u32 offset;

    // The size of the Variable.
    u32 size;

    // name size
    u32 name_size;
};

struct ShaderReflection {
    // single large allocation for names to reduce number of allocations
    char *pNamePool;
    VertexInput *pVertexInputs;
    ant::vector<ShaderResource> resources;
    ShaderVariable *pVariables;

    const char *entry;
    gal_shader_stage mShaderStage;

    u32 mNamePoolSize;
    u32 mVertexInputsCount;
    u32 mShaderResourceCount;
    u32 mVariableCount;

    // Thread group size for compute shader
    ant::fixed_array<u32, 3> work_group_size;
    
    //number of tessellation control point
    u32 tesc_control_point;
};

struct PipelineReflection {
    gal_shader_stage mShaderStages;
    // the individual stages reflection data.
    ShaderReflection mStageReflections[MAX_SHADER_STAGE_COUNT];
    u32 mStageReflectionCount;

    u32 mVertexStageIndex;
    u32 mHullStageIndex;
    u32 mDomainStageIndex;
    u32 mGeometryStageIndex;
    u32 mPixelStageIndex;

    ShaderResource *pShaderResources;
    u32 mShaderResourceCount;

    ShaderVariable *pVariables;
    u32 mVariableCount;
};

PipelineReflection CreatePipelineReflection();

} // namespace ant::gal
