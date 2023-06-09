#pragma once

// standard libraries
#include <filesystem>

// third party libraries
#include <dxc/d3d12shader.h>
#include <dxc/dxcapi.h>

// project headers
#include "framework/01core/memory/container.h"
#include "framework/01core/singleton/public_singleton.h"
#include "framework/01core/utils/utils.h"

#include "compile_desc.h"

namespace ant {

class ShaderCompiler : public Singleton<ShaderCompiler> {
  public:
    ShaderCompiler() noexcept;
    virtual ~ShaderCompiler() noexcept;

    constexpr ShaderCompiler(const ShaderCompiler &rhs) noexcept = delete;
    constexpr ShaderCompiler &operator=(const ShaderCompiler &rhs) noexcept = delete;
    constexpr ShaderCompiler(ShaderCompiler &&rhs) noexcept = delete;
    constexpr ShaderCompiler &operator=(ShaderCompiler &&rhs) noexcept = delete;

    //static void Compile(const ant::str &blob, const ShaderCompileDesc &compile_args);

    //static void CompileShaders(const ShaderCompilationSettings &settings);

    CompiledShader *Compile(const ShaderSourceBlob &blob, const ShaderCompileDesc &desc);

  private:
    IDxcUtils *idxc_utils;
    IDxcCompiler3 *idxc_compiler;
    IDxcIncludeHandler *idxc_include_handler;
};

} // namespace ant