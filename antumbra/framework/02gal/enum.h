#pragma once

#include "framework/01core/utils/utils.h"

namespace ant::gal {

inline constexpr u32 MAX_RENDER_TARGET_ATTACHMENTS = 8;
inline constexpr u32 MAX_VERTEX_BINDINGS = 15;
inline constexpr u32 MAX_VERTEX_ATTRIBS = 15;
inline constexpr u32 MAX_SEMANTIC_NAME_LENGTH = 128;
inline constexpr u32 MAX_DEBUG_NAME_LENGTH = 128;
inline constexpr u32 MAX_MIP_LEVELS = 0xFFFFFFFF;
inline constexpr u32 MAX_SWAPCHAIN_IMAGES = 3;
inline constexpr u32 MAX_GPU_VENDOR_STRING_LENGTH = 64; //max size for GPUVendorPreset strings

enum class gal_error_code : u32 { success, unsupporte_platform, error, unsupported_device, invalid_parameter };

enum class RenderApi { undefined, vulkan, d3d12 };

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

enum class BlendMode {
    BM_ADD,
    BM_SUBTRACT,
    BM_REVERSE_SUBTRACT,
    BM_MIN,
    BM_MAX,
    MAX_BLEND_MODES,
};

enum class StencilOp {
    STENCIL_OP_KEEP,
    STENCIL_OP_SET_ZERO,
    STENCIL_OP_REPLACE,
    STENCIL_OP_INVERT,
    STENCIL_OP_INCR,
    STENCIL_OP_DECR,
    STENCIL_OP_INCR_SAT,
    STENCIL_OP_DECR_SAT,
    MAX_STENCIL_OPS,
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

enum class gal_cull_mode { CULL_MODE_NONE = 0, CULL_MODE_BACK, CULL_MODE_FRONT, CULL_MODE_BOTH, MAX_CULL_MODES };

enum class gal_front_face { FRONT_FACE_CCW = 0, FRONT_FACE_CW };

enum class FillMode { FILL_MODE_SOLID, FILL_MODE_WIREFRAME, MAX_FILL_MODES };

enum class PipelineType {
    PIPELINE_TYPE_UNDEFINED = 0,
    PIPELINE_TYPE_COMPUTE,
    PIPELINE_TYPE_GRAPHICS,
    PIPELINE_TYPE_RAYTRACING,
    PIPELINE_TYPE_COUNT,
};

enum class gal_texture_dimension { undefined, td_1D, td_2D, td_3D };

enum class gal_texture_sample_count {
    tsc_1,
    tsc_2,
    tsc_4,
    tsc_8,
    tsc_16,
    tsc_32,
};

enum class gal_sampler_filter_mode {
    point,
    linear,
    // cubic, anisotropic?
};
enum class gal_compare_mode {
    undefined,
    never,
    less,
    less_equal,
    equal,
    not_equal,
    greater,
    greater_equal,
    always,
};

enum class gal_sampler_mip_mode { point, linear };

enum class gal_sampler_address_mode { repeat, mirror, clamp, border };

enum gal_texture_flag {
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
enum ShaderStage {
    SHADER_STAGE_NONE = 0,
    SHADER_STAGE_VERT = 0X00000001,
    SHADER_STAGE_TESC = 0X00000002,
    SHADER_STAGE_TESE = 0X00000004,
    SHADER_STAGE_GEOM = 0X00000008,
    SHADER_STAGE_FRAG = 0X00000010,
    SHADER_STAGE_COMP = 0X00000020,
    SHADER_STAGE_RAYTRACING = 0X00000040,
    SHADER_STAGE_ALL_GRAPHICS =
        ((uint32_t)SHADER_STAGE_VERT | (uint32_t)SHADER_STAGE_TESC | (uint32_t)SHADER_STAGE_TESE |
         (uint32_t)SHADER_STAGE_GEOM | (uint32_t)SHADER_STAGE_FRAG),
    SHADER_STAGE_HULL = SHADER_STAGE_TESC,
    SHADER_STAGE_DOMN = SHADER_STAGE_TESE,
    SHADER_STAGE_COUNT = 7,
};

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

enum class gal_queue_type { undefined, graphcis, compute, transfer };

// Enum for descriptor types
enum gal_resource_type : u32 {
    rt_none = 0,
    // constant buffer
    rt_constant_buffer = 1 << 0,
    // read write buffer
    rt_rw_buffer = 1 << 1,
    // read only texture
    rt_texture = 1 << 2,
    // read write texture
    rt_rw_texture = 1 << 3,
    rt_texture_cube = 1 << 4,
    rt_sampler = 1 << 5,
    rt_push_constant = 1 << 6,
    rt_vertex_buffer = 1 << 7,
    rt_index_buffer = 1 << 8,
    rt_indirect_buffer = 1 << 9,
    rt_color_rt = 1 << 10,
    rt_depth_stencil_rt = 1 << 11,
};

using gal_resource_types = u32;

enum gal_resource_state : u32 {
    rs_none = 0,
    // Your application should transition to this state only for accessing a resource across differentgraphics engine
    // types.
    rs_common = 1 << 0,
    rs_vertex_buffer = 1 << 1,
    rs_index_buffer = 1 << 2,
    rs_indiret_buffer = 1 << 3,
    // shader resource
    rs_constant_buffer = 1 << 4,
    rs_rw_buffer = 1 << 5,
    rs_texture = 1 << 6,
    rs_rw_texture = 1 << 7,
    rs_copy_src = 1 << 8,
    rs_copy_dst = 1 << 9,
    rs_cpu_read = 1 << 10,
    rs_cpu_write = 1 << 11,
    rs_render_target = 1 << 12,
    rs_present = 1 << 13,
    rs_depth_read = 1 << 14,
    rs_depth_write = 1 << 15,
    // rt_acce_structure,
    // shading_rate_src
};

using gal_resource_states = u32;

enum gal_memory_flag {
    undefined = 0,
    gpu_dedicated = 1 << 0, // gpu dedicated memory
    cpu_upload = 1 << 1,    // cpu upload memory
    gpu_download = 1 << 2,  // gpu download
};

using gal_memory_flags = u32;

enum gal_buffer_flag {
    /// Default flag (Buffer will use aliased memory, buffer will not be cpu accessible until mapBuffer is called)
    bcf_none = 0,
    /// Buffer will allocate its own memory (COMMITTED resource)
    bcf_own_memory = 1 << 0,
    /// Buffer will be persistently mapped
    bcf_persistent_map = 1 << 1,
    /// Use ESRAM to store this buffer
    bcf_esram = 1 << 2,
    /// Flag to specify not to allocate descriptors for the resource
    bcf_no_descriptor_view_creation = 1 << 3
};

using gal_buffer_flags = u32;

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

struct BlendStateDesc {
    /// Source blend factor per render target.
    BlendConstant mSrcFactors[MAX_RENDER_TARGET_ATTACHMENTS];
    /// Destination blend factor per render target.
    BlendConstant mDstFactors[MAX_RENDER_TARGET_ATTACHMENTS];
    /// Source alpha blend factor per render target.
    BlendConstant mSrcAlphaFactors[MAX_RENDER_TARGET_ATTACHMENTS];
    /// Destination alpha blend factor per render target.
    BlendConstant mDstAlphaFactors[MAX_RENDER_TARGET_ATTACHMENTS];
    /// Blend mode per render target.
    BlendMode mBlendModes[MAX_RENDER_TARGET_ATTACHMENTS];
    /// Alpha blend mode per render target.
    BlendMode mBlendAlphaModes[MAX_RENDER_TARGET_ATTACHMENTS];
    /// Write mask per render target.
    int32_t mMasks[MAX_RENDER_TARGET_ATTACHMENTS];
    /// Mask that identifies the render targets affected by the blend state.
    BlendStateTargets mRenderTargetMask;
    /// Set whether alpha to coverage should be enabled.
    bool mAlphaToCoverage;
    /// Set whether each render target has an unique blend function. When false the blend function in slot 0 will be used for all render targets.
    bool mIndependentBlend;
};

struct DepthStateDesc {
    bool mDepthTest;
    bool mDepthWrite;
    gal_compare_mode mDepthFunc;
    bool mStencilTest;
    uint8_t mStencilReadMask;
    uint8_t mStencilWriteMask;
    gal_compare_mode mStencilFrontFunc;
    StencilOp mStencilFrontFail;
    StencilOp mDepthFrontFail;
    StencilOp mStencilFrontPass;
    gal_compare_mode mStencilBackFunc;
    StencilOp mStencilBackFail;
    StencilOp mDepthBackFail;
    StencilOp mStencilBackPass;
};

struct RasterizerStateDesc {
    gal_cull_mode mCullMode;
    int32_t mDepthBias;
    float mSlopeScaledDepthBias;
    FillMode mFillMode;
    gal_front_face mFrontFace;
    bool mMultiSample;
    bool mScissor;
    bool mDepthClampEnable;
};

struct VertexAttrib {
    ShaderSemantic mSemantic;
    uint32_t mSemanticNameLength;
    char mSemanticName[MAX_SEMANTIC_NAME_LENGTH];
    gal_texture_format mFormat;
    uint32_t mBinding;
    uint32_t mLocation;
    uint32_t mOffset;
    VertexAttribRate mRate;
};

struct VertexLayout {
    uint32_t mAttribCount;
    VertexAttrib mAttribs[MAX_VERTEX_ATTRIBS];
    uint32_t mStrides[MAX_VERTEX_BINDINGS];
};

struct ReadRange {
    uint64_t mOffset;
    uint64_t mSize;
};

DECLARE_GAL_HANDLE(gal_context) {
  protected:
    gal_desc m_gal_desc;
};
DECLARE_GAL_HANDLE(gal_buffer) {
  protected:
    gal_resource_states m_resource_states;
    gal_memory_flags m_memory_flags;
    u64 m_size;
};
DECLARE_GAL_HANDLE(gal_texture){};

DECLARE_GAL_HANDLE(gal_sampler){};
DECLARE_GAL_HANDLE(gal_rendertarget){};
DECLARE_GAL_HANDLE(gal_fence){};
DECLARE_GAL_HANDLE(gal_semaphore){};
DECLARE_GAL_HANDLE(gal_swapchain){};
DECLARE_GAL_HANDLE(gal_shader){};
DECLARE_GAL_HANDLE(gal_rootsignature){};
DECLARE_GAL_HANDLE(gal_pipeline){};
DECLARE_GAL_HANDLE(gal_pipelinecache){};
DECLARE_GAL_HANDLE(gal_commandlist){};
DECLARE_GAL_HANDLE(gal_descriptorpool){};
using gal_texture_flags = u32;

struct gal_buffer_desc {
    u64 size;                          // size
    gal_resource_types resource_types; // descriptor types of buffer
    gal_resource_states initial_state; // initial state of buffer
    gal_memory_flags memory_flags;
    gal_buffer_flags flags;
};
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
    gal_rendertarget *pRenderTarget;
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

struct gal_texture_desc {
    u32 width;
    u32 height;
    u32 depth = 1;
    u32 array_size = 1;
    u32 mip_level = 1;
    gal_memory_flags memory_flags;
    gal_texture_dimension dimension;
    gal_texture_sample_count sample_count;
    gal_texture_format format;
    gal_texture_flag texture_flags;
    gal_resource_types resource_types; // descriptor types of buffer
    gal_resource_states initial_state; // initial state of buffer
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

struct gal_rendertarget_desc {};

struct swapchain_desc {};

struct shader_desc {};

struct gal_pipelinecache_desc {
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

struct gal_compute_pipeline_desc {
    gal_shader *shader;
    gal_pipelinecache *pipeline_cache;
    gal_rootsignature *root_signature;
};
struct gal_raytracing_pipeline_desc {};

struct gal_graphics_pipeline_desc {
    gal_shader *pShaderProgram;
    gal_rootsignature *pRootSignature;
    VertexLayout *pVertexLayout;
    BlendStateDesc *pBlendState;
    DepthStateDesc *pDepthState;
    RasterizerStateDesc *pRasterizerState;
    gal_texture_format *pColorFormats;
    uint32_t mRenderTargetCount;
    gal_texture_sample_count mSampleCount;
    uint32_t mSampleQuality;
    gal_texture_format mDepthStencilFormat;
    PrimitiveTopology mPrimitiveTopo;
    bool mSupportIndirectCommandBuffer;
    bool mVRFoveatedRendering;
};

struct gal_commandpool_desc {};

// struct to consume descriptorset

struct gal_descriptorset_desc {};

struct gal_rootsignature_desc {};

struct gal_fence_desc {};

struct gal_semaphore_desc {};
struct gal_command_pool_desc {};

// _desc to allocate commandlist
struct gal_commandlist_desc {};

struct gal_renderpass_begin_desc {};
} // namespace ant::gal