#pragma once

// standard libraries
#include <filesystem>

// third party libraries
#ifdef _WIN32
#include <dxcompiler/d3d12shader.h>
#else 
#include <dxcompiler/WinAdapter.h>
#endif
#include <dxcompiler/dxcapi.h>
// project headers
#include "framework/gal/enum.h"
#include "framework/utils/memory/container.h"
#include "framework/utils/singleton/public_singleton.h"
#include "framework/utils/types/types.h"
#include "framework/utils/blob/blob.h"

#include "framework/utils/helper.h"
// enums
// antumbra shader compiler
namespace ante::gal {

enum class shader_optimization_level { NONE, O0, O1, O2, O3 };

enum class shader_target_profile {
    INVALID,

    VS_6_0,
    VS_6_1,
    VS_6_2,
    VS_6_3,
    VS_6_4,
    VS_6_5,
    VS_6_6,
    VS_6_7,

    PS_6_0,
    PS_6_1,
    PS_6_2,
    PS_6_3,
    PS_6_4,
    PS_6_5,
    PS_6_6,
    PS_6_7,

    CS_6_0,
    CS_6_1,
    CS_6_2,
    CS_6_3,
    CS_6_4,
    CS_6_5,
    CS_6_6,
    CS_6_7,

    GS_6_0,
    GS_6_1,
    GS_6_2,
    GS_6_3,
    GS_6_4,
    GS_6_5,
    GS_6_6,
    GS_6_7,

    // hull, tese
    HS_6_0,
    HS_6_1,
    HS_6_2,
    HS_6_3,
    HS_6_4,
    HS_6_5,
    HS_6_6,
    HS_6_7,

    // domain, tesc
    DS_6_0,
    DS_6_1,
    DS_6_2,
    DS_6_3,
    DS_6_4,
    DS_6_5,
    DS_6_6,
    DS_6_7,

