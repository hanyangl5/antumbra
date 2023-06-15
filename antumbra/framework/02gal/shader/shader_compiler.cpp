#include "shader_compiler.h"

#include "framework/01core/io/file_system.h"
#include "framework/01core/logging/log.h"
#include "framework/01core/memory/memory.h"

//namespace std {
//template <typename X, typename Y> struct std::hash<std::pair<X, Y>> {
//    std::size_t operator()(const std::pair<X, Y> &pair) const {
//        return std::hash<X>()(pair.first) ^ std::hash<Y>()(pair.second);
//    }
//};
//} // namespace std

namespace ant::gal {

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

shader_compiler::shader_compiler() noexcept {
    //HMODULE dxil_module = ::LoadLibrary("dxil.dll");
    //if (!dxil_module) {
    //LOG_ERROR("failed to find dxil library");
    //    return;
    //}
    HRESULT hr;
    hr = (DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&m_idxc_compiler)));
    if (FAILED(hr)) {
        LOG_ERROR("failed to create idxc compiler");
        return;
    }
    hr = (DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&m_idxc_utils)));
    if (FAILED(hr)) {
        LOG_ERROR("failed to create idxc utils");
        return;
    }
    hr = (m_idxc_utils->CreateDefaultIncludeHandler((&m_idxc_include_handler)));
    if (FAILED(hr)) {
        LOG_ERROR("failed to create idxc include handler");
        return;
    }
}

shader_compiler::~shader_compiler() noexcept {}
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
//void shader_compiler::CompileShaders(const ShaderCompilationSettings &settings) {
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
//    //                              shader_compiler::Compile(shader_text, shader.args);
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
//        shader_compiler::Compile(shader_texts[shader.path], shader.args);
//    }
//}

