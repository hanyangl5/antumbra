#pragma once
#include "framework/01core/utils/utils.h"
#include "../enum.h"
#include <ctype.h>
#include "compile_desc.h"
namespace ant::gal {
  PipelineReflection create_pipelien_reflection()
//void destroyShaderReflection(ShaderReflection *pReflection);
//
//void createPipelineReflection(ShaderReflection *pReflection, u32 stageCount, PipelineReflection *pOutReflection);
//void destroyPipelineReflection(PipelineReflection *pReflection);
//
//inline bool isDescriptorRootConstant(const char *resourceName) {
//    char lower[MAX_RESOURCE_NAME_LENGTH] = {};
//    u32 length = (u32)strlen(resourceName);
//    for (u32 i = 0; i < length; ++i) {
//        lower[i] = tolower(resourceName[i]);
//    }
//    return strstr(lower, "rootconstant") || strstr(lower, "pushconstant");
//}
//
//inline bool isDescriptorRootCbv(const char *resourceName) {
//    char lower[MAX_RESOURCE_NAME_LENGTH] = {};
//    u32 length = (u32)strlen(resourceName);
//    for (u32 i = 0; i < length; ++i) {
//        lower[i] = tolower(resourceName[i]);
//    }
//    return strstr(lower, "rootcbv");
//}

//void serializeReflection(File* pInFile, Reflection* pReflection);
//void deserializeReflection(File* pOutFile, Reflection* pReflection);


} // namespace ant::gal