    // mesh shader
    MS_6_0,
    MS_6_1,
    MS_6_2,
    MS_6_3,
    MS_6_4,
    MS_6_5,
    MS_6_6,
    MS_6_7,
};

//enum class shader_hlsl_version { SM_6_0, SM_6_1, SM_6_2, SM_6_3, SM_6_4, SM_6_5, SM_6_6, SM_6_7 };

constexpr const wchar_t *utils_to_hlsl_target_profile(shader_target_profile tp) {
    switch (tp) {
    case shader_target_profile::VS_6_0:
        return L"vs_6_0";
    case shader_target_profile::PS_6_0:
        return L"ps_6_0";
    case shader_target_profile::GS_6_0:
        return L"gs_6_0";
    case shader_target_profile::HS_6_0:
        return L"hs_6_0";
    case shader_target_profile::DS_6_0:
        return L"ds_6_0";
    case shader_target_profile::CS_6_0:
        return L"cs_6_0";
    case shader_target_profile::MS_6_0:
        return L"ms_6_0";
    case shader_target_profile::VS_6_1:
        return L"vs_6_1";
    case shader_target_profile::PS_6_1:
        return L"ps_6_1";
    case shader_target_profile::GS_6_1:
        return L"gs_6_1";
    case shader_target_profile::HS_6_1:
        return L"hs_6_1";
    case shader_target_profile::DS_6_1:
        return L"ds_6_1";
    case shader_target_profile::CS_6_1:
        return L"cs_6_1";
    case shader_target_profile::MS_6_1:
        return L"ms_6_1";
    case shader_target_profile::VS_6_2:
        return L"vs_6_2";
    case shader_target_profile::PS_6_2:
        return L"ps_6_2";
    case shader_target_profile::GS_6_2:
        return L"gs_6_2";
    case shader_target_profile::HS_6_2:
        return L"hs_6_2";
    case shader_target_profile::DS_6_2:
        return L"ds_6_2";
    case shader_target_profile::CS_6_2:
        return L"cs_6_2";
    case shader_target_profile::MS_6_2:
        return L"ms_6_2";
    case shader_target_profile::VS_6_3:
        return L"vs_6_3";
    case shader_target_profile::PS_6_3:
        return L"ps_6_3";
    case shader_target_profile::GS_6_3:
        return L"gs_6_3";
    case shader_target_profile::HS_6_3:
        return L"hs_6_3";
    case shader_target_profile::DS_6_3:
        return L"ds_6_3";
    case shader_target_profile::CS_6_3:
        return L"cs_6_3";
    case shader_target_profile::MS_6_3:
        return L"ms_6_3";
    case shader_target_profile::VS_6_4:
        return L"vs_6_4";
    case shader_target_profile::PS_6_4:
        return L"ps_6_4";
    case shader_target_profile::GS_6_4:
        return L"gs_6_4";
    case shader_target_profile::HS_6_4:
        return L"hs_6_4";
    case shader_target_profile::DS_6_4:
        return L"ds_6_4";
    case shader_target_profile::CS_6_4:
        return L"cs_6_4";
    case shader_target_profile::MS_6_4:
        return L"ms_6_4";
    case shader_target_profile::VS_6_5:
        return L"vs_6_5";
    case shader_target_profile::PS_6_5:
        return L"ps_6_5";
    case shader_target_profile::GS_6_5:
        return L"gs_6_5";
    case shader_target_profile::HS_6_5:
        return L"hs_6_5";
    case shader_target_profile::DS_6_5:
        return L"ds_6_5";
    case shader_target_profile::CS_6_5:
        return L"cs_6_5";
    case shader_target_profile::MS_6_5:
        return L"ms_6_5";
    case shader_target_profile::VS_6_6:
        return L"vs_6_6";
    case shader_target_profile::PS_6_6:
        return L"ps_6_6";
    case shader_target_profile::GS_6_6:
        return L"gs_6_6";
    case shader_target_profile::HS_6_6:
        return L"hs_6_6";
    case shader_target_profile::DS_6_6:
        return L"ds_6_6";
    case shader_target_profile::CS_6_6:
        return L"cs_6_6";
    case shader_target_profile::MS_6_6:
        return L"ms_6_6";
    case shader_target_profile::VS_6_7:
        return L"vs_6_7";
    case shader_target_profile::PS_6_7:
        return L"ps_6_7";
    case shader_target_profile::GS_6_7:
        return L"gs_6_7";
    case shader_target_profile::HS_6_7:
        return L"hs_6_7";
    case shader_target_profile::DS_6_7:
        return L"ds_6_7";
    case shader_target_profile::CS_6_7:
        return L"cs_6_7";
    case shader_target_profile::MS_6_7:
        return L"ms_6_7";
    default:
        return L""; // error
    }
}

enum class shader_blob_type { SPIRV, DXIL };

// TODO(hylu): some setting might be shader dependent
//struct ShaderCompilationSettings {
//    ShaderModuleVersion sm_version{ShaderModuleVersion::SM_6_6};
//    ShaderOptimizationLevel optimization_level;
//    shader_blob_type target_api;
//    std::filesystem::path input_dir, output_dir;
//    ante::vector<std::filesystem::path> shader_list;
//    bool force_recompile{false};
//};

// compile desc of a single shader stage
struct shader_compile_desc {
    //std::filesystem::path filename;
    //std::filesystem::path output_filename;
    const char *entry;
    shader_target_profile target_profile;
    shader_optimization_level optimization_level;
    shader_blob_type target_api;
    //bool b_need_reflection;
    ante::vector<std::filesystem::path> include_search_path;
    ante::vector<std::filesystem::path> defines;
};

//inline constexpr ante::fixed_array<char, 4> hsb_header{'h', 's', 'b', '1'};

//struct ShaderBinaryHeader {
//    fixed_array<char, 4> header;
//    u32 shader_blob_offset;
//    u32 shader_blob_size;
//    //u32 pdb_offset;
//};

struct shader_source_blob : blob {};

struct VertexInput {
    // resource name
    const char *name;

    // The size of the attribute
    u32 size;

    // name size
    u32 name_size;
};

enum class ShaderResourceType { UNDEFINED, INPUT, OUTPUT, PUSH_CONSTANT, RESOURCE };

struct ShaderResource {
    // resource Type
    gal_descriptor_type descriptor_type = gal_descriptor_type::UNDEFINED;
    ShaderResourceType resource_type = ShaderResourceType::UNDEFINED;

    // The resource set for binding frequency

    u32 set = UINT_MAX;

    // spirv
    u32 vec_size = UINT_MAX;
    u32 array_size = 1;
    u32 columns = UINT_MAX;
    //u32 binding = UINT_MAX;
    // The resource binding location
    u32 reg = UINT_MAX;

    // The size of the resource. This will be the DescriptorInfo array size for textures
    u32 size = 0;

    // what stages use this resource
    gal_shader_stage used_stages = gal_shader_stage::UNDEFINED;

    // resource name
    // const char *name;
    ante::str name;

    // name size
    //u32 name_size;