compiled_shader *shader_compiler::compile(shader_source_blob *blob, shader_compile_desc *desc) {
    IDxcBlobEncoding *hlsl_blob;

    bool b_spv = desc->target_api == shader_blob_type::SPIRV ? true : false;

    HRESULT hr = (m_idxc_utils->CreateBlob(blob->data(), static_cast<u32>(blob->size()), 0, &hlsl_blob));
    if (FAILED(hr)) {
        LOG_ERROR("failed to create blob");
        return nullptr;
    }
    ant::stack_allocator stack_memory(512);
    ant::vector<LPCWSTR> args(&stack_memory);

    // entry point
    args.push_back(L"-E");
    WCHAR entry[64];
    swprintf(entry, 64, L"%hs", desc->entry);
    args.push_back(entry);
    // target profile
    args.push_back(L"-T");
    const wchar_t *tp = utils_to_hlsl_target_profile(desc->target_profile);
    args.push_back(tp);

    args.push_back(DXC_ARG_WARNINGS_ARE_ERRORS); // warning are errors
    args.push_back(DXC_ARG_ALL_RESOURCES_BOUND);

    for (auto &d : desc->defines) {
        args.push_back(L"-D");
        args.push_back(d.wstring().c_str());
    }

    for (auto &i : desc->include_search_path) {
        args.push_back(L"-I");
        args.push_back(i.wstring().c_str());
    }

    args.push_back(L"-HV 2021");

    if (true) {
        args.push_back(DXC_ARG_PACK_MATRIX_ROW_MAJOR);
    }

    switch (desc->optimization_level) {
    case shader_optimization_level::NONE:
        args.push_back(DXC_ARG_DEBUG);
        args.push_back(DXC_ARG_SKIP_OPTIMIZATIONS);
        break;
    case shader_optimization_level::O0:
        args.push_back(DXC_ARG_OPTIMIZATION_LEVEL0);
        break;
    case shader_optimization_level::O1:
        args.push_back(DXC_ARG_OPTIMIZATION_LEVEL1);
        break;
    case shader_optimization_level::O2:
        args.push_back(DXC_ARG_OPTIMIZATION_LEVEL2);
        break;
    case shader_optimization_level::O3:
        args.push_back(DXC_ARG_OPTIMIZATION_LEVEL3);
        break;
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
    if (FAILED(m_idxc_compiler->Compile(&source_buffer, args.data(), static_cast<u32>(args.size()),
                                        m_idxc_include_handler, IID_PPV_ARGS(&compile_result)))) {
        LOG_ERROR("{}", "Internal error or API misuse! Compile Failed");
        return nullptr;
    }

    IDxcBlob *byte_code = nullptr;
    IDxcBlob *pdb = nullptr;
    IDxcBlob *hash = nullptr;
    IDxcBlob *reflection = nullptr;

    //HRESULT hrStatus;
    //if (FAILED(compile_result->GetStatus(&hrStatus)) || FAILED(hrStatus)) {
    //    // Compilation failed, but successful HRESULT was returned.
    //    // Could reuse the compiler and allocator objects. For simplicity, exit here anyway
    //    LOG_ERROR("compilation faild");
    //    return nullptr;
    //}
    // Get compilation errors (if any).
    IDxcBlobUtf8 *errors = nullptr;

    if (SUCCEEDED(compile_result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr)) && errors != nullptr &&
        errors->GetStringLength() != 0) {
        LOG_ERROR("Warnings and Errors: {}", errors->GetStringPointer());
        return nullptr;
    }

    if (FAILED(compile_result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&byte_code), nullptr)) || byte_code == nullptr) {
        LOG_ERROR("failed to get DXC_OUT_OBJECT");
        return nullptr;
    }

    // TODO(hylu): fix error when gte pdb
    //if (FAILED(compile_result->GetOutput(DXC_OUT_PDB, IID_PPV_ARGS(&pdb), nullptr)) || pdb == nullptr) {
    //    LOG_ERROR("failed to get DXC_OUT_PDB");
    //    return nullptr;
    //}
    if (!b_spv && FAILED(compile_result->GetOutput(DXC_OUT_SHADER_HASH, IID_PPV_ARGS(&hash), nullptr))) {
        LOG_ERROR("failed to get DXC_OUT_SHADER_HASH");
        return nullptr;
    }

    if (!b_spv && FAILED(compile_result->GetOutput(DXC_OUT_REFLECTION, IID_PPV_ARGS(&reflection), nullptr))) {
        LOG_ERROR("failed tot get DXC_OUT_REFLECTION");
        return nullptr;
    }

    compiled_shader *ret = ant::ant_alloc<compiled_shader>(allocator::default_memory_allocator);
    if (byte_code) {
        ret->m_byte_code.set(byte_code->GetBufferPointer(), byte_code->GetBufferSize());
        ret->m_p_byte_code = byte_code;
    }
    if (pdb) {
        ret->m_pdb.set(pdb->GetBufferPointer(), pdb->GetBufferSize());
        ret->m_p_pdb = pdb;
    }
    if (hash) {
        ret->m_hash.set(hash->GetBufferPointer(), hash->GetBufferSize());
        ret->m_p_hash = hash;
    }
    if (reflection) {
        ret->m_dxc_reflection.set(reflection->GetBufferPointer(), reflection->GetBufferSize());
        ret->m_p_dxc_reflection = reflection;
    }
    ret->m_entry = desc->entry;
    ret->m_type = desc->target_api;

    if (b_spv) {
        ret->m_reflection = ant::ant_alloc<shader_reflection>(ant::allocator::default_memory_allocator);
        ret->create_shader_reflection();
    }

    return ret;
}

void compiled_shader::release() {
    if (m_p_byte_code) {
        reinterpret_cast<IDxcBlob *>(m_p_byte_code)->Release();
        m_byte_code.reset();
    }
    if (m_p_dxc_reflection) {
        reinterpret_cast<IDxcBlob *>(m_p_dxc_reflection)->Release();
        m_dxc_reflection.reset();
    }
    if (m_p_pdb) {
        reinterpret_cast<IDxcBlob *>(m_p_pdb)->Release();
        m_pdb.reset();
    }
    if (m_p_hash) {
        reinterpret_cast<IDxcBlob *>(m_p_hash)->Release();
        m_hash.reset();
    }
    ant_free(m_reflection, allocator::default_memory_allocator);
}

