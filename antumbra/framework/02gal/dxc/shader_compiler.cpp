#include "shader_compiler.h"

#include "framework/01core/io/file_system.h"
#include "framework/01core/logging/log.h"
#include "framework/01core/memory/memory.h"
#include <regex>

//namespace std {
//template <typename X, typename Y> struct std::hash<std::pair<X, Y>> {
//    std::size_t operator()(const std::pair<X, Y> &pair) const {
//        return std::hash<X>()(pair.first) ^ std::hash<Y>()(pair.second);
//    }
//};
//} // namespace std

namespace ant {

//struct ShaderCompilationSetting {
//    const std::filesystem::path path; // key to index the shader_text map
//    ShaderCompileDesc args;       // args
//};
//
//struct FileNode {
//    bool need_compile;
//    ant::vector<std::filesystem::path> header_files;
//};
//
//static std::filesystem::path cached_project_dir = std::filesystem::temp_directory_path() / "horizon";
//static std::filesystem::path cached_shader_dir = cached_project_dir / "shader";

ShaderCompiler::ShaderCompiler() noexcept {
    HRESULT hr;
    hr = (DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&idxc_compiler)));
    if (FAILED(hr)) {
        LOG_ERROR("failed to create idxc compiler");
        return;
    }
    hr = (DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&idxc_utils)));
    if (FAILED(hr)) {
        LOG_ERROR("failed to create idxc utils");
        return;
    }
    hr = (idxc_utils->CreateDefaultIncludeHandler((&idxc_include_handler)));
    if (FAILED(hr)) {
        LOG_ERROR("failed to create idxc include handler");
        return;
    }
}

ShaderCompiler::~ShaderCompiler() noexcept {}

ShaderTargetProfile GetShaderTargetProfile(const char *str, ShaderModuleVersion version) {
    constexpr ShaderModuleVersion base_version = ShaderModuleVersion::SM_6_0;
    ShaderTargetProfile base_profile;
    if (std::strcmp(str, "VS_MAIN") == 0) {
        base_profile = ShaderTargetProfile::VS_6_0;
    } else if (std::strcmp(str, "PS_MAIN") == 0) {
        base_profile = ShaderTargetProfile::PS_6_0;
    } else if (std::strcmp(str, "CS_MAIN") == 0) {
        base_profile = ShaderTargetProfile::CS_6_0;
    } else if (std::strcmp(str, "GS_MAIN") == 0) {
        base_profile = ShaderTargetProfile::GS_6_0;
    } else if (std::strcmp(str, "GS_MAIN") == 0) {
        base_profile = ShaderTargetProfile::HS_6_0;
    } else if (std::strcmp(str, "DS_MAIN") == 0) {
        base_profile = ShaderTargetProfile::DS_6_0;
    } else if (std::strcmp(str, "MS_MAIN") == 0) {
        base_profile = ShaderTargetProfile::MS_6_0;
    } else {
        LOG_ERROR("invalid shader type");
        return ShaderTargetProfile::INVALID;
    }

    u32 version_offset = static_cast<u32>(version) - static_cast<u32>(base_version);
    u32 target_version = static_cast<u32>(base_profile) + version_offset;
    return static_cast<ShaderTargetProfile>(target_version);
}

