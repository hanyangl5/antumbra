#pragma once

// standard libraries
#include <filesystem>

// third party libraries
#include <dxcompiler/d3d12shader.h>
#include <dxcompiler/dxcapi.h>

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

    DELETE_COPY_MOVE(ShaderCompiler)

    //static void Compile(const ant::str &blob, const ShaderCompileDesc &compile_args);

    //static void CompileShaders(const ShaderCompilationSettings &settings);

    CompiledShader *Compile(const ShaderSourceBlob &blob, const ShaderCompileDesc &desc);

  private:
    IDxcUtils *idxc_utils;
    IDxcCompiler3 *idxc_compiler;
    IDxcIncludeHandler *idxc_include_handler;
};

} // namespace ant