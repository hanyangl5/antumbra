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

#if defined(_WINDOWS) || defined(XBOX)
#include <stdlib.h>
#include <sys/stat.h>
#include <windows.h>

#endif

#if defined(__APPLE__)
#if !defined(TARGET_IOS)
#import <Carbon/Carbon.h>
#else
#include <stdint.h>
typedef uint64_t uint64;
#endif
#elif defined(__ANDROID__)
#include <android/log.h>
#include <android_native_app_glue.h>

#elif defined(__linux__) && !defined(VK_USE_PLATFORM_GGP)
#define VK_USE_PLATFORM_XLIB_KHR
#if defined(VK_USE_PLATFORM_XLIB_KHR) || defined(VK_USE_PLATFORM_XCB_KHR)
#include <X11/Xutil.h>
// X11 defines primitive types which conflict with Forge libraries
#undef Bool
#endif
#elif defined(NX64)
#include "../../../Switch/Common_3/OS/NX/NXTypes.h"
#elif defined(ORBIS)
#define THREAD_STACK_SIZE_ORBIS (64u * TF_KB)
#endif

namespace ant {

struct WindowHandle {
    // TODO: Separate vulkan ext from choosing xlib vs xcb
#if defined(VK_USE_PLATFORM_XLIB_KHR)
    Display *display;
    Window window;
    Atom xlib_wm_delete_window;
    Colormap colormap;
#elif defined(VK_USE_PLATFORM_XCB_KHR)
    xcb_connection_t *connection;
    xcb_window_t window;
    xcb_screen_t *screen;
    xcb_intern_atom_reply_t *atom_wm_delete_window;
#elif defined(__ANDROID__)
    ANativeWindow *window;
    ANativeActivity *activity;
    AConfiguration *configuration;
#else
    void *window; //hWnd
#endif
};
} // namespace ant
