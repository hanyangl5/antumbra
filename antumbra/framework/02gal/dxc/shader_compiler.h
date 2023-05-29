//
//#pragma once
//
//// standard libraries
//#include <filesystem>
//// third party libraries
//#include <d3d12shader.h>
//#include <dxcapi.h>
//
//// project headers
//#include <framework/01core/utils/utils.h>
//#include <framework/01core/singleton/public_singleton.h>
//#include <framework/01core/memory/container.h>
//
//namespace ant {
//
//inline constexpr ant::fixed_array<char, 4> hsb_header{'h', 's', 'b', '1'};
//
//
//struct ShaderCompilationArgs {
//    ant::str entry_point;
//    ShaderTargetProfile target_profile;
//    ShaderOptimizationLevel optimization_level;
//    ShaderTargetAPI target_api;
//    std::filesystem::path out_file_path;
//    std::filesystem::path include_path;
//};
//
//struct ShaderBinaryHeader {
//    fixed_array<char, 4> header;
//    u32 shader_blob_offset;
//    u32 shader_blob_size;
//    //u32 pdb_offset;
//};
//
//class ShaderCompiler : public Singleton<ShaderCompiler> {
//  public:
//    ShaderCompiler() noexcept;
//    virtual ~ShaderCompiler() noexcept;
//
//    constexpr ShaderCompiler(const ShaderCompiler &rhs) noexcept = delete;
//    constexpr ShaderCompiler &operator=(const ShaderCompiler &rhs) noexcept = delete;
//    constexpr ShaderCompiler(ShaderCompiler &&rhs) noexcept = delete;
//    constexpr ShaderCompiler &operator=(ShaderCompiler &&rhs) noexcept = delete;
//
//    static void Compile(const ant::str &blob, const ShaderCompilationArgs &compile_args);
//
//    static void CompileShaders(const ShaderCompilationSettings &settings);
//
//  private:
//    void InternalCompile(const ant::str &blob, const ShaderCompilationArgs &compile_args);
//
//  private:
//    IDxcUtils *idxc_utils;
//    IDxcCompiler3 *idxc_compiler;
//    IDxcIncludeHandler *idxc_include_handler;
//};
//
//} // namespace Horizon