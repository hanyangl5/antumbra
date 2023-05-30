//#include "shader_compiler.h"
//
//#include <regex>
//
//#include <boost/graph/adjacency_list.hpp>
//#include <boost/graph/breadth_first_search.hpp>
//#include <boost/graph/depth_first_search.hpp>
//#include <boost/graph/edge_list.hpp>
//
//#include <boost/algorithm/string.hpp>
//#include <boost/graph/degree_centrality.hpp>
//#include <boost/graph/directed_graph.hpp>
//#include <boost/graph/graph_utility.hpp>
//#include <boost/property_map/property_map.hpp>
//
//#include <tbb/blocked_range.h>
//#include <tbb/parallel_for.h>
//
//#include "runtime/core/encryption/md5.h"
//#include "runtime/core/io/file_system.h"
//#include "runtime/core/log/log.h"
//#include "runtime/core/memory/allocators.h"
//#include "runtime/core/platform/platform.h"
//#include "runtime/core/utils/functions.h"
//
//namespace std {
//template <typename X, typename Y> struct std::hash<std::pair<X, Y>> {
//    std::size_t operator()(const std::pair<X, Y> &pair) const {
//        return std::hash<X>()(pair.first) ^ std::hash<Y>()(pair.second);
//    }
//};
//} // namespace std
//
//using namespace boost;
//
//namespace Horizon {
//
//using Graph = adjacency_list<vecS, vecS, directedS>;
//
//struct ShaderCompilationSetting {
//    const std::filesystem::path path; // key to index the shader_text map
//    ShaderCompilationArgs args;       // args
//};
//
//struct FileNode {
//    bool need_compile;
//    Container::Array<std::filesystem::path> header_files;
//};
//
//static std::filesystem::path cached_project_dir = std::filesystem::temp_directory_path() / "horizon";
//static std::filesystem::path cached_shader_dir = cached_project_dir / "shader";
//
//ShaderCompiler::ShaderCompiler() noexcept {
//    CHECK_DX_RESULT(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&idxc_compiler)));
//    CHECK_DX_RESULT(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&idxc_utils)));
//    CHECK_DX_RESULT(idxc_utils->CreateDefaultIncludeHandler((&idxc_include_handler)));
//}
//
//ShaderCompiler::~ShaderCompiler() noexcept {}
//
//void ShaderCompiler::Compile(const Container::String &blob, const ShaderCompilationArgs &compile_args) {
//    ShaderCompiler::get().InternalCompile(blob, compile_args);
//}
//
//// TODO(hyl5): looking for better enum casting method
//// TODO(hylu): replace std::strcmp
//ShaderTargetProfile GetShaderTargetProfile(const char *str, ShaderModuleVersion version) {
//    constexpr ShaderModuleVersion base_version = ShaderModuleVersion::SM_6_0;
//    ShaderTargetProfile base_profile;
//    if (std::strcmp(str, "VS_MAIN") == 0) {
//        base_profile = ShaderTargetProfile::VS_6_0;
//    } else if (std::strcmp(str, "PS_MAIN") == 0) {
//        base_profile = ShaderTargetProfile::PS_6_0;
//    } else if (std::strcmp(str, "CS_MAIN") == 0) {
//        base_profile = ShaderTargetProfile::CS_6_0;
//    } else if (std::strcmp(str, "GS_MAIN") == 0) {
//        base_profile = ShaderTargetProfile::GS_6_0;
//    } else if (std::strcmp(str, "GS_MAIN") == 0) {
//        base_profile = ShaderTargetProfile::HS_6_0;
//    } else if (std::strcmp(str, "DS_MAIN") == 0) {
//        base_profile = ShaderTargetProfile::DS_6_0;
//    } else if (std::strcmp(str, "MS_MAIN") == 0) {
//        base_profile = ShaderTargetProfile::MS_6_0;
//    } else {
//        LOG_ERROR("invalid shader type");
//        return ShaderTargetProfile::INVALID;
//    }
//
//    u32 version_offset = static_cast<u32>(version) - static_cast<u32>(base_version);
//    u32 target_version = static_cast<u32>(base_profile) + version_offset;
//    return static_cast<ShaderTargetProfile>(target_version);
//}
//
//// we read once to build dependency graph and cache
//void IterateHeaderFiles(const std::filesystem::path &path,
//                        Container::HashMap<std::filesystem::path, FileNode> &dependency_map) {
//    FileNode current_node;
//    auto text = fs::read_text_file(Container::String{path.string()});
//    // we use last write time instead of md5, but that might not accurate
//    //auto md5_value = md5(text);
//    std::filesystem::file_time_type last_mod_time = std::filesystem::last_write_time(path);
//    auto s_last_mod_time = Container::String{std::to_string(to_time_t(last_mod_time))};
//    auto cached_path = cached_shader_dir / path.filename(); // FIXME(hylu): filename may conflict
//    if (std::filesystem::exists(cached_path) && (fs::read_text_file(cached_path) == s_last_mod_time)) {
//        current_node.need_compile = false;
//    } else {
//        fs::write_text_file(Container::String{cached_path.string()}, s_last_mod_time.data(),
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
//                 Container::HashSet<std::filesystem::path> &visited_vertices,
//                 Container::HashMap<std::filesystem::path, FileNode> &node_map) {
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
//    Container::HashMap<std::filesystem::path, std::pair<Container::String, bool>> shader_texts(
//        settings.shader_list.size());
//
//    for (auto &path : settings.shader_list) {
//        auto txt = fs::read_text_file(path.string().c_str());
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
//            Container::String output_file_name = Container::String{path.filename().string()} + "." +
//                                                 Container::String{pos->str().substr(0, 2)} + ".hsb"; // add api
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
//    // multithread shader compiling
//    tbb::parallel_for(tbb::blocked_range<u32>(0, static_cast<u32>(shader_compilation_settings.size())),
//                      [&shader_compilation_settings, &shader_texts, &settings](const tbb::blocked_range<u32> &r) {
//                          for (u32 v = r.begin(); v < r.end(); v++) {
//                              auto &shader = shader_compilation_settings[v];
//                              auto &[shader_text, need_compile] = shader_texts[shader.path];
//                              if (settings.force_recompile == true || need_compile) {
//                                  LOG_INFO("compiling {}, {}", shader.path.string(), need_compile);
//                                  ShaderCompiler::Compile(shader_text, shader.args);
//                              }
//                          }
//                      });
//
//    // single thread version
//    //for (auto &shader : shader_list) {
//    //    if (!shader.need_compile) {
//    //        continue;
//    //    }
//
//    //    ShaderCompiler::Compile(shader_texts[shader.path], shader.args);
//    //}
//}
//
//void ShaderCompiler::InternalCompile(const Container::String &hlsl_text, const ShaderCompilationArgs &compile_args) {
//    IDxcBlobEncoding *hlsl_blob{};
//    CHECK_DX_RESULT(idxc_utils->CreateBlob(hlsl_text.data(), static_cast<u32>(hlsl_text.size()), 0, &hlsl_blob));
//
//    auto stack_memory = Memory::GetStackMemoryResource(512);
//    Container::Array<LPCWSTR> compilation_arguments(&stack_memory);
//
//    // entry point
//    Container::WString ep(compile_args.entry_point.begin(), compile_args.entry_point.end(), &stack_memory);
//    compilation_arguments.push_back(L"-E");
//    compilation_arguments.push_back(ep.c_str());
//    // target profile
//    compilation_arguments.push_back(L"-T");
//    const wchar_t *tp = ToDxcTargetProfile(compile_args.target_profile);
//    compilation_arguments.push_back(tp);
//
//    compilation_arguments.push_back(DXC_ARG_WARNINGS_ARE_ERRORS); // warning are errors
//    compilation_arguments.push_back(DXC_ARG_ALL_RESOURCES_BOUND);
//    compilation_arguments.push_back(DXC_ARG_ALL_RESOURCES_BOUND);
//    //compilation_arguments.push_back(L"Fre");
//    //Container::WString ref_file =
//    //    Container::WString{compile_args.output_file_name.begin(), compile_args.output_file_name.end()} + L".ref";
//
//    //compilation_arguments.push_back(ref_file.c_str());
//
//    auto ws = compile_args.include_path.wstring();
//    Container::WString ip(ws.begin(), ws.end(), &stack_memory);
//
//    compilation_arguments.push_back(L"I");
//    compilation_arguments.push_back(ip.c_str());
//    compilation_arguments.push_back(L"-HV 2021");
//    
//    if (USE_ROW_MAJOR_MATRIX) {
//        compilation_arguments.push_back(DXC_ARG_PACK_MATRIX_ROW_MAJOR);
//    }
//
//    if (compile_args.optimization_level == ShaderOptimizationLevel::DEBUG) {
//        compilation_arguments.push_back(DXC_ARG_DEBUG);
//
//        //TODO(hylu): debug
//        //compilation_arguments.push_back(L"-Fo");
//        //Container::WString pdb_name = out_file_name / ".hlslpdb";
//        //compilation_arguments.push_back(pdb_name.c_str());
//    } else if (compile_args.optimization_level == ShaderOptimizationLevel::O3) {
//        compilation_arguments.push_back(DXC_ARG_OPTIMIZATION_LEVEL3);
//    }
//
//    if (compile_args.target_api == ShaderTargetAPI::SPIRV) {
//        compilation_arguments.push_back(L"-spirv");
//        compilation_arguments.push_back(L"-fspv-target-env=universal1.5");
//        //compilation_arguments.push_back(L"-fspv-reflect");
//    }
//
//    DxcBuffer source_buffer{hlsl_blob->GetBufferPointer(), hlsl_blob->GetBufferSize(), 0u};
//
//    IDxcResult *compile_result{};
//    if (FAILED(idxc_compiler->Compile(&source_buffer, compilation_arguments.data(),
//                                      static_cast<u32>(compilation_arguments.size()), idxc_include_handler,
//                                      IID_PPV_ARGS(&compile_result)))) {
//        LOG_ERROR("{}", "Internal error or API misuse! Compile Failed");
//        return;
//    }
//
//    // Get compilation errors (if any).
//    IDxcBlobUtf8 *errors{};
//
//    if (SUCCEEDED(compile_result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr)) && errors != nullptr &&
//        errors->GetStringLength() != 0) {
//        LOG_ERROR("Warnings and Errors:{}", errors->GetStringPointer());
//    }
//
//    //
//    // Quit if the compilation failed.
//    //
//    HRESULT hrStatus;
//    if (FAILED(compile_result->GetStatus(&hrStatus)) || FAILED(hrStatus)) {
//        // Compilation failed, but successful HRESULT was returned.
//        // Could reuse the compiler and allocator objects. For simplicity, exit here anyway
//        LOG_ERROR("{}", "Compilation Failed");
//        return;
//    }
//
//    // save result to disk
//    // TODO(hylu): prevent expose std::iostream to handle IO
//    std::ofstream os;
//    os.open(compile_args.out_file_path, std::ios::binary | std::ios::out);
//    ShaderBinaryHeader header{};
//    header.header = hsb_header;
//    
//    os.write(reinterpret_cast<const char *>(&header), sizeof(ShaderBinaryHeader));
//
//    header.shader_blob_offset = sizeof(ShaderBinaryHeader);
//
//    // IR/IL
//    //TODO(hylu) save both dxil and spirv, for changing backend in runtime?
//    if (compile_args.target_api == ShaderTargetAPI::DXIL) {
//
//    } else if (compile_args.target_api == ShaderTargetAPI::SPIRV) {
//        IDxcBlob *spirv_code;
//        compile_result->GetResult(&spirv_code);
//        os.write(static_cast<const char *>(spirv_code->GetBufferPointer()), spirv_code->GetBufferSize());
//        header.shader_blob_size = static_cast<u32>(spirv_code->GetBufferSize());
//    }
//    //
//    // Save pdb.
//    //
//    //IDxcBlob *p_pdb{};
//    //if (SUCCEEDED(compile_result->GetOutput(DXC_OUT_PDB, IID_PPV_ARGS(&p_pdb), nullptr))) {
//    //    os.write(static_cast<const char *>(p_pdb->GetBufferPointer()), p_pdb->GetBufferSize());
//    //}
//
//    //
//    // Print hash.
//    //
//    //CComPtr<IDxcBlob> pHash = nullptr;
//    //if (SUCCEEDED(pResults->GetOutput(DXC_OUT_SHADER_HASH, IID_PPV_ARGS(&pHash), nullptr)) && pHash != nullptr) {
//    //    wprintf(L"Hash: ");
//    //    DxcShaderHash *pHashBuf = (DxcShaderHash *)pHash->GetBufferPointer();
//    //    for (int i = 0; i < _countof(pHashBuf->HashDigest); i++)
//    //        wprintf(L"%x", pHashBuf->HashDigest[i]);
//    //    wprintf(L"\n");
//    //}
//
//    // create reflection data(only for dxil)
//    //IDxcBlob* pReflectionData;
//    //if (SUCCEEDED(compile_result->GetOutput(DXC_OUT_REFLECTION, IID_PPV_ARGS(&pReflectionData), nullptr)) &&
//    //    pReflectionData != nullptr) {
//    //
//    //}
//
//    //IDxcBlob *reflection_blob{};
//    //CHECK_DX_RESULT(compile_result->GetOutput(DXC_OUT_REFLECTION, IID_PPV_ARGS(&reflection_blob), nullptr));
//
//    //DxcBuffer reflectionBuffer{reflection_blob->GetBufferPointer(), reflection_blob->GetBufferSize(), 0};
//
//    //ID3D12ShaderReflection *shader_reflection{};
//    //idxc_utils->CreateReflection(&reflectionBuffer, IID_PPV_ARGS(&shader_reflection));
//    //D3D12_SHADER_DESC shaderDesc{};
//    //shader_reflection->GetDesc(&shaderDesc);
//    os.seekp(0, std::ios::beg);
//    os.write(reinterpret_cast<const char *>(&header), sizeof(ShaderBinaryHeader));
//    os.close();
//
//    // release resources
//}
//
//} // namespace Horizon
