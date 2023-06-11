#pragma once

#include <filesystem>

namespace ant {


enum class ShaderOptimizationLevel { NONE, O0, O1, O2, O3 };

enum class ShaderTargetProfile {
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

enum class ShaderModuleVersion { SM_6_0, SM_6_1, SM_6_2, SM_6_3, SM_6_4, SM_6_5, SM_6_6, SM_6_7 };

constexpr wchar_t *ToDxcTargetProfile(ShaderTargetProfile tp) {
    switch (tp) {
    case ShaderTargetProfile::VS_6_0:
        return L"vs_6_0";
    case ShaderTargetProfile::PS_6_0:
        return L"ps_6_0";
    case ShaderTargetProfile::GS_6_0:
        return L"gs_6_0";
    case ShaderTargetProfile::HS_6_0:
        return L"hs_6_0";
    case ShaderTargetProfile::DS_6_0:
        return L"ds_6_0";
    case ShaderTargetProfile::CS_6_0:
        return L"cs_6_0";
    case ShaderTargetProfile::MS_6_0:
        return L"ms_6_0";
    case ShaderTargetProfile::VS_6_1:
        return L"vs_6_1";
    case ShaderTargetProfile::PS_6_1:
        return L"ps_6_1";
    case ShaderTargetProfile::GS_6_1:
        return L"gs_6_1";
    case ShaderTargetProfile::HS_6_1:
        return L"hs_6_1";
    case ShaderTargetProfile::DS_6_1:
        return L"ds_6_1";
    case ShaderTargetProfile::CS_6_1:
        return L"cs_6_1";
    case ShaderTargetProfile::MS_6_1:
        return L"ms_6_1";
    case ShaderTargetProfile::VS_6_2:
        return L"vs_6_2";
    case ShaderTargetProfile::PS_6_2:
        return L"ps_6_2";
    case ShaderTargetProfile::GS_6_2:
        return L"gs_6_2";
    case ShaderTargetProfile::HS_6_2:
        return L"hs_6_2";
    case ShaderTargetProfile::DS_6_2:
        return L"ds_6_2";
    case ShaderTargetProfile::CS_6_2:
        return L"cs_6_2";
    case ShaderTargetProfile::MS_6_2:
        return L"ms_6_2";
    case ShaderTargetProfile::VS_6_3:
        return L"vs_6_3";
    case ShaderTargetProfile::PS_6_3:
        return L"ps_6_3";
    case ShaderTargetProfile::GS_6_3:
        return L"gs_6_3";
    case ShaderTargetProfile::HS_6_3:
        return L"hs_6_3";
    case ShaderTargetProfile::DS_6_3:
        return L"ds_6_3";
    case ShaderTargetProfile::CS_6_3:
        return L"cs_6_3";
    case ShaderTargetProfile::MS_6_3:
        return L"ms_6_3";
    case ShaderTargetProfile::VS_6_4:
        return L"vs_6_4";
    case ShaderTargetProfile::PS_6_4:
        return L"ps_6_4";
    case ShaderTargetProfile::GS_6_4:
        return L"gs_6_4";
    case ShaderTargetProfile::HS_6_4:
        return L"hs_6_4";
    case ShaderTargetProfile::DS_6_4:
        return L"ds_6_4";
    case ShaderTargetProfile::CS_6_4:
        return L"cs_6_4";
    case ShaderTargetProfile::MS_6_4:
        return L"ms_6_4";
    case ShaderTargetProfile::VS_6_5:
        return L"vs_6_5";
    case ShaderTargetProfile::PS_6_5:
        return L"ps_6_5";
    case ShaderTargetProfile::GS_6_5:
        return L"gs_6_5";
    case ShaderTargetProfile::HS_6_5:
        return L"hs_6_5";
    case ShaderTargetProfile::DS_6_5:
        return L"ds_6_5";
    case ShaderTargetProfile::CS_6_5:
        return L"cs_6_5";
    case ShaderTargetProfile::MS_6_5:
        return L"ms_6_5";
    case ShaderTargetProfile::VS_6_6:
        return L"vs_6_6";
    case ShaderTargetProfile::PS_6_6:
        return L"ps_6_6";
    case ShaderTargetProfile::GS_6_6:
        return L"gs_6_6";
    case ShaderTargetProfile::HS_6_6:
        return L"hs_6_6";
    case ShaderTargetProfile::DS_6_6:
        return L"ds_6_6";
    case ShaderTargetProfile::CS_6_6:
        return L"cs_6_6";
    case ShaderTargetProfile::MS_6_6:
        return L"ms_6_6";
    case ShaderTargetProfile::VS_6_7:
        return L"vs_6_7";
    case ShaderTargetProfile::PS_6_7:
        return L"ps_6_7";
    case ShaderTargetProfile::GS_6_7:
        return L"gs_6_7";
    case ShaderTargetProfile::HS_6_7:
        return L"hs_6_7";
    case ShaderTargetProfile::DS_6_7:
        return L"ds_6_7";
    case ShaderTargetProfile::CS_6_7:
        return L"cs_6_7";
    case ShaderTargetProfile::MS_6_7:
        return L"ms_6_7";
    default:
        return L""; // error
    }
}

enum class ShaderBlobType { SPIRV, DXIL };

// TODO(hylu): some setting might be shader dependent
//struct ShaderCompilationSettings {
//    ShaderModuleVersion sm_version{ShaderModuleVersion::SM_6_6};
//    ShaderOptimizationLevel optimization_level;
//    ShaderBlobType target_api;
//    std::filesystem::path input_dir, output_dir;
//    ant::vector<std::filesystem::path> shader_list;
//    bool force_recompile{false};
//};

// compile desc of a single shader stage
struct ShaderCompileDesc {
    //std::filesystem::path filename;
    //std::filesystem::path output_filename;
    const char* entry;
    ShaderTargetProfile target_profile;
    ShaderOptimizationLevel optimization_level;
    ShaderBlobType target_api;
    ant::vector<std::filesystem::path> include_search_path;
    ant::vector<std::filesystem::path> defines;
};

inline constexpr ant::fixed_array<char, 4> hsb_header{'h', 's', 'b', '1'};


struct ShaderBinaryHeader {
    fixed_array<char, 4> header;
    u32 shader_blob_offset;
    u32 shader_blob_size;
    //u32 pdb_offset;
};

struct ShaderSourceBlob {
    void *data;
    u64 size;
};

struct CompiledShader {
    void Release();
    ShaderBlobType type;
    // IDxcBlob *
    void *byte_code = nullptr;
    void *pdb = nullptr;
    void *hash = nullptr;
    void *reflection = nullptr;
    const char *entry; // can be reflected using spirv_cross
};




} // namespace ant
