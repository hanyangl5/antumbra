/*
 * Code adapted from ConfettiFX The-Forge
 * 
 * Copyright (c) 2017-2022 The Forge Interactive Inc.
 *
 * This file is part of The-Forge
 * (see https://github.com/ConfettiFX/The-Forge).
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
*/

#pragma once

#include <variant>
#ifdef WIN32
#include <Windows.h>
#endif
#include "framework/01core/utils/utils.h"
#include "framework/01core/memory/container.h"
#include "format.h"

namespace ant::gal {

inline constexpr u32 MAX_RESOURCE_NAME_LENGTH = 256;
inline constexpr u32 MAX_RENDER_TARGET_ATTACHMENTS = 8;
inline constexpr u32 MAX_VERTEX_BINDINGS = 15;
inline constexpr u32 MAX_VERTEX_ATTRIBS = 15;
inline constexpr u32 MAX_SEMANTIC_NAME_LENGTH = 128;
inline constexpr u32 MAX_DEBUG_NAME_LENGTH = 128;
inline constexpr u32 MAX_MIP_LEVELS = 0xFFFFFFFF;
inline constexpr u32 MAX_SWAPCHAIN_IMAGES = 3;
inline constexpr u32 MAX_GPU_VENDOR_STRING_LENGTH = 64; //max size for GPUVendorPreset strings
inline constexpr u32 MAX_SHADER_STAGE_COUNT = 5; //max size for GPUVendorPreset strings

#ifndef MAKE_ENUM_FLAG
#define MAKE_ENUM_FLAG(TYPE, ENUM_TYPE)                                                                                \
    inline constexpr ENUM_TYPE operator|(ENUM_TYPE a, ENUM_TYPE b) { return ENUM_TYPE(((TYPE)a) | ((TYPE)b)); }  \
    inline ENUM_TYPE &operator|=(ENUM_TYPE &a, ENUM_TYPE b) { return (ENUM_TYPE &)(((TYPE &)a) |= ((TYPE)b)); }        \
    inline constexpr ENUM_TYPE operator&(ENUM_TYPE a, ENUM_TYPE b) { return ENUM_TYPE(((TYPE)a) & ((TYPE)b)); }  \
    inline ENUM_TYPE &operator&=(ENUM_TYPE &a, ENUM_TYPE b) { return (ENUM_TYPE &)(((TYPE &)a) &= ((TYPE)b)); }        \
    inline constexpr ENUM_TYPE operator~(ENUM_TYPE a) { return ENUM_TYPE(~((TYPE)a)); }                          \
    inline constexpr ENUM_TYPE operator^(ENUM_TYPE a, ENUM_TYPE b) { return ENUM_TYPE(((TYPE)a) ^ ((TYPE)b)); }  \
    inline ENUM_TYPE &operator^=(ENUM_TYPE &a, ENUM_TYPE b) { return (ENUM_TYPE &)(((TYPE &)a) ^= ((TYPE)b)); }
#else
#define MAKE_ENUM_FLAG(TYPE, ENUM_TYPE)
#endif

enum class gal_error_code : u32 {
    GAL_ERRORCODE_SUCCESS,
    GAL_ERRORCODE_ERROR,
    GAL_ERRORCODE_INVALID_PLATFORM,
    GAL_ERRORCODE_INVALID_DEVICE,
    GAL_ERRORCODE_INVALID_PARAMETER
};

enum class gal_api { UNDEFINED, VULKAN, D3D12 };

#define DECLARE_GAL_HANDLE(name)                                                                                       \
    struct name##_T;                                                                                                   \
    using name = name##_T *;                                                                                           \
    struct name##_T

// declare gal handle
inline constexpr nullptr_t gal_null = nullptr;

struct gal_desc {
    // gal optional feature
    bool b_debug_layer : 1;
    bool b_async_compute : 1;
    bool b_async_transfer : 1;
    bool b_ray_tracing : 1;
    bool b_mesh_shader : 1;
    bool b_variable_rate_shading : 1;
    bool b_swap_chain : 1;
    bool b_surface : 1;
};

enum class gal_blend_mode {
    UNDEFINED,
    ADD,
    SUBTRACT,
    REVERSE_SUBTRACT,
    MIN,
    MAX
};

enum class gal_stencil_op {
    UNDEFINED,
    KEEP,
    SET_ZERO,
    REPLACE,
    INVERT,
    INCR,
    DECR,
    INCR_SAT,
    DECR_SAT,
};
enum BlendStateTargets {
    BLEND_STATE_TARGET_0 = 0x1,
    BLEND_STATE_TARGET_1 = 0x2,
    BLEND_STATE_TARGET_2 = 0x4,
    BLEND_STATE_TARGET_3 = 0x8,
    BLEND_STATE_TARGET_4 = 0x10,
    BLEND_STATE_TARGET_5 = 0x20,
    BLEND_STATE_TARGET_6 = 0x40,
    BLEND_STATE_TARGET_7 = 0x80,
    BLEND_STATE_TARGET_ALL = 0xFF,
};

enum class gal_cull_mode { UNDEFINED = 0, NONE, BACK, FRONT, BOTH };

enum class gal_front_face { UNDEFINED = 0, CCW, CW };

enum class gal_polygon_fill_mode { UNDEFINED, SOLID, WIREFRAME};

enum class gal_pipeline_type {
    UNDEFINED = 0,
    COMPUTE,
    GRAPHICS,
    RAYTRACING,
    PIPELINE_TYPE_COUNT,
};

enum class gal_texture_dimension { UNDEFINED, td_1D, td_2D, td_3D };

enum class gal_texture_sample_count {
    UNDEFINED,
    SAMPLE_COUNT_1,
    SAMPLE_COUNT_2,
    SAMPLE_COUNT_4,
    SAMPLE_COUNT_8,
    SAMPLE_COUNT_16,
    SAMPLE_COUNT_32,
    SAMPLE_COUNT_64,
};

enum class gal_sampler_filter_mode {
    UNDEFINED,
    POINT,
    LINEAR,
    // cubic, anisotropic?
};
enum class gal_compare_mode {
    UNDEFINED,
    NEVER,
    LESS,
    LESS_EQUAL,
    EQUAL,
    NOT_EQUAL,
    GREATER,
    GREATER_EQUAL,
    ALWAYS
};

enum class gal_sampler_mip_mode { UNDEFINED, POINT, LINEAR };

enum class gal_sampler_address_mode { UNDEFINED, REPEAT, MIRROR, CLAMP, BORDER };

enum class gal_texture_flag {
    TEXTURE_CREATION_FLAG_UNFEFINED = 0,
    /// Texture will allocate its own memory (COMMITTED resource)
    TEXTURE_CREATION_FLAG_OWN_MEMORY_BIT = 0x01,
    /// Texture will be allocated in memory which can be shared among multiple processes
    /// Texture will be imported from a handle created in another process
    TEXTURE_CREATION_FLAG_IMPORT_BIT = 0x08,
    /// Use ESRAM to store this texture
    TEXTURE_CREATION_FLAG_ESRAM = 0x10,
    /// Use on-tile memory to store this texture
    TEXTURE_CREATION_FLAG_ON_TILE = 0x20,
    /// Prevent compression meta data from generating (XBox)
    TEXTURE_CREATION_FLAG_NO_COMPRESSION = 0x40,
    /// Force 2D instead of automatically determining dimension based on width, height, depth
    TEXTURE_CREATION_FLAG_FORCE_2D = 0x80,
    /// Force 3D instead of automatically determining dimension based on width, height, depth
    TEXTURE_CREATION_FLAG_FORCE_3D = 0x100,
    /// Display target
    TEXTURE_CREATION_FLAG_ALLOW_DISPLAY_TARGET = 0x200,
    /// Create an sRGB texture.
    TEXTURE_CREATION_FLAG_SRGB = 0x400,
    /// Create a normal map texture
    TEXTURE_CREATION_FLAG_NORMAL_MAP = 0x800,
    /// Fast clear
    TEXTURE_CREATION_FLAG_FAST_CLEAR = 0x1000,
    /// Fragment mask
    TEXTURE_CREATION_FLAG_FRAG_MASK = 0x2000,
};
MAKE_ENUM_FLAG(u32, gal_texture_flag)
enum class gal_shader_stage {
    UNDEFINED = 0,
    VERT = 0X00000001,
    TESC = 0X00000002,
    TESE = 0X00000004,
    GEOM = 0X00000008,
    FRAG = 0X00000010,
    COMP = 0X00000020,
    RAYTRACING = 0X00000040,
    ALL_GRAPHICS =
        ((uint32_t)gal_shader_stage::VERT | (uint32_t)gal_shader_stage::TESC | (uint32_t)gal_shader_stage::TESE |
         (uint32_t)gal_shader_stage::GEOM | (uint32_t)gal_shader_stage::FRAG),
    HULL = gal_shader_stage::TESC,
    DOMN = gal_shader_stage::TESE,
};

MAKE_ENUM_FLAG(u32, gal_shader_stage)

inline constexpr u32 gal_shader_stage_count = 7;

enum class PrimitiveTopology {
    PRIMITIVE_TOPO_POINT_LIST = 0,
    PRIMITIVE_TOPO_LINE_LIST,
    PRIMITIVE_TOPO_LINE_STRIP,
    PRIMITIVE_TOPO_TRI_LIST,
    PRIMITIVE_TOPO_TRI_STRIP,
    PRIMITIVE_TOPO_PATCH_LIST,
    PRIMITIVE_TOPO_COUNT,
};

enum class IndexType {
    INDEX_TYPE_UINT32 = 0,
    INDEX_TYPE_UINT16,
};
enum VertexAttribRate {
    VERTEX_ATTRIB_RATE_VERTEX = 0,
    VERTEX_ATTRIB_RATE_INSTANCE = 1,
    VERTEX_ATTRIB_RATE_COUNT,
};

enum class ShaderSemantic {
    SEMANTIC_UNDEFINED = 0,
    SEMANTIC_POSITION,
    SEMANTIC_NORMAL,
    SEMANTIC_COLOR,
    SEMANTIC_TANGENT,
    SEMANTIC_BITANGENT,
    SEMANTIC_JOINTS,
    SEMANTIC_WEIGHTS,
    SEMANTIC_SHADING_RATE,
    SEMANTIC_TEXCOORD0,
    SEMANTIC_TEXCOORD1,
    SEMANTIC_TEXCOORD2,
    SEMANTIC_TEXCOORD3,
    SEMANTIC_TEXCOORD4,
    SEMANTIC_TEXCOORD5,
    SEMANTIC_TEXCOORD6,
    SEMANTIC_TEXCOORD7,
    SEMANTIC_TEXCOORD8,
    SEMANTIC_TEXCOORD9,
};

enum class gal_queue_type { UNDEFINED, graphcis, compute, transfer };

// Enum for descriptor types
enum class gal_descriptor_type : u32 {
    UNDEFINED = 0,
    // constant buffer
    CONSTANT_BUFFER = 1 << 0,
    // read write buffer
    RW_BUFFER = 1 << 1,
    // read only texture
    TEXTURE = 1 << 2,
    // read write texture
    RW_TEXTURE = 1 << 3,
    TEXTURE_CUBE = 1 << 4,
    SAMPLER = 1 << 5,
    PUSH_CONSTANT = 1 << 6,
    VERTEX_BUFFER = 1 << 7,
    INDEX_BUFFER = 1 << 8,
    INDIRECT_ARGUMENT = 1 << 9,
    COLOR_RT = 1 << 10,
    DEPTH_STENCIL_RT = 1 << 11,
};
MAKE_ENUM_FLAG(u32, gal_descriptor_type)

enum class gal_resource_state : u32 {
    UNDEFINIED = 0,
    // Your application should transition to this state only for accessing a resource across differentgraphics engine
    // types.
    COMMON = 1 << 0,
    VERTEX_BUFFER = 1 << 1,
    INDEX_BUFFER = 1 << 2,
    INDIRECT_BUFFER = 1 << 3,
    // shader resource
    CONSTANT_BUFFER = 1 << 4,
    RW_BUFFER = 1 << 5,
    TEXTURE = 1 << 6,
    RW_TEXTURE = 1 << 7,
    COPY_SRC = 1 << 8,
    COPY_DST = 1 << 9,
    CPU_READ = 1 << 10,
    CPU_WRITE = 1 << 11,
    RENDER_TARGET = 1 << 12,
    PRESENT = 1 << 13,
    DEPTH_READ = 1 << 14,
    DEPTH_WRITE = 1 << 15,
    // rt_acce_structure,
    // shading_rate_src
};
MAKE_ENUM_FLAG(u32, gal_resource_state)

enum class gal_memory_flag {
    UNDEFINED = 0,
    GPU_DEDICATED = 1 << 0, // gpu dedicated memory
    CPU_UPLOAD = 1 << 1,    // cpu upload memory
    GPU_DOWNLOAD = 1 << 2,  // gpu download
};
MAKE_ENUM_FLAG(u32, gal_memory_flag);

enum class gal_buffer_flag {
    /// Default flag (Buffer will use aliased memory, buffer will not be cpu accessible until mapBuffer is called)
    UNDEFINED = 0,
    /// Buffer will allocate its own memory (COMMITTED resource)
    OWN_MEMORY = 1 << 0,
    /// Buffer will be persistently mapped
    PERSISTENT_MAP = 1 << 1,
    /// Use ESRAM to store this buffer
    ESRAM = 1 << 2,
    /// Flag to specify not to allocate descriptors for the resource
    //NO_DESCRIPTOR_VIEW_CREATION = 1 << 3
};
MAKE_ENUM_FLAG(u32, gal_buffer_flag)

enum class BlendConstant {
    BC_ZERO = 0,
    BC_ONE,
    BC_SRC_COLOR,
    BC_ONE_MINUS_SRC_COLOR,
    BC_DST_COLOR,
    BC_ONE_MINUS_DST_COLOR,
    BC_SRC_ALPHA,
    BC_ONE_MINUS_SRC_ALPHA,
    BC_DST_ALPHA,
    BC_ONE_MINUS_DST_ALPHA,
    BC_SRC_ALPHA_SATURATE,
    BC_BLEND_FACTOR,
    BC_ONE_MINUS_BLEND_FACTOR,
    MAX_BLEND_CONSTANTS
};

struct gal_descriptorpool_desc {};

struct gal_blend_state_desc {
    /// Source blend factor per render target.
    BlendConstant mSrcFactors[MAX_RENDER_TARGET_ATTACHMENTS];
    /// Destination blend factor per render target.
    BlendConstant mDstFactors[MAX_RENDER_TARGET_ATTACHMENTS];
    /// Source alpha blend factor per render target.
    BlendConstant mSrcAlphaFactors[MAX_RENDER_TARGET_ATTACHMENTS];
    /// Destination alpha blend factor per render target.
    BlendConstant mDstAlphaFactors[MAX_RENDER_TARGET_ATTACHMENTS];
    /// Blend mode per render target.
    gal_blend_mode mBlendModes[MAX_RENDER_TARGET_ATTACHMENTS];
    /// Alpha blend mode per render target.
    gal_blend_mode mBlendAlphaModes[MAX_RENDER_TARGET_ATTACHMENTS];
    /// Write mask per render target.
    int32_t mMasks[MAX_RENDER_TARGET_ATTACHMENTS];
    /// Mask that identifies the render targets affected by the blend state.
    BlendStateTargets mRenderTargetMask;
    /// Set whether alpha to coverage should be enabled.
    bool mAlphaToCoverage;
    /// Set whether each render target has an unique blend function. When false the blend function in slot 0 will be used for all render targets.
    bool mIndependentBlend;
};

struct gal_depth_state_desc {
    bool mDepthTest;
    bool mDepthWrite;
    gal_compare_mode mDepthFunc;
    bool mStencilTest;
    uint8_t mStencilReadMask;
    uint8_t mStencilWriteMask;
    gal_compare_mode mStencilFrontFunc;
    gal_stencil_op mStencilFrontFail;
    gal_stencil_op mDepthFrontFail;
    gal_stencil_op mStencilFrontPass;
    gal_compare_mode mStencilBackFunc;
    gal_stencil_op mStencilBackFail;
    gal_stencil_op mDepthBackFail;
    gal_stencil_op mStencilBackPass;
};

struct gal_rasterizer_state_desc {
    gal_cull_mode mCullMode;
    int32_t mDepthBias;
    float mSlopeScaledDepthBias;
    gal_polygon_fill_mode mFillMode;
    gal_front_face mFrontFace;
    bool mMultiSample;
    bool mScissor;
    bool mDepthClampEnable;
};

struct gal_vertex_attrib {
    ShaderSemantic mSemantic;
    uint32_t mSemanticNameLength;
    char mSemanticName[MAX_SEMANTIC_NAME_LENGTH];
    gal_texture_format mFormat;
    uint32_t mBinding;
    uint32_t mLocation;
    uint32_t mOffset;
    VertexAttribRate mRate;
};

struct gal_vertex_layout {
    uint32_t mAttribCount;
    gal_vertex_attrib mAttribs[MAX_VERTEX_ATTRIBS];
    uint32_t mStrides[MAX_VERTEX_BINDINGS];
};

struct gal_read_range {
    uint64_t mOffset;
    uint64_t mSize;
};

struct gal_clear_value {
    struct rgb{
        float r;
        float g;
        float b;
        float a;
    };
    struct ds{
        float depth;
        uint32_t stencil;
    };
    std::variant<std::monostate, rgb, ds> value;
};

DECLARE_GAL_HANDLE(gal_context) {
  public:
    gal_desc m_gal_desc;
};

struct gal_buffer_desc {
    u64 size;                          // size
    gal_descriptor_type descriptor_types; // descriptor types of buffer
    gal_resource_state initial_state; // initial state of buffer
    gal_memory_flag memory_flags;
    gal_buffer_flag flags;
};

DECLARE_GAL_HANDLE(gal_buffer) {
  public:
    gal_buffer_desc m_gal_buffer_desc;
};

struct gal_texture_desc {
    u32 width;
    u32 height;
    u32 depth = 1;
    u32 array_size = 1;
    u32 mip_level = 1;
    gal_memory_flag memory_flags;
    gal_texture_dimension dimension;
    gal_texture_sample_count sample_count;
    u32 texture_sample_quality;
    gal_texture_format format;
    gal_texture_flag texture_flags;
    gal_descriptor_type descriptor_types; // descriptor types of buffer
    gal_resource_state initial_state; // initial state of buffer
    void *native_handle;
};

DECLARE_GAL_HANDLE(gal_texture) {
  public:
    gal_texture_desc m_gal_texture_desc;
};

struct gal_render_target_desc {
    gal_texture_flag flags;
    gal_texture_format format;
    u32 width;
    u32 height;
    u32 depth = 1;
    u32 mip_level = 1;
    gal_texture_dimension dimension;
    gal_texture_sample_count sample_count;
    u32 texture_sample_quality;
    gal_clear_value clear_value;
    gal_descriptor_type descriptor_types; // descriptor types of buffer
    gal_resource_state initial_state; // initial state of buffer
    void *native_handle;
};

DECLARE_GAL_HANDLE(gal_render_target) {
  public:
    gal_render_target_desc m_desc;
    gal_texture m_texture;
};

struct gal_sampler_desc {
    gal_sampler_filter_mode mag_filter; // filter metho when texel is larger than pixel
    gal_sampler_filter_mode min_filter; // filter method when texel is smaller than pixel
    gal_sampler_mip_mode mip_mode;
    gal_sampler_address_mode address_mode_u;
    gal_sampler_address_mode address_mode_v;
    gal_sampler_address_mode address_mode_w;
    gal_compare_mode compare_mode;
    f32 mip_lod_bias;
    f32 min_lod;
    f32 max_lod;
    f32 max_anisotropy;
    ant::fixed_array<f32, 4> border_color;
};
DECLARE_GAL_HANDLE(gal_sampler) {
  public:
    gal_sampler_desc m_gal_sampler_desc;
};

DECLARE_GAL_HANDLE(gal_fence){};
DECLARE_GAL_HANDLE(gal_semaphore){};

struct gal_swap_chain_desc {
    gal_clear_value clear_value;
#ifdef WIN32
    HWND hwnd_window;
#endif
    bool b_present;
    u32 image_count;
    u32 width;
    u32 height;
    gal_texture_format format;
    bool b_vsync;
};

DECLARE_GAL_HANDLE(gal_swap_chain) {  
    gal_swap_chain_desc m_desc;
    ant::fixed_array<gal_render_target, MAX_SWAPCHAIN_IMAGES> m_render_targets;
};

enum class TextureDimension {
    TEXTURE_DIM_1D,
    TEXTURE_DIM_2D,
    TEXTURE_DIM_2DMS,
    TEXTURE_DIM_3D,
    TEXTURE_DIM_CUBE,
    TEXTURE_DIM_1D_ARRAY,
    TEXTURE_DIM_2D_ARRAY,
    TEXTURE_DIM_2DMS_ARRAY,
    TEXTURE_DIM_CUBE_ARRAY,
    TEXTURE_DIM_COUNT,
    TEXTURE_DIM_UNDEFINED,
};

struct VertexInput {
    // resource name
    const char *name;

