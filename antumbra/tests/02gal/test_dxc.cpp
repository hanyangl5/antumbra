#include "framework/02gal/dxc/shader_compiler.h"

#include <catch2/catch_test_macros.hpp>

#include <iostream>

using namespace ant;

TEST_CASE("compile single shader ") { ShaderCompiler sc;
    ShaderSourceBlob source;
    ShaderCompileDesc desc;
    CompiledShader* ret =  sc.Compile(source, desc);
    ret->Release();
}