void compiled_shader_group::set_from_source(shader_source_blob *source, shader_gourp_source_desc *descs) {
    shader_compiler sc;
    if (descs->desc_vert != nullptr) {
        compiled_shader *ret = sc.compile(source, descs->desc_vert);
        if (!ret) {
            LOG_ERROR("failed to compile vertex shader");
            return;
        }
        m_vert = ret;
        m_stage_flags |= gal_shader_stage::VERT;
    }
    if (descs->desc_frag != nullptr) {
        compiled_shader *ret = sc.compile(source, descs->desc_frag);
        if (!ret) {
            LOG_ERROR("failed to compile hull shader");
            return;
        }
        m_frag = ret;
        m_stage_flags |= gal_shader_stage::FRAG;
    }
    if (descs->desc_geom != nullptr) {
        compiled_shader *ret = sc.compile(source, descs->desc_geom);
        if (!ret) {
            LOG_ERROR("failed to compile geometry shader");
            return;
        }
        m_geom = ret;
        m_stage_flags |= gal_shader_stage::GEOM;
    }
    if (descs->desc_hull != nullptr) {
        compiled_shader *ret = sc.compile(source, descs->desc_hull);
        if (!ret) {
            LOG_ERROR("failed to compile hull shader");
            return;
        }
        m_hull = sc.compile(source, descs->desc_hull);
        m_stage_flags |= gal_shader_stage::HULL;
    }
    if (descs->desc_domin != nullptr) {
        compiled_shader *ret = sc.compile(source, descs->desc_domin);
        if (!ret) {
            LOG_ERROR("failed to compile domain shader");
            return;
        }
        m_domain = ret;
        m_stage_flags |= gal_shader_stage::DOMN;
    }
    if (descs->desc_comp != nullptr) {
        compiled_shader *ret = sc.compile(source, descs->desc_comp);
        if (!ret) {
            LOG_ERROR("failed to compile compute shader");
            return;
        }
        m_comp = ret;
        m_stage_flags |= gal_shader_stage::COMP;
    }
    // FIXME(hyl5): error handling? release memory?
    m_b_same_root_signature = true;
    create_pipeline_reflection();
}

void compiled_shader_group::set(compiled_shader_gourp_desc *desc) {
    m_vert = desc->vert;
    m_frag = desc->frag;
    m_domain = desc->domain;
    m_hull = desc->hull;
    m_geom = desc->geom;
    m_comp = desc->comp;
    if (desc->vert) {
        m_stage_flags |= gal_shader_stage::VERT;
    }
    if (desc->frag) {
        m_stage_flags |= gal_shader_stage::FRAG;
    }
    if (desc->comp) {
        m_stage_flags |= gal_shader_stage::COMP;
    }
    if (desc->domain) {
        m_stage_flags |= gal_shader_stage::DOMN;
    }
    if (desc->hull) {
        m_stage_flags |= gal_shader_stage::HULL;
    }
    if (desc->geom) {
        m_stage_flags |= gal_shader_stage::GEOM;
    }

    create_pipeline_reflection();
}

void compiled_shader_group::release() {
    m_vert->release();
    m_frag->release();
    m_domain->release();
    m_hull->release();
    m_geom->release();
    m_comp->release();
}

compiled_shader *compiled_shader_group::vert() { return m_vert != nullptr ? m_vert : nullptr; }
compiled_shader *compiled_shader_group::frag() { return m_frag != nullptr ? m_frag : nullptr; }
compiled_shader *compiled_shader_group::domain() { return m_domain != nullptr ? m_domain : nullptr; }
compiled_shader *compiled_shader_group::geom() { return m_geom != nullptr ? m_geom : nullptr; }
compiled_shader *compiled_shader_group::hull() { return m_hull != nullptr ? m_hull : nullptr; }
compiled_shader *compiled_shader_group::comp() { return m_comp != nullptr ? m_comp : nullptr; }

gal_shader_stage compiled_shader_group::stages() { return m_stage_flags; }

pipeline_reflection *compiled_shader_group::reflection() { return m_pipeline_reflection; }

} // namespace ant::gal