    // The size of the attribute
    u32 size;

    // name size
    u32 name_size;
};

struct ShaderResource {
    // resource Type
    gal_descriptor_type type;

    // The resource set for binding frequency
    u32 set;

    // The resource binding location
    u32 reg;

    // The size of the resource. This will be the DescriptorInfo array size for textures
    u32 size;

    // what stages use this resource
    gal_shader_stage used_stages;

    // resource name
    const char *name;

    // name size
    u32 name_size;

    // 1D / 2D / Array / MSAA / ...
    TextureDimension dim;
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
    ShaderResource *pShaderResources;
    ShaderVariable *pVariables;

#if defined(VULKAN)
    char *pEntryPoint;
#endif

    gal_shader_stage mShaderStage;

    u32 mNamePoolSize;
    u32 mVertexInputsCount;
    u32 mShaderResourceCount;
    u32 mVariableCount;

    // Thread group size for compute shader
    u32 mNumThreadsPerGroup[3];

    //number of tessellation control point
    u32 mNumControlPoint;
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

struct gal_shader_desc {
    u64 size;
    void *data;
};

DECLARE_GAL_HANDLE(gal_shader) {
    gal_shader_stage stage_flag : 31;
    u32 mNumThreadsPerGroup[3];
    PipelineReflection *pReflection;
};


struct BinaryShaderStageDesc {
    /// Byte code array
    void *byte_code;
    u64 size;
    const char *entry;
};

struct gal_shader_program_desc {
    gal_shader_stage stage_flag;
    BinaryShaderStageDesc vert;
    BinaryShaderStageDesc frag;
    BinaryShaderStageDesc geom;
    BinaryShaderStageDesc hull;
    BinaryShaderStageDesc domain;
    BinaryShaderStageDesc comp;
};

// shader program is a set of shader for single pipeline
DECLARE_GAL_HANDLE(gal_shader_program) {
    gal_shader_program_desc m_desc;
    u32 m_stage_count;
    //ShaderStage mStages : 31;
    //uint32_t mNumThreadsPerGroup[3];
    //PipelineReflection *pReflection;
};

DECLARE_GAL_HANDLE(gal_rootsignature){};

DECLARE_GAL_HANDLE(gal_pipeline_cache){};

struct gal_compute_pipeline_desc {
    gal_shader_program* shader;
    gal_rootsignature root_signature;
};
struct gal_raytracing_pipeline_desc {};

struct gal_graphics_pipeline_desc {
    gal_shader_program *shader;
    gal_rootsignature root_signature;
    gal_vertex_layout *pVertexLayout;
    gal_blend_state_desc *pBlendState;
    gal_depth_state_desc *pDepthState;
    gal_rasterizer_state_desc *pRasterizerState;
    gal_texture_format *pColorFormats;
    uint32_t mRenderTargetCount;
    gal_texture_sample_count mSampleCount;
    uint32_t mSampleQuality;
    gal_texture_format mDepthStencilFormat;
    PrimitiveTopology mPrimitiveTopo;
    bool mSupportIndirectCommandBuffer;
    bool mVRFoveatedRendering;
};

struct gal_pipeline_desc {
    std::variant<std::monostate, gal_graphics_pipeline_desc, gal_compute_pipeline_desc, gal_raytracing_pipeline_desc>
        desc;
    gal_pipeline_cache pipeline_cache;
};


DECLARE_GAL_HANDLE(gal_pipeline) { 
    gal_pipeline_desc m_desc;
    gal_pipeline_type m_type;
};
DECLARE_GAL_HANDLE(gal_commandlist){};
DECLARE_GAL_HANDLE(gal_descriptorpool){};

struct gal_pipeline_cache_desc {
    ant::str filename;
};

//struct RaytracingPipelineDesc {
//    Raytracing *pRaytracing;
//    gal_rootsignature *pGlobalRootSignature;
//    gal_shader *pRayGenShader;
//    gal_rootsignature *pRayGenRootSignature;
//    gal_shader **ppMissShaders;
//    gal_rootsignature **ppMissRootSignatures;
//    RaytracingHitGroup *pHitGroups;
//    gal_rootsignature *pEmptyRootSignature;
//    u32 mMissShaderCount;
//    u32 mHitGroupCount;
//    // #TODO : Remove this after adding shader reflection for raytracing shaders
//    u32 mPayloadSize;
//    // #TODO : Remove this after adding shader reflection for raytracing shaders
//    u32 mAttributeSize;
//    u32 mMaxTraceRecursionDepth;
//    u32 mMaxRaysCount;
//};

struct gal_commandpool_desc {};

// struct to consume descriptorset

struct gal_descriptorset_desc {};

struct gal_rootsignature_desc {};

struct gal_fence_desc {};

struct gal_semaphore_desc {};
struct gal_command_pool_desc {};

// desc to allocate commandlist
struct gal_commandlist_desc {};

struct gal_renderpass_begin_desc {};

struct BufferBarrier {
    gal_buffer *pBuffer;
    gal_resource_state mCurrentState;
    gal_resource_state mNewState;
    u8 mBeginOnly : 1;
    u8 mEndOnly : 1;
    u8 mAcquire : 1;
    u8 mRelease : 1;
    u8 mQueueType : 5;
};

struct TextureBarrier {
    gal_texture *pTexture;
    gal_resource_state mCurrentState;
    gal_resource_state mNewState;
    u8 mBeginOnly : 1;
    u8 mEndOnly : 1;
    u8 mAcquire : 1;
    u8 mRelease : 1;
    u8 mQueueType : 5;
    /// Specifiy whether following barrier targets particular subresource
    u8 mSubresourceBarrier : 1;
    /// Following values are ignored if mSubresourceBarrier is false
    u8 mMipLevel : 7;
    uint16_t mArrayLayer;
};
struct RenderTargetBarrier {
    gal_render_target *pRenderTarget;
    gal_resource_state mCurrentState;
    gal_resource_state mNewState;
    u8 mBeginOnly : 1;
    u8 mEndOnly : 1;
    u8 mAcquire : 1;
    u8 mRelease : 1;
    u8 mQueueType : 5;
    /// Specifiy whether following barrier targets particular subresource
    u8 mSubresourceBarrier : 1;
    /// Following values are ignored if mSubresourceBarrier is false
    u8 mMipLevel : 7;
    uint16_t mArrayLayer;
};


//enum class gal_descriptor_resource_type {
//    UNDEFINED,
//    BUFFER,
//    TEXTURE
//};
//struct gal_buffer_srv_desc {
//    gal_buffer buffer;
//    u64 offste;
//    u64 range;
//};
//using gal_buffer_uav_desc = gal_buffer_srv_desc;
//struct gal_texture_srv_desc {};
//using gal_texture_uav_desc = gal_texture_srv_desc;
//struct gal_src_descriptor_view_desc {
//    gal_descriptor_resource_type type;
//    std::variant<std::monostate, gal_buffer_srv_desc, gal_texture_srv_desc> srv_desc;
//};
//
//struct gal_uav_descriptor_view_desc {
//    gal_descriptor_resource_type type;
//    std::variant<std::monostate, gal_texture_uav_desc, gal_texture_uav_desc> srv_desc;
//};
//
//DECLARE_GAL_HANDLE(gal_srv_descriptor_view) {
//  protected:
//    gal_src_descriptor_view_desc m_desc;
//};
//
//DECLARE_GAL_HANDLE(gal_uav_descriptor_view) {
//  protected:
//    gal_uav_descriptor_view_desc m_desc;
//};

} // namespace ant::gal