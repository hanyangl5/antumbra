#include "framework/01core/io/file_system.h"
#include "framework/02gal/shader/shader_compiler.h"
#include "framework/02gal/shader/spirv_reflection.h"
#include <catch2/catch_test_macros.hpp>

#include <iostream>

using namespace ant;

TEST_CASE("compile single shader ") {
    ShaderCompiler sc;

    ant::str test_fs = "\
    struct PSInput {\
        float4 color : COLOR;\
    };\
\
    float4 PSMain(PSInput input) : SV_TARGET {\
        return input.color;\
    }\
";

    ShaderSourceBlob source;
    source.data = test_fs.data();
    source.size = test_fs.size();
    ShaderCompileDesc desc;
    desc.entry = "PSMain";
    desc.optimization_level = ShaderOptimizationLevel::NONE;
    desc.target_api = ShaderBlobType::DXIL;
    desc.target_profile = ShaderTargetProfile::PS_6_0;

    CompiledShader *ret = sc.Compile(source, desc);
    REQUIRE(ret != nullptr);
    ret->Release();
    desc.target_api = ShaderBlobType::SPIRV;
    CompiledShader *ret2 = sc.Compile(source, desc);
    REQUIRE(ret2 != nullptr);
    ret2->Release();
}