//// we read once to build dependency graph and cache
//void IterateHeaderFiles(const std::filesystem::path &path,
//                        ant::hash_map<std::filesystem::path, FileNode> &dependency_map) {
//    FileNode current_node;
//    auto text = io::read_text_file(ant::str{path.string()});
//    // we use last write time instead of md5, but that might not accurate
//    //auto md5_value = md5(text);
//    std::filesystem::file_time_type last_mod_time = std::filesystem::last_write_time(path);
//    auto s_last_mod_time = ant::str{std::to_string(to_time_t(last_mod_time))};
//    auto cached_path = cached_shader_dir / path.filename(); // FIXME(hylu): filename may conflict
//    if (std::filesystem::exists(cached_path) && (io::read_text_file(cached_path) == s_last_mod_time)) {
//        current_node.need_compile = false;
//    } else {
//        io::write_text_file(ant::str{cached_path.string()}, s_last_mod_time.data(),
//                            s_last_mod_time.size() * sizeof(char));
//    }
//
//    // the following code is generated from ChatGPT.
//    std::regex reg{"#include\\s+[\"<](.*)[\">]"};
//    std::smatch match;
//    // Search for all #include directives in the file
//    std::string::const_iterator search_start(text.cbegin());
//    while (std::regex_search(search_start, text.cend(), match, reg)) {
//        std::filesystem::path abs_include_path = path.parent_path() / match[1].str();
//        // Move the search start to the end of the matched string
//        search_start = match.suffix().first;
//        IterateHeaderFiles(std::filesystem::absolute(abs_include_path), dependency_map);
//        current_node.header_files.push_back(std::filesystem::absolute(abs_include_path));
//    }
//    dependency_map.emplace(path, std::move(current_node));
//}
//
//bool NeedCompile(const std::filesystem::path &current_node_path,
//                 ant::hash_set<std::filesystem::path> &visited_vertices,
//                ant::hash_map<std::filesystem::path, FileNode> &node_map) {
//
//    auto &current_node = node_map[current_node_path];
//    if (visited_vertices.find(current_node_path) != visited_vertices.end()) {
//        return current_node.need_compile;
//    }
//    for (auto &header : node_map[current_node_path].header_files) {
//        current_node.need_compile |= NeedCompile(header, visited_vertices, node_map);
//    }
//    visited_vertices.emplace(current_node_path);
//    return current_node.need_compile;
//}
//
//void ShaderCompiler::CompileShaders(const ShaderCompilationSettings &settings) {
//    // create cached directory
//    if (!std::filesystem::exists(cached_project_dir)) {
//        std::filesystem::create_directory(cached_project_dir);
//    }
//    if (!std::filesystem::exists(cached_shader_dir)) {
//        std::filesystem::create_directory(cached_shader_dir);
//    }
//
//    // shader text map, value contains shader blob and cache stat.
//    Container::HashMap<std::filesystem::path, std::pair<ant::str, bool>> shader_texts(
//        settings.shader_list.size());
//
//    for (auto &path : settings.shader_list) {
//        auto txt = io::read_text_file(path.string().c_str());
//        if (txt.empty()) {
//            continue;
//        }
//        shader_texts.emplace(std::filesystem::absolute(path),
//                             std::make_pair(txt,
//                                            true)); // TODO(hylu): handle file reading error
//    }
//
//    // extract args from shader text for compiling
//    Container::Array<ShaderCompilationSetting> shader_compilation_settings;
//    for (auto &[path, text] : shader_texts) {
//        std::regex entry{"[VPCHDM]S_MAIN"}; // vs, ps, cs, hs, ds, ms, TODO(hylu): rt
//        std::sregex_iterator pos(text.first.cbegin(), text.first.cend(), entry);
//
//        for (std::sregex_iterator end; pos != end; ++pos) {
//            ShaderCompilationSetting scs{path.string()}; // shader text ref
//            scs.args.entry_point = pos->str();
//            scs.args.optimization_level = settings.optimization_level;
//            scs.args.target_api = settings.target_api;
//            scs.args.include_path = settings.input_dir / "include";
//            ant::str output_file_name = ant::str{path.filename().string()} + "." +
//                                                 ant::str{pos->str().substr(0, 2)} + ".hsb"; // add api
//            scs.args.out_file_path = settings.output_dir / output_file_name;
//            scs.args.target_profile = GetShaderTargetProfile(pos->str().c_str(), settings.sm_version);
//            shader_compilation_settings.push_back(std::move(scs));
//        }
//    }
//
//    Container::HashMap<std::filesystem::path, FileNode> shader_node_map;
//    // build shader dependency graph
//    // TODO(hylu): threading
//    for (auto &[path, text] : shader_texts) {
//        IterateHeaderFiles(path, shader_node_map);
//    }
//
//    // iterate depedency graph
//    // TODO(hylu): threading
//    Container::HashSet<std::filesystem::path> visited_vertices{};
//    for (auto shader : shader_compilation_settings) {
//        shader_texts[shader.path].second = NeedCompile(shader.path, visited_vertices, shader_node_map);
//    }
//
//    //// multithread shader compiling
//    //tbb::parallel_for(tbb::blocked_range<u32>(0, static_cast<u32>(shader_compilation_settings.size())),
//    //                  [&shader_compilation_settings, &shader_texts, &settings](const tbb::blocked_range<u32> &r) {
//    //                      for (u32 v = r.begin(); v < r.end(); v++) {
//    //                          auto &shader = shader_compilation_settings[v];
//    //                          auto &[shader_text, need_compile] = shader_texts[shader.path];
//    //                          if (settings.force_recompile == true || need_compile) {
//    //                              LOG_INFO("compiling {}, {}", shader.path.string(), need_compile);
//    //                              ShaderCompiler::Compile(shader_text, shader.args);
//    //                          }
//    //                      }
//    //                  });
//
//    // single thread version
//    for (auto &shader : shader_list) {
//        if (!shader.need_compile) {
//            continue;
//        }
//
//        ShaderCompiler::Compile(shader_texts[shader.path], shader.args);
//    }
//}