    // 1D / 2D / Array / MSAA / ...
    gal_texture_dimension dim = gal_texture_dimension::UNDEFINED;
};

// var in cbuffer
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

struct shader_reflection {
    // single large allocation for names to reduce number of allocations
    char *pNamePool;
    VertexInput *pVertexInputs;
    ante::hash_map<ante::str, ShaderResource> resources;
    ante::vector<u32> sets;
    ShaderVariable *pVariables;

    const char *entry;
    gal_shader_stage mShaderStage;

    u32 mNamePoolSize;
    u32 mVertexInputsCount;
    u32 mShaderResourceCount;
    u32 mVariableCount;

    // Thread group size for compute shader
    ante::fixed_array<u32, 3> work_group_size;

    //number of tessellation control point
    u32 tesc_control_point;
};

struct pipeline_reflection {
    ante::vector<ShaderResource> m_resources;
    ante::vector<u32> sets; // 16bit set count, 16 bit binding count;
    //ante::vector<std::pair<const char*, ShaderResource>> m_resources;
    //gal_shader_stage mShaderStages;
    // the individual stages reflection data.
    //shader_reflection mStageReflections[MAX_SHADER_STAGE_COUNT];
    //u32 mStageReflectionCount;

    //u32 mVertexStageIndex;
    //u32 mHullStageIndex;
    //u32 mDomainStageIndex;
    //u32 mGeometryStageIndex;
    //u32 mPixelStageIndex;

    //ShaderResource *pShaderResources;
    //u32 mShaderResourceCount;

    //ShaderVariable *pVariables;
    //u32 mVariableCount;
};

struct pipeline_reflection_vk : public pipeline_reflection {};

class shader_compiler;

struct compiled_shader {
    friend class shader_compiler;

  public:
    void release();
    const blob *byte_code() const;
    const blob *pdb() const;
    const blob *hash() const;
    const blob *dxc_reflection() const;
    const shader_reflection *reflection() const;
    const char *entry();

  private:
    void create_shader_reflection();
    void create_shader_reflection_from_spirv();

    shader_blob_type m_type;
    // IDxcBlob *
    blob m_byte_code;
    void *m_p_byte_code;
    blob m_pdb;
    void *m_p_pdb;
    blob m_hash;
    void *m_p_hash;
    blob m_dxc_reflection;
    void *m_p_dxc_reflection;
    const char *m_entry; // can be reflected using spirv_cross
    shader_reflection *m_reflection;
};

struct compiled_shader_gourp_desc {
    compiled_shader *vert;
    compiled_shader *frag;
    compiled_shader *geom;
    compiled_shader *hull;
    compiled_shader *domain;
    compiled_shader *comp;
};
struct shader_gourp_source_desc {
    shader_compile_desc *desc_vert;
    shader_compile_desc *desc_frag;
    shader_compile_desc *desc_geom;
    shader_compile_desc *desc_domin;
    shader_compile_desc *desc_hull;
    shader_compile_desc *desc_comp;
};

// FIXME(hyl5): I think oop interface is not consistent with the gal degisn
// TODO(hyl5): serializer/deserialzer for compiled_shader_group, the dxil/spirv/reflection shouldn't be generated in runtime.
struct compiled_shader_group {
  public:
    // set from compiled shaders
    void set(compiled_shader_gourp_desc *desc);
    // set a shader group from source code and compile descs
    void set_from_source(shader_source_blob *source, shader_gourp_source_desc *descs);
    void release();

    compiled_shader *vert();
    compiled_shader *frag();
    compiled_shader *hull();
    compiled_shader *geom();
    compiled_shader *domain();
    compiled_shader *comp();
    gal_shader_stage stages();
    pipeline_reflection *reflection();

  private:
    void create_pipeline_reflection();

    // the shader group is created from one shader
    bool m_b_same_root_signature = false;
    gal_shader_stage m_stage_flags = gal_shader_stage::UNDEFINED;
    compiled_shader *m_vert = nullptr;
    compiled_shader *m_frag = nullptr;
    compiled_shader *m_geom = nullptr;
    compiled_shader *m_hull = nullptr;
    compiled_shader *m_domain = nullptr;
    compiled_shader *m_comp = nullptr;
    pipeline_reflection *m_pipeline_reflection = nullptr;
};

class shader_compiler : public Singleton<shader_compiler> {
  public:
    shader_compiler() noexcept;
    ~shader_compiler() noexcept override;

    DELETE_COPY_MOVE(shader_compiler)

    compiled_shader *compile(shader_source_blob *blob, shader_compile_desc *desc);

  private:
    IDxcUtils *m_idxc_utils;
    IDxcCompiler3 *m_idxc_compiler;
    IDxcIncludeHandler *m_idxc_include_handler;
};

} // namespace ante::gal