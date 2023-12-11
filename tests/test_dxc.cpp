#include "framework/utils/io/file_system.h"
#include "framework/utils/memory/memory.h"
#include "framework/gal/shader/shader_compiler.h"
//#include <catch2/catch_test_macros.hpp>
#include <doctest.h>

#include <iostream>

using namespace ante;

ante::str test_fs = "\
    struct PSInput {\
        float4 color : COLOR;\
    };\
    \
    float4 PSMain(PSInput input) : SV_TARGET {\
        return input.color;\
    }";
ante::str test_cs = "\
#define UPDATE_FREQ_NONE space0\n\
#define UPDATE_FREQ_PER_FRAME space1\n\
#define UPDATE_FREQ_PER_BATCH space2\n\
#define UPDATE_FREQ_PER_DRAW space3\n\
#define UPDATE_FREQ_BINDLESS space4\n\
// #define CBUFFER(NAME, FREQ, REG, BINDING) [[vk::binding(BINDING, FREQ)]] cbuffer NAME : register(REG, FREQ)\n\
// #define RES(TYPE, NAME, FREQ, REG, BINDING) [[vk::binding(BINDING, FREQ)]] TYPE NAME : register(REG, FREQ)\n\
#define CBUFFER(NAME, FREQ) cbuffer NAME : register(FREQ)\n\
#define RES(TYPE, NAME, FREQ) TYPE NAME : register(FREQ)\n\
CBUFFER(cbuf1, UPDATE_FREQ_NONE)\n\
{\n\
    float4 a;\n\
    float b;\n\
};\n\
RES(StructuredBuffer<uint>, sbuffer, UPDATE_FREQ_NONE);\n\
RES(StructuredBuffer<uint3>, sbuffer_arr[4], UPDATE_FREQ_NONE);\n\
RES(Texture2D<float>, tex1, UPDATE_FREQ_PER_FRAME);\n\
RES(SamplerState, spl, UPDATE_FREQ_PER_FRAME);\n\
RES(RWTexture2D<float2>, tex2, UPDATE_FREQ_PER_FRAME);\n\
RES(RWTexture2D<float2>, tex_arr[4], UPDATE_FREQ_PER_FRAME);\n\
[numthreads(8, 8, 1)]\n\
void CS_MAIN(uint3 thread_id: SV_DispatchThreadID) \n\
{\n\
    float color = tex1.SampleLevel(spl, float2(thread_id.xy), 0);\n\
    tex2[thread_id.xy] = color;\n\
}";

TEST_CASE("compile single shader") {
    using namespace ante::gal;
    shader_compiler sc;
    shader_source_blob source;
    source.set(test_fs.data(), test_fs.size());
    shader_compile_desc desc;
    desc.entry = "PSMain";
    desc.optimization_level = shader_optimization_level::NONE;
    desc.target_api = shader_blob_type::DXIL;
    desc.target_profile = shader_target_profile::PS_6_0;

    compiled_shader *ret = sc.compile(&source, &desc);
    REQUIRE(ret != nullptr);
    ret->release();
    desc.target_api = shader_blob_type::SPIRV;
    compiled_shader *ret2 = sc.compile(&source, &desc);
    REQUIRE(ret2 != nullptr);
    ret2->release();
}

TEST_CASE("shader reflection test") {
    using namespace ante::gal;
    shader_compiler sc;
    shader_source_blob source;
    source.set(test_cs.data(), test_cs.size());
    shader_compile_desc desc;
    desc.entry = "CS_MAIN";
    desc.optimization_level = shader_optimization_level::NONE;
    desc.target_api = shader_blob_type::DXIL;
    desc.target_profile = shader_target_profile::CS_6_0;

    compiled_shader *ret = sc.compile(&source, &desc);
    REQUIRE(ret != nullptr);
    ret->release();
    desc.target_api = shader_blob_type::SPIRV;
    compiled_shader *ret2 = sc.compile(&source, &desc);
    REQUIRE(ret2 != nullptr);
    ret2->release();
}

TEST_CASE("shader group from source test") {
    //using namespace ante::gal;
    //shader_compiler sc;
    //shader_source_blob source;
    //source.set(test_cs.data(), test_cs.size());
    //shader_compile_desc desc;
    //desc.entry = "CS_MAIN";
    //desc.optimization_level = shader_optimization_level::NONE;
    //desc.target_api = shader_blob_type::DXIL;
    //desc.target_profile = shader_target_profile::CS_6_0;

    //
}