CompiledShader *ShaderCompiler::Compile(const ShaderSourceBlob &blob, const ShaderCompileDesc &desc) {
    IDxcBlobEncoding *hlsl_blob;

    bool b_spv = desc.target_api == ShaderTargetAPI::SPIRV ? true : false;

    HRESULT hr = (idxc_utils->CreateBlob(blob.data, static_cast<u32>(blob.size), 0, &hlsl_blob));
    if (FAILED(hr)) {
        LOG_ERROR("failed to create blob");
        return nullptr;
    }
    auto stack_memory = ant::get_stack_memory_resource(512);
    ant::vector<LPCWSTR> args(&stack_memory);

    // entry point
    args.push_back(L"-E");
    args.push_back(ant::wstr(desc.entry_point.begin(), desc.entry_point.end(), &stack_memory).c_str());
    // target profile
    args.push_back(L"-T");
    const wchar_t *tp = ToDxcTargetProfile(desc.target_profile);
    args.push_back(tp);

    args.push_back(DXC_ARG_WARNINGS_ARE_ERRORS); // warning are errors
    args.push_back(DXC_ARG_ALL_RESOURCES_BOUND);

    for (auto &d : desc.defines) {
        args.push_back(L"-D");
        args.push_back(d.wstring().c_str());
    }

    for (auto &i : desc.include_search_path) {
        args.push_back(L"-I");
        args.push_back(i.wstring().c_str());
    }

    args.push_back(L"-HV 2021");

    if (true) {
        args.push_back(DXC_ARG_PACK_MATRIX_ROW_MAJOR);
    }

    switch (desc.optimization_level) {
    case ShaderOptimizationLevel::NONE:
        args.push_back(DXC_ARG_DEBUG);
        args.push_back(DXC_ARG_SKIP_OPTIMIZATIONS);
    case ShaderOptimizationLevel::O0:
        args.push_back(DXC_ARG_OPTIMIZATION_LEVEL0);
    case ShaderOptimizationLevel::O1:
        args.push_back(DXC_ARG_OPTIMIZATION_LEVEL1);
    case ShaderOptimizationLevel::O2:
        args.push_back(DXC_ARG_OPTIMIZATION_LEVEL2);
    case ShaderOptimizationLevel::O3:
        args.push_back(DXC_ARG_OPTIMIZATION_LEVEL3);
    default:
        break;
    }

    if (b_spv) {
        args.push_back(L"-spirv");
        args.push_back(L"-fspv-target-env=vulkan1.3");
        args.push_back(L"-fspv-reflect");
        args.push_back(L"-fvk-use-dx-layout");
        args.push_back(L"-fvk-use-dx-position-w");
    }

    DxcBuffer source_buffer;
    source_buffer.Ptr = hlsl_blob->GetBufferPointer();
    source_buffer.Size = hlsl_blob->GetBufferSize();
    source_buffer.Encoding = 0;

    IDxcResult *compile_result;
    if (FAILED(idxc_compiler->Compile(&source_buffer, args.data(), static_cast<u32>(args.size()), idxc_include_handler,
                                      IID_PPV_ARGS(&compile_result)))) {
        LOG_ERROR("{}", "Internal error or API misuse! Compile Failed");
        return nullptr;
    }

    IDxcBlob *byte_code = nullptr;
    IDxcBlob *pdb = nullptr;
    IDxcBlob *hash = nullptr;
    IDxcBlob *reflection = nullptr;

    HRESULT hrStatus;
    if (FAILED(compile_result->GetStatus(&hrStatus)) || FAILED(hrStatus)) {
        LOG_ERROR("error");
        return nullptr;
    }

    // Get compilation errors (if any).
    IDxcBlobUtf8 *errors = nullptr;

    if (SUCCEEDED(compile_result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr)) && errors != nullptr &&
        errors->GetStringLength() != 0) {
        LOG_ERROR("Warnings and Errors: {}", errors->GetStringPointer());
        return nullptr;
    }

    if (FAILED(compile_result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&byte_code), nullptr)) || byte_code == nullptr) {
        LOG_ERROR("error");
        return nullptr;
    }

    if (FAILED(compile_result->GetOutput(DXC_OUT_PDB, IID_PPV_ARGS(&pdb), nullptr)) || pdb == nullptr) {
        LOG_ERROR("error");
        return nullptr;
    }
    if (FAILED(compile_result->GetOutput(DXC_OUT_SHADER_HASH, IID_PPV_ARGS(&hash), nullptr))) {
        LOG_ERROR("error");
        return nullptr;
    }

    if (!b_spv && FAILED(compile_result->GetOutput(DXC_OUT_REFLECTION, IID_PPV_ARGS(&reflection), nullptr))) {
        LOG_ERROR("error");
        return nullptr;
    }

    CompiledShader *ret = ant::ant_alloc<CompiledShader>();

    ret->byte_code = byte_code;
    ret->reflection = reflection;
    ret->pdb = pdb;
    ret->hash = hash;

    return ret;
}

void ant::CompiledShader::Release() {
    reinterpret_cast<IDxcBlob *>(byte_code)->Release();
    reinterpret_cast<IDxcBlob *>(reflection)->Release();
    reinterpret_cast<IDxcBlob *>(pdb)->Release();
    reinterpret_cast<IDxcBlob *>(hash)->Release();
    byte_code = nullptr;
    reflection = nullptr;
    pdb = nullptr;
    hash = nullptr;
}

} // namespace ant
