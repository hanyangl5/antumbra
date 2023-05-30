/*
 * Copyright (c) 2017-2022 The Forge Interactive Inc.
 *
 * This file is part of The-Forge
 * (see https://github.com/ConfettiFX/The-Forge).
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
*/

#pragma once
#include "framework/01core/utils/utils.h"
#include "../enum.h"
#include <ctype.h>

namespace ant::gal {

void destroyShaderReflection(ShaderReflection *pReflection);

void createPipelineReflection(ShaderReflection *pReflection, u32 stageCount, PipelineReflection *pOutReflection);
void destroyPipelineReflection(PipelineReflection *pReflection);

inline bool isDescriptorRootConstant(const char *resourceName) {
    char lower[MAX_RESOURCE_NAME_LENGTH] = {};
    u32 length = (u32)strlen(resourceName);
    for (u32 i = 0; i < length; ++i) {
        lower[i] = tolower(resourceName[i]);
    }
    return strstr(lower, "rootconstant") || strstr(lower, "pushconstant");
}

inline bool isDescriptorRootCbv(const char *resourceName) {
    char lower[MAX_RESOURCE_NAME_LENGTH] = {};
    u32 length = (u32)strlen(resourceName);
    for (u32 i = 0; i < length; ++i) {
        lower[i] = tolower(resourceName[i]);
    }
    return strstr(lower, "rootcbv");
}

//void serializeReflection(File* pInFile, Reflection* pReflection);
//void deserializeReflection(File* pOutFile, Reflection* pReflection);
} // namespace ant::gal