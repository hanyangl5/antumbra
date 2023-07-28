/*
 * Code adapted from ConfettiFX The-Forge
 * 
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

#include "gal_vulkan.h"

#include <optional>

#ifdef WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#elif __linux__
#define VK_USE_PLATFORM_XLIB_KHR
#elif __APPLE__
#define VK_USE_PLATFORM_MACOS_MVK
#endif
#include <vulkan/vulkan.h>
#define VMA_RECORDING_ENABLED 1
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#define VMA_IMPLEMENTATION 1

#ifndef NDEBUG
#ifndef VMA_DEBUG_LOG_FORMAT
#define VMA_DEBUG_LOG_FORMAT(format, ...)
/*
   #define VMA_DEBUG_LOG_FORMAT(format, ...) do { \
       printf((format), __VA_ARGS__); \
       printf("\n"); \
   } while(false)
   */
#endif

#ifndef VMA_DEBUG_LOG
#define VMA_DEBUG_LOG(str) VMA_DEBUG_LOG_FORMAT("%s", (str))
#endif
#endif // !NDEBUG
#include <vk_mem_alloc.h>

#include "framework/01core/io/file_system.h"
#include "framework/01core/logging/log.h"
#include "framework/01core/math/math.h"
#include "framework/01core/memory/memory.h"
#include "framework/02gal/shader/shader_compiler.h"
#include "gal_vulkan_enum.h"
#include "gal_vulkan_utils.h"
#define ANT_VK_API_VERSION VK_API_VERSION_1_3

namespace ante::gal {

// gal_error_code vk_result_to_gal_error_code(VkResult res) {

//}
// TODO(hyl5): chain macro to enable physical device features
// #define ADD_TO_NEXT_CHAIN(condition, next)         \
//    if ((condition)) {                             \
//        base->pNext = (VkBaseOutStructure *)&next; \
//        base = (VkBaseOutStructure *)base->pNext;  \
//    }

constexpr u32 utils_to_vk_queue_index(gal_queue_type type) { return static_cast<u32>(type) - 1; }
constexpr u32 graphics_queue_index = utils_to_vk_queue_index(gal_queue_type::graphcis);
constexpr u32 compute_queue_index = utils_to_vk_queue_index(gal_queue_type::compute);
constexpr u32 transfer_queue_index = utils_to_vk_queue_index(gal_queue_type::transfer);

constexpr u32 utils_to_vk_queue_family_index(gal_queue_type queue_type, vk_context *vk_ctx) {
    return vk_ctx->queues[utils_to_vk_queue_index(queue_type)].queue_family_index;
}

gal_error_code vk_init_gal(gal_context *context) {
    vk_context *vk_ctx = ante::memory::alloc<vk_context>(nullptr);
    if (vk_ctx == gal_null) {
        return gal_error_code::ERR;
    }
    load_gal_vk_functions();
    *context = vk_ctx;
    return gal_error_code::SUC;
}

gal_error_code vk_destroy_gal(gal_context context) {
    if (context != gal_null) {
        vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
        ante::memory::afree(vk_ctx);
        context = gal_null;
        offload_gal_vk_functions();
    }
    return gal_error_code::SUC;
}

gal_error_code vk_create_instance(gal_desc *gal_desc, gal_context *context) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(*context);
    vk_ctx->m_gal_desc = *gal_desc;

    ACQUIRE_STACK_MEMORY_RESOURCE(stack_memory, 2048);
    ante::vector<const char *> required_instance_layers(&stack_memory);
    ante::vector<const char *> required_instance_extensions(&stack_memory);

    if (gal_desc->b_debug_layer) {
        required_instance_layers.emplace_back("VK_LAYER_KHRONOS_validation");
        required_instance_extensions.emplace_back("VK_EXT_debug_utils");
    }

    // to query device features
    required_instance_extensions.emplace_back("VK_KHR_get_physical_device_properties2");

    if (gal_desc->b_surface) {
        required_instance_extensions.emplace_back("VK_KHR_surface");
#ifdef WIN32
        required_instance_extensions.emplace_back("VK_KHR_win32_surface");
#endif // WIN32
    }

    u32 available_layer_count = 0;

    u32 available_extension_count = 0;
    vkEnumerateInstanceLayerProperties(&available_layer_count, nullptr);
    vkEnumerateInstanceExtensionProperties(nullptr, &available_extension_count, nullptr);

    ante::vector<VkLayerProperties> available_layers(available_layer_count, &stack_memory);
    ante::vector<VkExtensionProperties> available_extensions(available_extension_count, &stack_memory);

    vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers.data());
    vkEnumerateInstanceExtensionProperties(nullptr, &available_extension_count, available_extensions.data());
    u32 required_layer_count = static_cast<u32>(required_instance_layers.size());
    u32 required_extension_count = static_cast<u32>(required_instance_extensions.size());

    for (u32 i = 0; i < available_layer_count; i++) {
        for (u32 j = 0; j < static_cast<u32>(required_instance_layers.size()); j++) {
            if (std::strcmp(required_instance_layers[j], available_layers[i].layerName) == 0) {
                required_layer_count--;
            }
        }
    }

    if (required_layer_count != 0) {
        return gal_error_code::ERR;
    }
    for (u32 i = 0; i < available_extension_count; i++) {
        for (u32 j = 0; j < static_cast<u32>(required_instance_extensions.size()); j++) {
            if (std::strcmp(required_instance_extensions[j], available_extensions[i].extensionName) == 0) {
                required_extension_count--;
            }
        }
    }
    if (required_extension_count != 0) {
        return gal_error_code::ERR;
    }

    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = nullptr;
    app_info.applicationVersion = 0u;
    app_info.pEngineName = nullptr;
    app_info.engineVersion = 0u;
    app_info.apiVersion = ANT_VK_API_VERSION;

    VkInstanceCreateInfo instance_create_info{};
    instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo = &app_info;
    instance_create_info.flags = 0;
    instance_create_info.enabledExtensionCount = static_cast<u32>(required_instance_extensions.size());
    instance_create_info.ppEnabledExtensionNames = required_instance_extensions.data();
    instance_create_info.enabledLayerCount = static_cast<u32>(required_instance_layers.size());
    instance_create_info.ppEnabledLayerNames = required_instance_layers.data();

    VkResult res = vkCreateInstance(&instance_create_info, nullptr, &(vk_ctx->instance));
    if (res == VK_SUCCESS && vk_ctx->instance != VK_NULL_HANDLE) {
        return gal_error_code::SUC;
    } else {
        return gal_error_code::ERR;
    }
}
gal_error_code vk_destroy_instance(gal_context *context) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(*context);

    vkDestroyInstance(vk_ctx->instance, nullptr);
    return gal_error_code::SUC;
}
gal_error_code vk_create_device(gal_desc *gal_desc, gal_context *context) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(*context);

    // ACQUIRE_STACK_MEMORY_RESOURCE(stack_memory, 1024);
    ACQUIRE_STACK_MEMORY_RESOURCE(stack_memory, 1024);
    ante::vector<const char *> required_device_extensions(&stack_memory);
    ante::vector<const char *> required_device_layers(&stack_memory);
    if (gal_desc->b_swap_chain) {
        required_device_extensions.emplace_back("VK_KHR_swapchain");
    }
    required_device_extensions.emplace_back("VK_KHR_maintenance1");
    required_device_extensions.emplace_back("VK_KHR_dynamic_rendering");

    u32 physical_device_count = 0;
    // get physical device count
    vkEnumeratePhysicalDevices(vk_ctx->instance, &physical_device_count, nullptr);

    ante::vector<VkPhysicalDevice> physical_devices(physical_device_count, &stack_memory);
    if (physical_device_count == 0) {
        LOG_ERROR("no available device");
        return gal_error_code::ERR;
    }
    vkEnumeratePhysicalDevices(vk_ctx->instance, &physical_device_count, physical_devices.data());

    u32 required_queue_count = 1u;
    if (gal_desc->b_async_compute) {
        required_queue_count++;
    }
    if (gal_desc->b_async_transfer) {
        required_queue_count++;
    }

    // pick gpu
    auto pick_gpu = [&](ante::fixed_array<u32, 3> &queue_family_indices) -> gal_error_code {
        for (const auto &physical_device : physical_devices) {
            u32 queue_family_count = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count,
                                                     nullptr); // Get queue family properties

            // early exist

            if (queue_family_count < required_queue_count) {
                // unsupported device
                continue;
            }
            ante::vector<VkQueueFamilyProperties> queue_family_properties(queue_family_count, &stack_memory);

            queue_family_properties.resize(queue_family_count);
            vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count,
                                                     queue_family_properties.data()); // Get queue family properties

            auto getQueueFamilyIndex = [&](gal_queue_type queue_type) -> std::optional<u32> {
                // For other queue types or if no separate compute queue is present, return the first one to support
                // the requested flags

                if (queue_type == gal_queue_type::graphcis) {
                    return 0;
                }

                // Dedicated queue for compute
                // Try to find a queue family index that supports compute but not graphics
                if (queue_type == gal_queue_type::compute) {
                    for (u32 i = 0; i < static_cast<u32>(queue_family_properties.size()); i++) {
                        if ((queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) &&
                            ((queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)) {
                            return i;
                        }
                    }
                }

                // Dedicated queue for transfer
                // Try to find a queue family index that supports transfer but not graphics and compute
                if (queue_type == gal_queue_type::transfer) {
                    for (u32 i = 0; i < static_cast<u32>(queue_family_properties.size()); i++) {
                        if ((queue_family_properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) &&
                            ((queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) &&
                            ((queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0)) {
                            return i;
                        }
                    }
                }
                LOG_WARN("request queue not found, fallback to graphcis queue");
                return {};
            };

            auto graphics_queue_family_index = getQueueFamilyIndex(gal_queue_type::graphcis);
            auto compute_queue_family_index = graphics_queue_family_index;
            auto transfer_queue_family_index = graphics_queue_family_index;
            // TODO(hyl5): code is not clear
            if (gal_desc->b_async_compute) {
                auto t = getQueueFamilyIndex(gal_queue_type::compute);
                if (!t.has_value()) {
                    continue;
                }
                compute_queue_family_index = t.value();
            }
            if (gal_desc->b_async_transfer) {
                auto t = getQueueFamilyIndex(gal_queue_type::transfer);
                if (!t.has_value()) {
                    continue;
                }
                transfer_queue_family_index = t.value();
            }

            // check layers and extensions
            u32 available_layer_count = 0;
            u32 available_extension_count = 0;
            const char *layer_name = nullptr;
            vkEnumerateDeviceLayerProperties(physical_device, &available_layer_count, nullptr);
            ante::vector<VkLayerProperties> available_device_layers(available_layer_count, &stack_memory);
            vkEnumerateDeviceLayerProperties(physical_device, &available_layer_count, available_device_layers.data());

            vkEnumerateDeviceExtensionProperties(physical_device, layer_name, &available_extension_count, nullptr);
            ante::vector<VkExtensionProperties> available_device_extensions(available_extension_count, &stack_memory);
            vkEnumerateDeviceExtensionProperties(physical_device, layer_name, &available_extension_count,
                                                 available_device_extensions.data());

            u32 required_layer_count = static_cast<u32>(required_device_layers.size());
            u32 required_extension_count = static_cast<u32>(required_device_extensions.size());
            for (u32 i = 0; i < available_layer_count; i++) {
                for (u32 j = 0; j < static_cast<u32>(required_device_layers.size()); j++) {
                    if (std::strcmp(required_device_layers[j], available_device_layers[i].layerName) == 0) {
                        required_layer_count--;
                    }
                }
            }

            if (required_layer_count != 0) {
                continue;
            }
            for (u32 i = 0; i < available_extension_count; i++) {
                for (u32 j = 0; j < static_cast<u32>(required_device_extensions.size()); j++) {
                    if (std::strcmp(required_device_extensions[j], available_device_extensions[i].extensionName) == 0) {
                        required_extension_count--;
                    }
                }
            }
            if (required_extension_count != 0) {
                continue;
            }

            vk_ctx->active_gpu = physical_device;

            queue_family_indices = {graphics_queue_family_index.value(), compute_queue_family_index.value(),
                                    transfer_queue_family_index.value()};

            return gal_error_code::SUC;
        }
        return gal_error_code::ERR;
    };

    ante::fixed_array<u32, 3> queue_family_indices;

    gal_error_code result = pick_gpu(queue_family_indices);
    if (result != gal_error_code::SUC) {
        return gal_error_code::ERR;
    }
    if (vk_ctx->active_gpu == VK_NULL_HANDLE) {
        return gal_error_code::ERR;
    }
    // TODO(hyl5): provide chain method to add features
    VkPhysicalDeviceShaderDrawParametersFeatures shader_draw_parameters_features{};
    shader_draw_parameters_features.shaderDrawParameters = true;
    shader_draw_parameters_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETERS_FEATURES;

    VkPhysicalDeviceDynamicRenderingFeatures dyanmic_rendering_features{};

    dyanmic_rendering_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
    dyanmic_rendering_features.dynamicRendering = true;
    dyanmic_rendering_features.pNext = &shader_draw_parameters_features;

    VkPhysicalDeviceDescriptorIndexingFeatures descriptor_indexing_features{};
    descriptor_indexing_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT;
    descriptor_indexing_features.pNext = &dyanmic_rendering_features;
    descriptor_indexing_features.runtimeDescriptorArray = VK_TRUE;
    descriptor_indexing_features.descriptorBindingVariableDescriptorCount = VK_TRUE;
    descriptor_indexing_features.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
    descriptor_indexing_features.descriptorBindingPartiallyBound = VK_TRUE;

    VkPhysicalDeviceFeatures2 device_features{};
    device_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    device_features.pNext = &descriptor_indexing_features;
    vkGetPhysicalDeviceFeatures2(vk_ctx->active_gpu, &device_features);
    ante::set<u32> queue_family_indices_set(queue_family_indices.begin(), queue_family_indices.end(), &stack_memory);
    ante::vector<VkDeviceQueueCreateInfo> device_queue_create_info(queue_family_indices_set.size(), &stack_memory);
    u32 idx = 0;
    ante::fixed_array<u32, 3> required_queue_family_indices;
    for (auto &index : queue_family_indices_set) {
        required_queue_family_indices[idx++] = index;
    }

    for (u32 i = 0; i < static_cast<u32>(queue_family_indices_set.size()); i++) {
        device_queue_create_info[i] = {};
        device_queue_create_info[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        device_queue_create_info[i].pNext = nullptr;
        device_queue_create_info[i].flags = 0;
        device_queue_create_info[i].queueFamilyIndex = required_queue_family_indices[i];
        device_queue_create_info[i].queueCount = 1;
        device_queue_create_info[i].pQueuePriorities = &vk_ctx->default_queue_property;
    }
    VkDeviceCreateInfo device_create_info{};
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pQueueCreateInfos = device_queue_create_info.data();
    device_create_info.queueCreateInfoCount = static_cast<u32>(device_queue_create_info.size());
    device_create_info.enabledExtensionCount = static_cast<u32>(required_device_extensions.size());
    device_create_info.ppEnabledExtensionNames = required_device_extensions.data();
    device_create_info.enabledLayerCount = static_cast<u32>(required_device_layers.size());
    device_create_info.ppEnabledLayerNames = required_device_layers.data();
    device_create_info.pNext = &device_features;

    VkResult res = vkCreateDevice(vk_ctx->active_gpu, &device_create_info, nullptr, &vk_ctx->device);
    if (res != VK_SUCCESS || vk_ctx->device == VK_NULL_HANDLE) {
        return gal_error_code::ERR;
    }
    // create device queue
    vkGetDeviceQueue(vk_ctx->device, queue_family_indices[graphics_queue_index], 0,
                     &vk_ctx->queues[graphics_queue_index].queue);

    if (gal_desc->b_async_compute) {
        vkGetDeviceQueue(vk_ctx->device, queue_family_indices[compute_queue_index], 0,
                         &vk_ctx->queues[compute_queue_index].queue);
    } else {
        vkGetDeviceQueue(vk_ctx->device, queue_family_indices[graphics_queue_index], 0,
                         &vk_ctx->queues[compute_queue_index].queue);
    }

    if (gal_desc->b_async_transfer) {
        vkGetDeviceQueue(vk_ctx->device, queue_family_indices[transfer_queue_index], 0,
                         &vk_ctx->queues[transfer_queue_index].queue);
    } else {
        vkGetDeviceQueue(vk_ctx->device, queue_family_indices[graphics_queue_index], 0,
                         &vk_ctx->queues[transfer_queue_index].queue);
    }

    vk_ctx->queues[utils_to_vk_queue_index(gal_queue_type::graphcis)].queue_family_index =
        queue_family_indices[graphics_queue_index];
    vk_ctx->queues[utils_to_vk_queue_index(gal_queue_type::compute)].queue_family_index =
        queue_family_indices[compute_queue_index];
    vk_ctx->queues[utils_to_vk_queue_index(gal_queue_type::transfer)].queue_family_index =
        queue_family_indices[transfer_queue_index];
    vk_ctx->queues[utils_to_vk_queue_index(gal_queue_type::graphcis)].m_type = gal_queue_type::graphcis;
    vk_ctx->queues[utils_to_vk_queue_index(gal_queue_type::compute)].m_type = gal_queue_type::compute;
    vk_ctx->queues[utils_to_vk_queue_index(gal_queue_type::transfer)].m_type = gal_queue_type::transfer;

    // TODO(hyl5): query supported memory type of gpu, platform differences
    return gal_error_code::SUC;
}

gal_error_code vk_destroy_device(gal_context *context) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(*context);

    vkDestroyDevice(vk_ctx->device, nullptr);
    return gal_error_code::SUC;
}

gal_error_code vk_create_memory_allocator(gal_context *context) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(*context);

    VmaVulkanFunctions vulkan_functions{};
    vulkan_functions.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
    vulkan_functions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo vma_create_info = {};
    vma_create_info.device = vk_ctx->device;
    vma_create_info.physicalDevice = vk_ctx->active_gpu;
    vma_create_info.instance = vk_ctx->instance;
    vma_create_info.pVulkanFunctions = &vulkan_functions;
    vma_create_info.vulkanApiVersion = ANT_VK_API_VERSION;
    VkResult result = vmaCreateAllocator(&vma_create_info, &vk_ctx->vma_allocator);

    if (result == VK_SUCCESS && vk_ctx->vma_allocator != VK_NULL_HANDLE) {
        return gal_error_code::SUC;
    } else {
        return gal_error_code::ERR;
    }
}
gal_error_code vk_destroy_memory_allocator(gal_context *context) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(*context);

    vmaDestroyAllocator(vk_ctx->vma_allocator);
    return gal_error_code::SUC;
}

gal_error_code vk_create_buffer(gal_context context, gal_buffer_desc *desc, gal_buffer *buffer) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vk_buffer *vk_buf = ante::memory::alloc<ante::gal::vk_buffer>(nullptr);
    if (!vk_buf) {
        return gal_error_code::ERR;
    }

    VkBufferCreateInfo buffer_create_info{};
    buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.size = desc->size;
    buffer_create_info.usage = util_to_vk_buffer_usage(desc->descriptor_types);

    VmaAllocationCreateInfo alloc_info = {};

    if (desc->memory_flags == gal_memory_flag::UNDEFINED) {
        return gal_error_code::ERR;
    }
    if ((desc->memory_flags & gal_memory_flag::GPU_DEDICATED) != gal_memory_flag::UNDEFINED) {
        alloc_info.requiredFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        alloc_info.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_ONLY;
        buffer_create_info.usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT; //
    }
    if ((desc->memory_flags & gal_memory_flag::CPU_UPLOAD) != gal_memory_flag::UNDEFINED) {
        alloc_info.requiredFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
        alloc_info.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_CPU_TO_GPU;
        buffer_create_info.usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT; // stage buffer upload
    }
    if ((desc->memory_flags & gal_memory_flag::GPU_DOWNLOAD) != gal_memory_flag::UNDEFINED) {
        alloc_info.requiredFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
        alloc_info.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_TO_CPU;
        buffer_create_info.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT; // stage buffer download
    }

    if ((desc->flags & gal_buffer_flag::OWN_MEMORY) != gal_buffer_flag::UNDEFINED) {
        alloc_info.flags |= VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
    } else if ((desc->flags & gal_buffer_flag::PERSISTENT_MAP) != gal_buffer_flag::UNDEFINED) {
        alloc_info.flags |= VMA_ALLOCATION_CREATE_MAPPED_BIT;
    }

    VmaAllocationInfo vma_alloc_info = {};

    VkResult result = vmaCreateBuffer(vk_ctx->vma_allocator, &buffer_create_info, &alloc_info, &vk_buf->m_buffer,
                                      &vk_buf->m_allocation, &vma_alloc_info);

    if (result != VK_SUCCESS || vk_buf->m_buffer == VK_NULL_HANDLE || vk_buf->m_allocation == VK_NULL_HANDLE) {
        return gal_error_code::ERR;
    }
    vk_buf->cpu_mapped_address = vma_alloc_info.pMappedData;
    // todo create buffer view for texelbuffer

    *buffer = vk_buf;
    vk_buf->m_gal_buffer_desc = *desc;
    return gal_error_code::SUC;
}

gal_error_code vk_destroy_buffer(gal_context context, gal_buffer buffer) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);

    if (buffer != gal_null) {
        vk_buffer *vk_buf = reinterpret_cast<vk_buffer *>(buffer);
        vmaDestroyBuffer(vk_ctx->vma_allocator, vk_buf->m_buffer, vk_buf->m_allocation);
        ante::memory::afree(vk_buf);
        buffer = gal_null;
    }
    return gal_error_code::SUC;
}

gal_error_code vk_create_texture(gal_context context, gal_texture_desc *desc, gal_texture *texture) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vk_texture *vk_tex = ante::memory::alloc<ante::gal::vk_texture>(nullptr);

    if (!vk_tex) {
        return gal_error_code::ERR;
    }

    u32 array_size = desc->array_size > 1u ? desc->array_size : 1u;
    u32 depth = desc->depth > 1u ? desc->depth : 1u;
    bool b_is_cubemap =
        ((desc->descriptor_types & gal_descriptor_type::TEXTURE_CUBE) != gal_descriptor_type::UNDEFINED) &&
        desc->array_size == 6;
    VkImageCreateInfo image_create_info{};
    image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.imageType = utils_to_vk_image_type(desc->dimension);
    image_create_info.extent.width = desc->width;
    image_create_info.extent.height = desc->height;
    image_create_info.extent.depth = depth;
    image_create_info.mipLevels = desc->mip_level;
    image_create_info.arrayLayers = array_size;
    image_create_info.format = galtextureformat_to_vkformat(desc->format);
    image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_create_info.flags = 0; //utils_to_vk_image_creation_flags(desc->descriptor_types);
    image_create_info.usage = utils_to_vk_image_usage(desc->descriptor_types, desc->memory_flags);
    image_create_info.samples = utils_to_vk_sample_count_flags(desc->sample_count);

    // cubemap flags
    if (b_is_cubemap) {
        // TODO(hyl5): should gal layer validate parameter?
        //if (array_size != 6) {
        //    return gal_error_code::invalid_parameter;
        //}
        image_create_info.flags |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
    }
    if (array_size > 1) {
        image_create_info.flags |= VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT;
    }
    VkFormatProperties format_props{};
    vkGetPhysicalDeviceFormatProperties(vk_ctx->active_gpu, image_create_info.format, &format_props);

    // Verify that GPU supports this format
    VkFormatFeatureFlags format_features = util_vk_image_usage_to_format_features(image_create_info.usage);

    if (!(format_props.optimalTilingFeatures & format_features)) {
        return gal_error_code::ERR;
    }
    VmaAllocationCreateInfo allocation_create_info{};

    if ((desc->memory_flags & gal_memory_flag::GPU_DEDICATED) != gal_memory_flag::UNDEFINED) {
        allocation_create_info.requiredFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        allocation_create_info.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_ONLY;
    }
    if ((desc->memory_flags & gal_memory_flag::CPU_UPLOAD) != gal_memory_flag::UNDEFINED) {
        allocation_create_info.requiredFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
        allocation_create_info.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_CPU_TO_GPU;
    }
    if ((desc->memory_flags & gal_memory_flag::GPU_DOWNLOAD) != gal_memory_flag::UNDEFINED) {
        allocation_create_info.requiredFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
        allocation_create_info.usage = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_TO_CPU;
    }
    if (desc->native_handle) {
        vk_tex->m_image = reinterpret_cast<VkImage>(desc->native_handle);
    } else {
        VkResult result = vmaCreateImage(vk_ctx->vma_allocator, &image_create_info, &allocation_create_info,
                                         &vk_tex->m_image, &vk_tex->m_allocation, nullptr);
        if (result != VK_SUCCESS || vk_tex->m_image == VK_NULL_HANDLE || vk_tex->m_allocation == VK_NULL_HANDLE) {
            return gal_error_code::ERR;
        }
    }

    VkImageViewCreateInfo image_view_ci{};
    image_view_ci.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    image_view_ci.pNext = nullptr;
    image_view_ci.flags = 0;
    image_view_ci.image = vk_tex->m_image;
    image_view_ci.viewType = utils_to_vk_image_view_type(desc->dimension);
    VkFormat format = galtextureformat_to_vkformat(desc->format);
    image_view_ci.format = format;
    image_view_ci.components.r = VK_COMPONENT_SWIZZLE_R;
    image_view_ci.components.g = VK_COMPONENT_SWIZZLE_G;
    image_view_ci.components.b = VK_COMPONENT_SWIZZLE_B;
    image_view_ci.components.a = VK_COMPONENT_SWIZZLE_A;
    image_view_ci.subresourceRange.aspectMask = util_vk_determine_aspect_mask(format, true);
    // TODO(hyl5): support multiple view
    image_view_ci.subresourceRange.baseMipLevel = 0;
    image_view_ci.subresourceRange.levelCount = desc->mip_level;
    image_view_ci.subresourceRange.baseArrayLayer = 0;
    image_view_ci.subresourceRange.layerCount = desc->array_size;
    // create srv
    VkImageView view;
    if ((desc->descriptor_types & gal_descriptor_type::TEXTURE) != gal_descriptor_type::UNDEFINED ||
        (desc->descriptor_types & gal_descriptor_type::RW_TEXTURE) != gal_descriptor_type::UNDEFINED) {
        VkResult result = vkCreateImageView(vk_ctx->device, &image_view_ci, nullptr, &view);
        if (result != VK_SUCCESS) {
            return gal_error_code::ERR;
        }
        vk_tex->m_view = view;
    }
    // create srv
    //if ((desc->descriptor_types & gal_descriptor_type::TEXTURE) != gal_descriptor_type::UNDEFINED) {
    //    VkResult result = vkCreateImageView(vk_ctx->device, &image_view_ci, nullptr, &vk_tex->srv);
    //    if (result != VK_SUCCESS) {
    //        return gal_error_code::ERR;
    //    }
    //}
    //// create uav
    //if ((desc->descriptor_types & gal_descriptor_type::RW_TEXTURE) != gal_descriptor_type::UNDEFINED) {
    //    VkResult result = vkCreateImageView(vk_ctx->device, &image_view_ci, nullptr, &vk_tex->uav);
    //    if (result != VK_SUCCESS) {
    //        return gal_error_code::ERR;
    //    }
    //}
    //

    *texture = vk_tex;
    vk_tex->m_desc = *desc;
    return gal_error_code::SUC;
}

gal_error_code vk_destroy_texture(gal_context context, gal_texture texture) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    if (texture != gal_null) {
        vk_texture *vk_tex = reinterpret_cast<vk_texture *>(texture);
        if ((vk_tex->m_desc.texture_flags & gal_texture_flag::TEXTURE_CREATION_FLAG_IMPORT_BIT) ==
            gal_texture_flag::TEXTURE_CREATION_FLAG_UNFEFINED) {
            vmaDestroyImage(vk_ctx->vma_allocator, vk_tex->m_image, vk_tex->m_allocation);
            //vkDestroyImageView(vk_ctx->device, vk_tex->image_view, nullptr);
        }
        ante::memory::afree(vk_tex);
        texture = gal_null;
    }
    return gal_error_code::SUC;
}

gal_error_code vk_create_sampler(gal_context context, gal_sampler_desc *sampler_desc, gal_sampler *sampler) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vk_sampler *vk_spl = ante::memory::alloc<ante::gal::vk_sampler>(nullptr);

    if (!vk_spl) {
        return gal_error_code::ERR;
    }

    VkSamplerCreateInfo sampler_create_info{};
    sampler_create_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    sampler_create_info.flags = 0;
    sampler_create_info.minFilter = utils_to_vk_filter(sampler_desc->min_filter);
    sampler_create_info.magFilter = utils_to_vk_filter(sampler_desc->mag_filter);
    sampler_create_info.mipmapMode = utils_to_vk_mip_map_mode(sampler_desc->mip_mode);
    sampler_create_info.addressModeU = utils_to_vk_address_mode(sampler_desc->address_mode_u);
    sampler_create_info.addressModeV = utils_to_vk_address_mode(sampler_desc->address_mode_v);
    sampler_create_info.addressModeW = utils_to_vk_address_mode(sampler_desc->address_mode_w);
    //sampler_create_info.borderColor; TODO(hyl5):border color

    sampler_create_info.minLod = sampler_desc->min_lod;
    sampler_create_info.maxLod = sampler_desc->max_lod;
    sampler_create_info.maxAnisotropy = sampler_desc->max_anisotropy;
    sampler_create_info.compareEnable = sampler_desc->compare_mode != gal_compare_mode::UNDEFINED;
    sampler_create_info.compareOp = utils_to_vk_compare_op(sampler_desc->compare_mode);
    sampler_create_info.mipLodBias = sampler_desc->mip_lod_bias;
    VkResult result = vkCreateSampler(vk_ctx->device, &sampler_create_info, nullptr, &vk_spl->m_sampler);
    if (result != VK_SUCCESS || vk_spl->m_sampler == VK_NULL_HANDLE) {

        return gal_error_code::ERR;
    }
    *sampler = vk_spl;
    vk_spl->m_gal_sampler_desc = *sampler_desc;
    return gal_error_code::SUC;
}

gal_error_code vk_destroy_sampler(gal_context context, gal_sampler sampler) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    if (sampler != gal_null) {
        vk_sampler *vk_spl = reinterpret_cast<vk_sampler *>(sampler);
        vkDestroySampler(vk_ctx->device, vk_spl->m_sampler, nullptr);
        ante::memory::afree(vk_spl);
        sampler = gal_null;
    }
    return gal_error_code::SUC;
}

gal_error_code vk_create_render_target(gal_context context, gal_render_target_desc *desc,
                                       gal_render_target *render_target) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vk_render_target *vk_rt = ante::memory::alloc<vk_render_target>(nullptr);

    if (!vk_rt) {
        return gal_error_code::ERR;
    }

    bool const isDepth = gal_tf_is_depth_only(desc->format) || gal_tf_is_depth_and_stencil(desc->format);

    if (((isDepth) && (desc->descriptor_types & gal_descriptor_type::RW_TEXTURE) != gal_descriptor_type::UNDEFINED)) {
        return gal_error_code::ERR;
    }

    desc->mip_level = _max(1u, desc->mip_level);

    gal_texture_desc textureDesc = {};
    textureDesc.array_size = 1;
    textureDesc.depth = desc->depth;
    textureDesc.texture_flags = desc->flags;
    textureDesc.format = desc->format;
    textureDesc.width = desc->width;
    textureDesc.height = desc->height;
    textureDesc.mip_level = desc->mip_level;
    textureDesc.sample_count = desc->sample_count;
    textureDesc.texture_sample_quality = desc->texture_sample_quality;
    textureDesc.native_handle = desc->native_handle;
    textureDesc.dimension = desc->dimension;
    textureDesc.memory_flags = gal_memory_flag::GPU_DEDICATED;
    if (!isDepth)
        textureDesc.initial_state |= gal_resource_state::RENDER_TARGET;
    else
        textureDesc.initial_state |= gal_resource_state::DEPTH_WRITE;

    // Set this by default to be able to sample the rendertarget in shader
    textureDesc.descriptor_types = desc->descriptor_types;
    // Create SRV by default for a render target unless this is on tile texture where SRV is not supported
    if ((desc->flags & gal_texture_flag::TEXTURE_CREATION_FLAG_ON_TILE) ==
        gal_texture_flag::TEXTURE_CREATION_FLAG_UNFEFINED) {
        textureDesc.descriptor_types |= gal_descriptor_type::TEXTURE;

    } else {
        if ((textureDesc.descriptor_types & gal_descriptor_type::TEXTURE) != gal_descriptor_type::UNDEFINED ||
            (textureDesc.descriptor_types & gal_descriptor_type::RW_TEXTURE) != gal_descriptor_type::UNDEFINED) {
            LOG_WARN("On tile textures do not support DESCRIPTOR_TYPE_TEXTURE or DESCRIPTOR_TYPE_RW_TEXTURE");
        }
        // On tile textures do not support SRV/UAV as there is no backing memory
        // You can only read these textures as input attachments inside same render pass
        textureDesc.descriptor_types &= ~(gal_descriptor_type::TEXTURE | gal_descriptor_type::RW_TEXTURE);
    }

    if (isDepth) {
        // Make sure depth/stencil format is supported - fall back to VK_FORMAT_D16_UNORM if not
        VkFormat vk_depth_stencil_format = galtextureformat_to_vkformat(desc->format);
        if (VK_FORMAT_UNDEFINED != vk_depth_stencil_format) {
            VkImageFormatProperties properties{};
            VkResult vk_res = vkGetPhysicalDeviceImageFormatProperties(
                vk_ctx->active_gpu, vk_depth_stencil_format, VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 0, &properties);
            // Fall back to something that's guaranteed to work
            if (VK_SUCCESS != vk_res) {
                textureDesc.format = gal_texture_format::D32_SFLOAT;
                LOG_WARN("Depth stencil format (%u) not supported. Falling back to D32 format", desc->height);
            }
        }
    }
    // gal_create_texture
    gal_error_code gal_result = vk_create_texture(context, &textureDesc, &vk_rt->m_texture);
    if (gal_result != gal_error_code::SUC) {
        return gal_error_code::ERR;
    }

    VkImageViewType view_type = utils_to_vk_image_view_type(desc->dimension);

    vk_texture *vk_tex = reinterpret_cast<vk_texture *>(vk_rt->m_texture);
    VkImageViewCreateInfo rtvDesc{};
    rtvDesc.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    rtvDesc.pNext = nullptr;
    rtvDesc.flags = 0;
    rtvDesc.image = vk_tex->m_image;
    rtvDesc.viewType = view_type;
    rtvDesc.format = galtextureformat_to_vkformat(textureDesc.format);
    rtvDesc.components.r = VK_COMPONENT_SWIZZLE_R;
    rtvDesc.components.g = VK_COMPONENT_SWIZZLE_G;
    rtvDesc.components.b = VK_COMPONENT_SWIZZLE_B;
    rtvDesc.components.a = VK_COMPONENT_SWIZZLE_A;
    rtvDesc.subresourceRange.aspectMask = util_vk_determine_aspect_mask(rtvDesc.format, true);
    rtvDesc.subresourceRange.baseMipLevel = 0;
    rtvDesc.subresourceRange.levelCount = 1;
    rtvDesc.subresourceRange.baseArrayLayer = 0;
    rtvDesc.subresourceRange.layerCount = 1;

    VkResult result = (vkCreateImageView(vk_ctx->device, &rtvDesc, nullptr, &vk_rt->pVkDescriptor));
    if (result != VK_SUCCESS || vk_rt->pVkDescriptor == VK_NULL_HANDLE) {
        return gal_error_code::ERR;
    }

    vk_rt->pVkSliceDescriptors = static_cast<VkImageView *>(malloc(desc->mip_level * sizeof(VkImageView)));

    for (u32 i = 0; i < desc->mip_level; ++i) {
        rtvDesc.subresourceRange.baseMipLevel = i;
        //if ((desc->descriptor_types & DESCRIPTOR_TYPE_RENDER_TARGET_ARRAY_SLICES) ||
        //    (desc->descriptor_types & DESCRIPTOR_TYPE_RENDER_TARGET_DEPTH_SLICES)) {
        //    for (u32 j = 0; j < depthOrArraySize; ++j) {
        //        rtvDesc.subresourceRange.layerCount = 1;
        //        rtvDesc.subresourceRange.baseArrayLayer = j;
        //        result = (vkCreateImageView(vk_ctx->device, &rtvDesc, nullptr,
        //                                    &pRenderTarget->mVulkan.pVkSliceDescriptors[i * depthOrArraySize + j]));
        //    }
        //} else {
        result = (vkCreateImageView(vk_ctx->device, &rtvDesc, nullptr, &vk_rt->pVkSliceDescriptors[i]));
        if (result != VK_SUCCESS || vk_rt->pVkSliceDescriptors[i] == VK_NULL_HANDLE) {
            return gal_error_code::ERR;
        }
        //}
    }

    // Unlike DX12, Vulkan textures start in undefined layout.
    // To keep in line with DX12, we transition them to the specified layout manually so app code doesn't have to worry about this
    // Render targets wont be created during runtime so this overhead will be minimal
    //util_initial_transition(pRenderer, pRenderTarget->pTexture, desc->mStartState);

    *render_target = vk_rt;
    vk_rt->m_desc = *desc;
    return gal_error_code::SUC;
}

gal_error_code vk_destroy_render_target(gal_context context, gal_render_target render_target) {
    if (render_target != gal_null) {
        vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
        vk_render_target *vk_rt = reinterpret_cast<vk_render_target *>(render_target);
        destroy_texture(context, vk_rt->m_texture);
        vkDestroyImageView(vk_ctx->device, vk_rt->pVkDescriptor, nullptr);

        for (u32 i = 0; i < vk_rt->m_desc.mip_level; ++i) {
            vkDestroyImageView(vk_ctx->device, vk_rt->pVkSliceDescriptors[i], nullptr);
        }
        ante::memory::afree(vk_rt);
        render_target = gal_null;
    }
    return gal_error_code::SUC;
}

// surface
gal_error_code vk_create_swap_chain(gal_context context, gal_swap_chain_desc *desc, gal_swap_chain *swap_chain) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vk_swap_chain *vk_sc = ante::memory::alloc<vk_swap_chain>(nullptr);

    if (!vk_sc) {
        return gal_error_code::ERR;
    }

    VkResult result;
#if defined(VK_USE_PLATFORM_WIN32_KHR)
    VkWin32SurfaceCreateInfoKHR add_info{};
    add_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    add_info.pNext = nullptr;
    add_info.flags = 0;
    add_info.hinstance = ::GetModuleHandle(nullptr);
    add_info.hwnd = desc->hwnd_window;
    result = vkCreateWin32SurfaceKHR(vk_ctx->instance, &add_info, nullptr, &vk_sc->m_surface);
    if (result != VK_SUCCESS || vk_sc->m_surface == VK_NULL_HANDLE) {
        return gal_error_code::ERR;
    }
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    VkXlibSurfaceCreateInfoKHR add_info{};
    add_info.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    add_info.pNext = nullptr;
    add_info.flags = 0;
    add_info.dpy = desc->mWindowHandle.display;   //TODO
    add_info.window = desc->mWindowHandle.window; //TODO
    result = (vkCreateXlibSurfaceKHR(vk_ctx->instance, &add_info, nullptr, &vk_sc->m_surface));
    if (result != VK_SUCCESS || vk_sc->m_surface == VK_NULL_HANDLE) {
        return gal_error_code::ERR;
    }
#elif defined(VK_USE_PLATFORM_XCB_KHR)
    VkXcbSurfaceCreateInfoKHR add_info{};
    add_info.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    add_info.pNext = nullptr;
    add_info.flags = 0;
    add_info.connection = desc->mWindowHandle.connection; //TODO
    add_info.window = desc->mWindowHandle.window;         //TODO
    result = (vkCreateXcbSurfaceKHR(pRenderer->pVkInstance, &add_info, nullptr, &vk_sc->m_surface));
    if (result != VK_SUCCESS || vk_sc->m_surface == VK_NULL_HANDLE) {
        return gal_error_code::ERR;
    }
#elif defined(VK_USE_PLATFORM_IOS_MVK)
    // Add IOS support here
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
    // Add MacOS support here
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
    VkAndroidSurfaceCreateInfoKHR add_info{};
    add_info.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
    add_info.pNext = nullptr;
    add_info.flags = 0;
    add_info.window = desc->mWindowHandle.window;
    result = (vkCreateAndroidSurfaceKHR(vk_ctx->instance, &add_info, nullptr, &vk_sc->m_surface));
    if (result != VK_SUCCESS || vk_sc->m_surface == VK_NULL_HANDLE) {
        return gal_error_code::ERR;
    }
#elif defined(VK_USE_PLATFORM_GGP)
    extern VkResult ggpCreateSurface(VkInstance, vk_sc->m_surfaceKHR * surface);
    result = (ggpCreateSurface(pRenderer->pVkInstance, &vk_sc->m_surface));
    if (result != VK_SUCCESS || vk_sc->m_surface == VK_NULL_HANDLE) {
        return gal_error_code::ERR;
    }
#elif defined(VK_USE_PLATFORM_VI_NN)
    extern VkResult nxCreateSurface(VkInstance, vk_sc->m_surfaceKHR * surface);
    result = (nxCreateSurface(vk_ctx->instance, &vk_sc->m_surface));
    if (result != VK_SUCCESS || vk_sc->m_surface == VK_NULL_HANDLE) {
        return gal_error_code::ERR;
    }
#else
#error PLATFORM NOT SUPPORTED
#endif

    ACQUIRE_STACK_MEMORY_RESOURCE(stack_memory, 256);

    // Image count
    if (0 == desc->image_count) {
        desc->image_count = 2;
    }

    VkSurfaceCapabilitiesKHR caps;
    result = (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vk_ctx->active_gpu, vk_sc->m_surface, &caps));
    if (result != VK_SUCCESS) {
        return gal_error_code::ERR;
    }
    // fix invalid image count
    if ((caps.maxImageCount > 0) && (desc->image_count > caps.maxImageCount)) {
        LOG_WARN("Changed requested SwapChain images {} to maximum allowed SwapChain images {}", desc->image_count,
                 caps.maxImageCount);
        desc->image_count = caps.maxImageCount;
    }
    if (desc->image_count < caps.minImageCount) {
        LOG_WARN("Changed requested SwapChain images {%u} to minimum required SwapChain images {%u}", desc->image_count,
                 caps.minImageCount);
        desc->image_count = caps.minImageCount;
    }

    // Surface format
    // Select a surface format, depending on whether HDR is available.

    VkSurfaceFormatKHR surface_format = {VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_MAX_ENUM_KHR};
    u32 surfaceFormatCount = 0;
    ante::vector<VkSurfaceFormatKHR> formats(&stack_memory);

    // Get surface formats count
    result = vkGetPhysicalDeviceSurfaceFormatsKHR(vk_ctx->active_gpu, vk_sc->m_surface, &surfaceFormatCount, nullptr);
    if (result != VK_SUCCESS) {
        return gal_error_code::ERR;
    }
    // Allocate and get surface formats
    formats.resize(surfaceFormatCount);
    result = (vkGetPhysicalDeviceSurfaceFormatsKHR(vk_ctx->active_gpu, vk_sc->m_surface, &surfaceFormatCount,
                                                   formats.data()));
    if (result != VK_SUCCESS) {
        return gal_error_code::ERR;
    }
    if ((1 == surfaceFormatCount) && (VK_FORMAT_UNDEFINED == formats[0].format)) {
        surface_format.format = VK_FORMAT_B8G8R8A8_UNORM;
        surface_format.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    } else {
        VkSurfaceFormatKHR hdrSurfaceFormat = {VK_FORMAT_A2B10G10R10_UNORM_PACK32, VK_COLOR_SPACE_HDR10_ST2084_EXT};
        VkFormat requested_format = galtextureformat_to_vkformat(desc->format);
        VkColorSpaceKHR requested_color_space = requested_format == hdrSurfaceFormat.format
                                                    ? hdrSurfaceFormat.colorSpace
                                                    : VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        for (u32 i = 0; i < surfaceFormatCount; ++i) {
            if ((requested_format == formats[i].format) && (requested_color_space == formats[i].colorSpace)) {
                surface_format.format = requested_format;
                surface_format.colorSpace = requested_color_space;
                break;
            }
        }

        // Default to VK_FORMAT_B8G8R8A8_UNORM if requested format isn't found
        if (VK_FORMAT_UNDEFINED == surface_format.format) {
            surface_format.format = VK_FORMAT_B8G8R8A8_UNORM;
            surface_format.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        }
    }

    // Free formats
    formats.clear();

    // The VK_PRESENT_MODE_FIFO_KHR mode must always be present as per spec
    // This mode waits for the vertical blank ("v-sync")
    VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
    u32 swapChainImageCount = 0;
    ante::vector<VkPresentModeKHR> modes(&stack_memory);
    // Get present mode count
    result = (vkGetPhysicalDeviceSurfacePresentModesKHR(vk_ctx->active_gpu, vk_sc->m_surface, &swapChainImageCount,
                                                        nullptr));
    if (result != VK_SUCCESS) {
        return gal_error_code::ERR;
    }
    // Allocate and get present modes
    modes.resize(swapChainImageCount);

    result = (vkGetPhysicalDeviceSurfacePresentModesKHR(vk_ctx->active_gpu, vk_sc->m_surface, &swapChainImageCount,
                                                        modes.data()));
    if (result != VK_SUCCESS) {
        return gal_error_code::ERR;
    }
    const u32 preferredModeCount = 4;
    VkPresentModeKHR preferredModeList[preferredModeCount] = {
        VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_FIFO_RELAXED_KHR,
        VK_PRESENT_MODE_FIFO_KHR};
    u32 preferredModeStartIndex = desc->b_vsync ? 2 : 0;

    for (u32 j = preferredModeStartIndex; j < preferredModeCount; ++j) {
        VkPresentModeKHR mode = preferredModeList[j];
        u32 i = 0;
        for (; i < swapChainImageCount; ++i) {
            if (modes[i] == mode) {
                break;
            }
        }
        if (i < swapChainImageCount) {
            present_mode = mode;
            break;
        }
    }

    // Swapchain
    VkExtent2D extent{};
    extent.width = std::clamp(desc->width, caps.minImageExtent.width, caps.maxImageExtent.width);
    extent.height = std::clamp(desc->height, caps.minImageExtent.height, caps.maxImageExtent.height);

    VkSurfaceTransformFlagBitsKHR pre_transform;
    // #TODO: Add more if necessary but identity should be enough for now
    if (caps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
        pre_transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    } else {
        pre_transform = caps.currentTransform;
    }

    VkCompositeAlphaFlagBitsKHR compositeAlphaFlags[] = {
        VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
    };
    VkCompositeAlphaFlagBitsKHR composite_alpha = VK_COMPOSITE_ALPHA_FLAG_BITS_MAX_ENUM_KHR;
    for (VkCompositeAlphaFlagBitsKHR flag : compositeAlphaFlags) {
        if (caps.supportedCompositeAlpha & flag) {
            composite_alpha = flag;
            break;
        }
    }
    if (composite_alpha == VK_COMPOSITE_ALPHA_FLAG_BITS_MAX_ENUM_KHR) {
        return gal_error_code::ERR;
    }

    VkSwapchainCreateInfoKHR swapChainCreateInfo{};
    swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainCreateInfo.pNext = nullptr;
    swapChainCreateInfo.flags = 0;
    swapChainCreateInfo.surface = vk_sc->m_surface;
    swapChainCreateInfo.minImageCount = desc->image_count;
    swapChainCreateInfo.imageFormat = surface_format.format;
    swapChainCreateInfo.imageColorSpace = surface_format.colorSpace;
    swapChainCreateInfo.imageExtent = extent;
    swapChainCreateInfo.imageArrayLayers = 1;
    swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapChainCreateInfo.queueFamilyIndexCount = 0;
    swapChainCreateInfo.preTransform = pre_transform;
    swapChainCreateInfo.compositeAlpha = composite_alpha;
    swapChainCreateInfo.presentMode = present_mode;
    swapChainCreateInfo.clipped = VK_TRUE;
    swapChainCreateInfo.oldSwapchain = 0;
    result = (vkCreateSwapchainKHR(vk_ctx->device, &swapChainCreateInfo, nullptr, &vk_sc->m_swap_chain));
    if (result != VK_SUCCESS || vk_sc->m_swap_chain == VK_NULL_HANDLE) {
        return gal_error_code::ERR;
    }
    desc->format = vkformat_to_galtextureformat(surface_format.format);

    // Create rendertargets from swap chain
    u32 imageCount = 0;
    result = (vkGetSwapchainImagesKHR(vk_ctx->device, vk_sc->m_swap_chain, &imageCount, nullptr));
    if (result != VK_SUCCESS) {
        return gal_error_code::ERR;
    }
    assert(imageCount >= desc->image_count);

    result =
        (vkGetSwapchainImagesKHR(vk_ctx->device, vk_sc->m_swap_chain, &imageCount, vk_sc->swap_chain_images.data()));
    if (result != VK_SUCCESS) {
        return gal_error_code::ERR;
    }

    gal_render_target_desc descColor = {};
    descColor.width = desc->width;
    descColor.height = desc->height;
    descColor.depth = 1;
    descColor.format = desc->format;
    descColor.clear_value = desc->clear_value;
    descColor.sample_count = gal_texture_sample_count::SAMPLE_COUNT_1;
    descColor.texture_sample_quality = 0;
    descColor.initial_state = gal_resource_state::PRESENT;
    descColor.dimension = gal_texture_dimension::_2D;
    // texture image are imported from swapchain
    descColor.flags |= gal_texture_flag::TEXTURE_CREATION_FLAG_IMPORT_BIT;

    // Populate the vk_image field and add the Vulkan texture objects
    for (u32 i = 0; i < imageCount; ++i) {
        descColor.native_handle = (void *)vk_sc->swap_chain_images[i];
        vk_create_render_target(context, &descColor, &vk_sc->get_render_targets()[i]);
    }

    *swap_chain = vk_sc;
    vk_sc->m_desc = *desc;
    return gal_error_code::SUC;
}
gal_error_code vk_destroy_swap_chain(gal_context context, gal_swap_chain swap_chain) {
    if (swap_chain != gal_null) {
        vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
        vk_swap_chain *vk_sc = reinterpret_cast<vk_swap_chain *>(swap_chain);

        auto &rts = vk_sc->get_render_targets();
        for (auto &rt : rts) {
            if (rt) {
                destroy_render_target(context, rt);
            }
        }

        vkDestroySwapchainKHR(vk_ctx->device, vk_sc->m_swap_chain, nullptr);
        vkDestroySurfaceKHR(vk_ctx->instance, vk_sc->m_surface, nullptr);

        // resources are freed automatically
        ante::memory::afree(vk_sc);
        swap_chain = gal_null;
    }
    return gal_error_code::SUC;
}

gal_error_code vk_create_shader_program(gal_context context, gal_shader_program_desc *desc,
                                        gal_shader_program *shader_program) {

    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vk_shader_program *vk_sp = ante::memory::alloc<ante::gal::vk_shader_program>(nullptr);
    if (!vk_sp) {
        return gal_error_code::ERR;
    }

    u32 stage_count = 0;
    VkResult result;
    auto create_shader_module = [&](compiled_shader *shader, shader_stage_index idx) {
        if (shader == nullptr) {
            return gal_error_code::SUC;
        }
        VkShaderModuleCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        create_info.pNext = nullptr;
        create_info.flags = 0;
        create_info.codeSize = shader->byte_code()->size();
        create_info.pCode = static_cast<const u32 *>(shader->byte_code()->data());
        result = vkCreateShaderModule(vk_ctx->device, &create_info, nullptr,
                                      &vk_sp->m_shader_modules[static_cast<u32>(idx)]);
        if (result != VK_SUCCESS) {
            return gal_error_code::ERR;
        }
        vk_sp->m_entrys[static_cast<u32>(idx)] = shader->entry();

        stage_count++;
        return gal_error_code::SUC;
    };

    create_shader_module(desc->shader_group->vert(), shader_stage_index::VERT);
    create_shader_module(desc->shader_group->hull(), shader_stage_index::TESC);
    create_shader_module(desc->shader_group->domain(), shader_stage_index::TESE);
    create_shader_module(desc->shader_group->geom(), shader_stage_index::GEOM);
    create_shader_module(desc->shader_group->frag(), shader_stage_index::FRAG);
    create_shader_module(desc->shader_group->comp(), shader_stage_index::COMP);

    vk_sp->m_stage_count = stage_count;

    //createPipelineReflection(stageReflections, stage_count, desc->pReflection);

    //addShaderDependencies(pShaderProgram, desc);
    vk_sp->m_desc = *desc;
    *shader_program = vk_sp;
    return gal_error_code::SUC;
}

gal_error_code vk_create_pipeline_cache(gal_context context, gal_pipeline_cache_desc *desc,
                                        gal_pipeline_cache *pipeline_cache) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vk_pipeline_cache *vk_pc = ante::memory::alloc<ante::gal::vk_pipeline_cache>(nullptr);
    if (!vk_pc) {
        return gal_error_code::ERR;
    }
    if (desc->data.data() == nullptr) {
        VkPipelineCacheCreateInfo ci{};
        ci.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        ci.initialDataSize = 0;
        ci.pInitialData = nullptr;
        VkResult result = vkCreatePipelineCache(vk_ctx->device, &ci, nullptr, &vk_pc->pipeline_cache);
        if (result != VK_SUCCESS || vk_pc->pipeline_cache == VK_NULL_HANDLE) {
            return gal_error_code::ERR;
        }
    } else {
        VkPipelineCacheCreateInfo ci{};
        ci.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        ci.initialDataSize = desc->data.size();
        ci.pInitialData = desc->data.data();
        VkResult result = vkCreatePipelineCache(vk_ctx->device, &ci, nullptr, &vk_pc->pipeline_cache);
        if (result != VK_SUCCESS || vk_pc->pipeline_cache == VK_NULL_HANDLE) {

            return gal_error_code::ERR;
        }
    }
    *pipeline_cache = vk_pc;
    return gal_error_code::SUC;
}
gal_error_code vk_destroy_pipeline_cache(gal_context context, gal_pipeline_cache _pipeline_cache) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    if (_pipeline_cache != gal_null) {
        vk_pipeline_cache *vk_pc = reinterpret_cast<vk_pipeline_cache *>(_pipeline_cache);
        vkDestroyPipelineCache(vk_ctx->device, vk_pc->pipeline_cache, nullptr);
        ante::memory::afree(vk_pc);
        _pipeline_cache = gal_null;
    }
    return gal_error_code::SUC;
}
gal_error_code vk_get_pipeline_cache_data(gal_context context, gal_pipeline_cache _pipeline_cache, u64 *_size,
                                          void *_data) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vk_pipeline_cache *vk_pc = reinterpret_cast<vk_pipeline_cache *>(_pipeline_cache);
    VkResult result = vkGetPipelineCacheData(vk_ctx->device, vk_pc->pipeline_cache, _size, _data);
    if (result != VK_SUCCESS) {
        return gal_error_code::ERR;
    }
    return gal_error_code::SUC;
}

gal_error_code vk_create_compute_pipeline(gal_context context, gal_pipeline_desc *desc, gal_pipeline *pipeline) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vk_pipeline *vk_pipe = ante::memory::alloc<ante::gal::vk_pipeline>(nullptr);

    if (!vk_pipe) {
        return gal_error_code::ERR;
    }

    gal_compute_pipeline_desc &comp_desc = std::get<gal_compute_pipeline_desc>(desc->desc);

    vk_shader_program *vk_sp = reinterpret_cast<vk_shader_program *>(comp_desc.shader);

    vk_rootsignature *vk_rs = reinterpret_cast<vk_rootsignature *>(comp_desc.root_signature);

    VkPipelineShaderStageCreateInfo shader_stage_create_info{};
    shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stage_create_info.flags = 0;
    shader_stage_create_info.stage = VK_SHADER_STAGE_COMPUTE_BIT;

    shader_stage_create_info.module = vk_sp->m_shader_modules[static_cast<u32>(shader_stage_index::COMP)];
    shader_stage_create_info.pName = vk_sp->m_entrys[static_cast<u32>(shader_stage_index::COMP)];
    shader_stage_create_info.pSpecializationInfo = nullptr;

    VkComputePipelineCreateInfo pipeline_create_info{};
    pipeline_create_info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipeline_create_info.flags = 0;
    pipeline_create_info.layout = vk_rs->pipeline_layout;
    pipeline_create_info.stage = shader_stage_create_info;

    VkPipelineCache pipeline_cache =
        desc->pipeline_cache ? reinterpret_cast<vk_pipeline_cache *>(desc->pipeline_cache)->pipeline_cache : nullptr;

    VkResult result =
        vkCreateComputePipelines(vk_ctx->device, pipeline_cache, 1, &pipeline_create_info, nullptr, &vk_pipe->pipeline);
    if (result != VK_SUCCESS || vk_pipe->pipeline == VK_NULL_HANDLE) {
        return gal_error_code::ERR;
    }
    *pipeline = vk_pipe;
    vk_pipe->m_desc = *desc;
    vk_pipe->m_type = gal_pipeline_type::COMPUTE;
    return gal_error_code::SUC;
}

gal_error_code vk_create_graphics_pipeline(gal_context context, gal_pipeline_desc *desc, gal_pipeline *pipeline) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vk_pipeline *vk_pipe = ante::memory::alloc<ante::gal::vk_pipeline>(nullptr);
    if (!vk_pipe) {
        return gal_error_code::ERR;
    }
    gal_graphics_pipeline_desc gfx_desc = std::get<gal_graphics_pipeline_desc>(desc->desc);

    vk_shader_program *vk_sp = reinterpret_cast<vk_shader_program *>(gfx_desc.shader);

    gal_vertex_layout *pVertexLayout = gfx_desc.pVertexLayout;
    u32 stage_count = 0;
    ante::fixed_array<VkPipelineShaderStageCreateInfo, 5> stages;

    gal_shader_program_desc *sp_desc = &vk_sp->m_desc;

    for (u32 i = 0; i < 5; ++i) {
        gal_shader_stage stage_mask = static_cast<gal_shader_stage>(1 << i);
        if (stage_mask == (sp_desc->shader_group->stages() & stage_mask)) {
            stages[stage_count].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            stages[stage_count].pNext = nullptr;
            stages[stage_count].flags = 0;
            stages[stage_count].pSpecializationInfo = nullptr;
            stages[stage_count].module = vk_sp->m_shader_modules[i];
            stages[stage_count].pName = vk_sp->m_entrys[i];
            switch (stage_mask) {
            case gal_shader_stage::VERT: {
                stages[stage_count].stage = VK_SHADER_STAGE_VERTEX_BIT;
            } break;
            case gal_shader_stage::TESC: {
                stages[stage_count].stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
            } break;
            case gal_shader_stage::TESE: {
                stages[stage_count].stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
            } break;
            case gal_shader_stage::GEOM: {
                stages[stage_count].stage = VK_SHADER_STAGE_GEOMETRY_BIT;
            } break;
            case gal_shader_stage::FRAG: {
                stages[stage_count].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            } break;
            default:
                return gal_error_code::ERR;
                // assert(false && "Shader Stage not supported!");
                break;
            }
            ++stage_count;
        }
    }

    // Make sure there's a shader
    if (stage_count == 0) {
        return gal_error_code::ERR;
    }
    u32 input_binding_count = 0;
    VkVertexInputBindingDescription input_bindings[MAX_VERTEX_BINDINGS]{};
    u32 input_attribute_count = 0;
    VkVertexInputAttributeDescription input_attributes[MAX_VERTEX_ATTRIBS]{};

    // Make sure there's attributes
    if (pVertexLayout != nullptr) {
        // Ignore everything that's beyond max_vertex_attribs
        u32 attrib_count =
            pVertexLayout->mAttribCount > MAX_VERTEX_ATTRIBS ? MAX_VERTEX_ATTRIBS : pVertexLayout->mAttribCount;
        u32 binding_value = UINT32_MAX;

        // Initial values
        for (u32 i = 0; i < attrib_count; ++i) {
            const gal_vertex_attrib *attrib = &(pVertexLayout->mAttribs[i]);

            if (binding_value != attrib->mBinding) {
                binding_value = attrib->mBinding;
                ++input_binding_count;
            }

            input_bindings[input_binding_count - 1].binding = binding_value;
            if (attrib->mRate == VERTEX_ATTRIB_RATE_INSTANCE) {
                input_bindings[input_binding_count - 1].inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
            } else {
                input_bindings[input_binding_count - 1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            }
            input_bindings[input_binding_count - 1].stride += gal_tf_bit_size_of_block(attrib->mFormat) / 8;

            input_attributes[input_attribute_count].location = attrib->mLocation;
            input_attributes[input_attribute_count].binding = attrib->mBinding;
            input_attributes[input_attribute_count].format = galtextureformat_to_vkformat(attrib->mFormat);
            input_attributes[input_attribute_count].offset = attrib->mOffset;
            ++input_attribute_count;
        }
    }

    VkPipelineVertexInputStateCreateInfo vi{};
    vi.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vi.pNext = nullptr;
    vi.flags = 0;
    vi.vertexBindingDescriptionCount = input_binding_count;
    vi.pVertexBindingDescriptions = input_bindings;
    vi.vertexAttributeDescriptionCount = input_attribute_count;
    vi.pVertexAttributeDescriptions = input_attributes;

    VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    switch (gfx_desc.mPrimitiveTopo) {
    case PrimitiveTopology::PRIMITIVE_TOPO_POINT_LIST:
        topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
        break;
    case PrimitiveTopology::PRIMITIVE_TOPO_LINE_LIST:
        topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
        break;
    case PrimitiveTopology::PRIMITIVE_TOPO_LINE_STRIP:
        topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
        break;
    case PrimitiveTopology::PRIMITIVE_TOPO_TRI_STRIP:
        topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
        break;
    case PrimitiveTopology::PRIMITIVE_TOPO_PATCH_LIST:
        topology = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
        break;
    case PrimitiveTopology::PRIMITIVE_TOPO_TRI_LIST:
        topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        break;
    default:
        return gal_error_code::ERR;
        //assert(false && "Primitive Topo not supported!");
        break;
    }
    VkPipelineInputAssemblyStateCreateInfo ia{};
    ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    ia.pNext = nullptr;
    ia.flags = 0;
    ia.topology = topology;
    ia.primitiveRestartEnable = VK_FALSE;

    VkPipelineTessellationStateCreateInfo ts{};
    if ((sp_desc->shader_group->stages() & gal_shader_stage::TESC) != gal_shader_stage::UNDEFINED &&
        (sp_desc->shader_group->stages() & gal_shader_stage::TESE) != gal_shader_stage::UNDEFINED) {
        ts.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
        ts.pNext = nullptr;
        ts.flags = 0;
        // TODO(reflection)
        //ts.patchControlPoints =
        //    pShaderProgram->pReflection->mStageReflections[pShaderProgram->pReflection->hullStageIndex]
        //        .mNumControlPoint;
    }

    VkPipelineViewportStateCreateInfo vs{};
    vs.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vs.pNext = nullptr;
    vs.flags = 0;
    // we are using dynamic viewports but we must set the count to 1
    vs.viewportCount = 1;
    vs.pViewports = nullptr;
    vs.scissorCount = 1;
    vs.pScissors = nullptr;
    VkPipelineMultisampleStateCreateInfo ms{};
    ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    ms.pNext = nullptr;
    ms.flags = 0;
    ms.rasterizationSamples = util_to_vk_sample_count(gfx_desc.mSampleCount);
    ms.sampleShadingEnable = VK_FALSE;
    ms.minSampleShading = 0.0f;
    ms.pSampleMask = 0;
    ms.alphaToCoverageEnable = VK_FALSE;
    ms.alphaToOneEnable = VK_FALSE;

    VkPipelineRasterizationStateCreateInfo rs;
    rs = gfx_desc.pRasterizerState ? util_to_rasterizer_desc(gfx_desc.pRasterizerState)
                                   : VkPipelineRasterizationStateCreateInfo{};

    /// TODO: Dont create depth state if no depth stencil bound
    VkPipelineDepthStencilStateCreateInfo ds = util_to_depth_desc(gfx_desc.pDepthState);

    VkPipelineColorBlendAttachmentState cbAtt[MAX_RENDER_TARGET_ATTACHMENTS];
    VkPipelineColorBlendStateCreateInfo cb = util_to_blend_desc(gfx_desc.pBlendState, cbAtt);
    cb.attachmentCount = gfx_desc.mRenderTargetCount;

    VkDynamicState dyn_states[] = {
        VK_DYNAMIC_STATE_VIEWPORT,     VK_DYNAMIC_STATE_SCISSOR,           VK_DYNAMIC_STATE_BLEND_CONSTANTS,
        VK_DYNAMIC_STATE_DEPTH_BOUNDS, VK_DYNAMIC_STATE_STENCIL_REFERENCE,
    };
    VkPipelineDynamicStateCreateInfo dy{};
    dy.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dy.pNext = nullptr;
    dy.flags = 0;
    dy.dynamicStateCount = sizeof(dyn_states) / sizeof(dyn_states[0]);
    dy.pDynamicStates = dyn_states;

    VkGraphicsPipelineCreateInfo add_info{};
    add_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    add_info.pNext = nullptr;
    add_info.flags = 0;
    add_info.stageCount = stage_count;
    add_info.pStages = stages.data();
    add_info.pVertexInputState = &vi;
    add_info.pInputAssemblyState = &ia;

    if ((sp_desc->shader_group->stages() & gal_shader_stage::TESC) != gal_shader_stage::UNDEFINED &&
        (sp_desc->shader_group->stages() & gal_shader_stage::TESE) != gal_shader_stage::UNDEFINED)
        add_info.pTessellationState = &ts;
    else
        add_info.pTessellationState = nullptr; // set tessellation state to null if we have no tessellation

    add_info.pViewportState = &vs;
    add_info.pRasterizationState = &rs;
    add_info.pMultisampleState = &ms;
    add_info.pDepthStencilState = &ds;
    add_info.pColorBlendState = &cb;
    add_info.pDynamicState = &dy;
    //add_info.layout = vk_sp.m_reflection->pipelien_layout;
    add_info.subpass = 0;

    VkPipelineCache pipeline_cache = (desc->pipeline_cache == gal_null)
                                         ? VK_NULL_HANDLE
                                         : reinterpret_cast<vk_pipeline_cache *>(desc->pipeline_cache)->pipeline_cache;

    VkResult result =
        (vkCreateGraphicsPipelines(vk_ctx->device, pipeline_cache, 1, &add_info, nullptr, &vk_pipe->pipeline));

    if (result != VK_SUCCESS || vk_pipe->pipeline == gal_null) {
        return gal_error_code::ERR;
    }
    *pipeline = vk_pipe;
    vk_pipe->m_desc = *desc;
    vk_pipe->m_type = gal_pipeline_type::COMPUTE;
    return gal_error_code::SUC;
}
//gal_error_code vk_create_raytracing_pipeline(gal_context context, gal_raytracing_pipeline_desc *desc,
//                                           gal_pipeline *pipeline) {
//    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
//    *pipeline = reinterpret_cast<gal_pipeline>(new ante::gal::vk_pipeline);
//    vk_pipeline *vk_spl = reinterpret_cast<vk_pipeline *>(*pipeline);
//    VkRayTracingPipelineCreateInfoKHR pipeline_create_info{};
//    pipeline_create_info.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR;
//    pipeline_create_info.
//}

gal_error_code vk_destroy_pipeline(gal_context context, gal_pipeline pipeline) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    if (pipeline != gal_null) {
        vk_pipeline *vk_pipe = reinterpret_cast<vk_pipeline *>(pipeline);
        vkDestroyPipeline(vk_ctx->device, vk_pipe->pipeline, nullptr);
        ante::memory::afree(vk_pipe);
        pipeline = gal_null;
    }
    return gal_error_code::SUC;
}

// we don't expose descriptor pool creation in gal
// A descriptor pool maintains a pool of descriptors, from which descriptor sets are allocated. Descriptor pools are externally synchronized, meaning that the application must not allocate and/or free descriptor sets from the same pool in multiple threads simultaneously.
gal_error_code vk_create_descriptor_pool(gal_context context, vk_descriptor_pool_desc *desc, VkDescriptorPool *pool) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    VkDescriptorPoolCreateInfo descriptor_pool_ci{};
    descriptor_pool_ci.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptor_pool_ci.pNext = nullptr;
    descriptor_pool_ci.poolSizeCount = desc->numPoolSizes;
    descriptor_pool_ci.pPoolSizes = desc->pPoolSizes;
    descriptor_pool_ci.flags = desc->flags;
    descriptor_pool_ci.maxSets = desc->numDescriptorSets;

    VkResult result = vkCreateDescriptorPool(vk_ctx->device, &descriptor_pool_ci, nullptr, pool);
    if (result != VK_SUCCESS) {
        return gal_error_code::ERR;
    }
    return gal_error_code::SUC;
}
gal_error_code vk_destroy_descriptor_pool(gal_context context, vk_descriptor_pool *pool) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vkDestroyDescriptorPool(vk_ctx->device, pool->pool, nullptr);
    return gal_error_code::SUC;
}

gal_error_code vk_get_descriptor_set(gal_context context, gal_descriptor_set_desc *desc, u32 set_count,
                                     gal_descriptor_set *sets) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vk_rootsignature *vk_rs = reinterpret_cast<vk_rootsignature *>(desc->root_signature);
    //vk_descriptor_set *vk_ds = ante::memory::alloc<ante::gal::vk_descriptor_set>(nullptr);

    u32 set_index = desc->set.index;
    if (set_index > MAX_DESCRIPTOR_SET_COUNT) {
        return gal_error_code::ERR;
    }
    if (!vk_rs->set_layouts[set_index]) {
        return gal_error_code::ERR;
    }
    vk_descriptor_set *vk_ds =
        (ante::gal::vk_descriptor_set *)ante::memory::amalloc(set_count * sizeof(ante::gal::vk_descriptor_set));
    if (!vk_ds) {
        return gal_error_code::ERR;
    }

    ACQUIRE_STACK_MEMORY_RESOURCE(stack_memory, 2048);
    ante::vector<VkDescriptorSet> vk_sets(&stack_memory);
    for (u32 i = 0; i < set_count; i++) {
        vk_sets.push_back(vk_ds[i].set);
    }
    ante::vector<VkDescriptorSetLayout> set_layouts(&stack_memory);
    set_layouts.resize(set_count);
    std::fill(set_layouts.begin(), set_layouts.end(), vk_rs->set_layouts[desc->set.index]);
    vk_descriptor_pool_desc dp_desc{};
    dp_desc.numDescriptorSets = set_count;
    dp_desc.flags =
        VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT; // TODO(hyl5): https://community.arm.com/arm-community-blogs/b/graphics-gaming-and-vr-blog/posts/vulkan-descriptor-and-buffer-management
    if (set_index == static_cast<u32>(gal_descriptor_set_update_freq::BINDLESS)) {
        dp_desc.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;
    }
    dp_desc.pPoolSizes = vk_rs->descriptor_pool_size[set_index].pool_sizes;
    dp_desc.numPoolSizes = vk_rs->descriptor_pool_size[set_index].pool_size_count;
    vk_descriptor_pool *vk_dp = ante::memory::alloc<vk_descriptor_pool>(nullptr);

    VkDescriptorPool pool = VK_NULL_HANDLE;
    gal_error_code res = vk_create_descriptor_pool(context, &dp_desc, &pool);
    if (res != gal_error_code::SUC) {
        return gal_error_code::ERR;
    }
    VkDescriptorSetAllocateInfo ds_ai{};
    ds_ai.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    ds_ai.pNext = nullptr;
    ds_ai.descriptorPool = pool;
    ds_ai.descriptorSetCount = set_count;
    ds_ai.pSetLayouts = set_layouts.data();
    VkResult result = vkAllocateDescriptorSets(vk_ctx->device, &ds_ai, vk_sets.data());
    if (result != VK_SUCCESS) {
        return gal_error_code::ERR;
    }

    vk_dp->pool = pool;
    vk_dp->ref_count = set_count;
    vk_dp->root_signature = vk_rs;

    for (u32 i = 0; i < set_count; i++) {
        vk_ds[i].set = vk_sets[i];
        vk_ds[i].pool = vk_dp;
        vk_ds[i].set_index = set_index;
        sets[i] = &vk_ds[i];
    }
    return gal_error_code::SUC;
}
gal_error_code vk_free_descriptor_set(gal_context context, gal_descriptor_set set, bool free_all_pool) {

    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vk_descriptor_set *vk_ds = reinterpret_cast<vk_descriptor_set *>(set);

    // already freed
    if (vk_ds->pool->ref_count == 0 || vk_ds->pool == nullptr) {
        return gal_error_code::ERR;
    }

    if (free_all_pool || --vk_ds->pool->ref_count == 0) {
        vk_destroy_descriptor_pool(vk_ctx, vk_ds->pool);
        ante::memory::afree(vk_ds->pool);
        vk_ds->pool = nullptr;
    } else {
        VkResult result = vkFreeDescriptorSets(vk_ctx->device, vk_ds->pool->pool, 1, &vk_ds->set);
        if (result != VK_SUCCESS) {
            return gal_error_code::ERR;
        }
    }

    return gal_error_code::SUC;
}

gal_error_code vk_update_descriptor_set(gal_context context, gal_descriptor_set_update_desc *update_desc,
                                        gal_descriptor_set set) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vk_descriptor_set *vk_ds = reinterpret_cast<vk_descriptor_set *>(set);

    ACQUIRE_STACK_MEMORY_RESOURCE(stack_memory, 1024);
    ante::vector<VkDescriptorUpdateData> descriptor_updates(&stack_memory);
    descriptor_updates.resize(update_desc->count);
    for (u32 i = 0; i < update_desc->count; i++) {
        VkDescriptorUpdateData update_data{};
        gal_descriptor_upate_desc &t_update_data = update_desc->updates[i];

        vk_rootsignature *vk_rs = reinterpret_cast<vk_rootsignature *>(vk_ds->pool->root_signature);
        auto iter = vk_rs->resource_map.find(t_update_data.name);
        if (iter == vk_rs->resource_map.end()) {
            return gal_error_code::ERR;
        }
        u32 set_bind_bind = iter->second;
        u32 bind_order = (set_bind_bind & 0x7E0000) >> 17;
        //u32 bind_index = (set_bind_bind & (64 << 23));
        //u32 set_index = (set_bind_bind >> 29) & 0xF;
        u32 set_index = (set_bind_bind & 0xE0000000) >> 29;
        if (set_index != vk_ds->set_index) {
            return gal_error_code::ERR;
        }
        switch (update_desc->updates[i].type) {
        case gal_descriptor_type::CONSTANT_BUFFER:
        case gal_descriptor_type::RW_BUFFER:
            update_data.buffer_info.buffer = reinterpret_cast<vk_buffer *>(t_update_data.desc.b.buffer)->m_buffer;
            update_data.buffer_info.offset = 0;
            update_data.buffer_info.range = VK_WHOLE_SIZE;
            break;
        // case gal_descriptor_type::TEXEL_BUFFER: TODO(hyl5):
        case gal_descriptor_type::TEXTURE: {
            vk_texture *vk_tex = reinterpret_cast<vk_texture *>(t_update_data.desc.t.texture);
            update_data.image_info.imageLayout = util_to_vk_image_layout(gal_resource_state::TEXTURE);
            update_data.image_info.imageView = vk_tex->m_view;
        } break;
        case gal_descriptor_type::RW_TEXTURE: {

            vk_texture *vk_tex = reinterpret_cast<vk_texture *>(t_update_data.desc.t.texture);
            update_data.image_info.imageLayout = util_to_vk_image_layout(gal_resource_state::RW_TEXTURE);
            update_data.image_info.imageView = vk_tex->m_view;
        } break;
        case gal_descriptor_type::SAMPLER:
            update_data.image_info.sampler = reinterpret_cast<vk_sampler *>(t_update_data.desc.s.sampler)->m_sampler;
            break;
        default:
            return gal_error_code::ERR;
        }
        descriptor_updates[bind_order] = std::move(update_data);
    }

    vkUpdateDescriptorSetWithTemplate(vk_ctx->device, vk_ds->set,
                                      reinterpret_cast<vk_rootsignature *>(vk_ds->pool->root_signature)
                                          ->descriptor_set_update_template[vk_ds->set_index],
                                      descriptor_updates.data());
    return gal_error_code::SUC;
}

gal_error_code vk_create_rootsignature(gal_context context, gal_rootsignature_desc *desc,
                                       gal_rootsignature *root_signature) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vk_rootsignature *vk_rs = ante::memory::alloc<ante::gal::vk_rootsignature>(nullptr);
    if (!vk_rs) {
        return gal_error_code::ERR;
    }
    // manually zeroing memory for safety
    std::memset(vk_rs->set_layouts.data(), (int)nullptr, sizeof(VkDescriptorSetLayout) * vk_rs->set_layouts.size());

    const auto &refl = desc->shader->reflection();
    u32 set_count = static_cast<u32>(refl->sets.size());
    // TODO(hyl5): seperate descriptor set layouts for caching

    ACQUIRE_STACK_MEMORY_RESOURCE(stack_memory, 1024);
    if (set_count > MAX_DESCRIPTOR_SET_COUNT) {
        LOG_ERROR("maximum of descriptor set count is: {}, current is: {}", MAX_DESCRIPTOR_SET_COUNT, set_count);
        return gal_error_code::ERR;
    }

    ante::fixed_array<VkDescriptorSetLayoutCreateInfo, MAX_DESCRIPTOR_SET_COUNT> set_layouts_ci;
    //ante::fixed_array<u32, MAX_DESCRIPTOR_SET_COUNT> set_binding_offsets;
    ante::vector<VkDescriptorSetLayoutBinding> bindings(&stack_memory);
    //ante::vector<u32> binding_prefix_sum(refl->sets.size(), &stack_memory);
    ante::vector<u32> binding_offsets(refl->sets.size(), &stack_memory);
    u32 total_binding_count = 0;
    ante::map<u32, u32> set_index_map(&stack_memory);
    for (u32 i = 0; i < refl->sets.size(); i++) {
        u32 set_index = (refl->sets[i] >> 16) & 0x0000ffff; // first 16bit
        u32 set_binding_count = refl->sets[i] & 0x0000ffff; // last 16bit

        set_index_map.insert({set_index, i});
        binding_offsets[i] = total_binding_count;
        total_binding_count += set_binding_count;
        //binding_prefix_sum[i] = set_binding_count;
    }

    bindings.resize(total_binding_count);

    ante::vector<VkPushConstantRange> push_constants(&stack_memory);
    VkShaderStageFlags stages = util_to_vk_shader_stage_flags(desc->shader->stages());

    // iterate all bindings and push constants
    for (auto &resource : refl->m_resources) {
        if (resource.resource_type == ShaderResourceType::RESOURCE) {
            VkDescriptorSetLayoutBinding binding{};
            binding.binding = resource.reg;
            binding.descriptorCount = resource.array_size;
            binding.descriptorType = utils_to_vk_descriptor_type(resource.descriptor_type);
            binding.stageFlags = stages;
            bindings[binding_offsets[set_index_map[resource.set]]] = std::move(binding);
            binding_offsets[set_index_map[resource.set]]++;
        } else if (resource.resource_type == ShaderResourceType::PUSH_CONSTANT) {
            push_constants.emplace_back(VkPushConstantRange{stages, 0, 0});
        }
    }

    ante::fixed_array<u32, MAX_DESCRIPTOR_SET_COUNT> set_binding_offsets{};
    for (auto &resource : refl->m_resources) {
        vk_rs->resource_map[resource.name.c_str()] =
            (resource.set << 29) | (resource.reg << 23) |
            (set_binding_offsets[resource.set]++ << 17); // TODO(hyl5): remove hardcoded number

    }
    // binding offsets is prefix sum now

    VkResult result = VK_SUCCESS;

    for (u32 i = 0; i < set_count; i++) {
        u32 set_index = (refl->sets[i] >> 16) & 0x0000ffff; // first 16bit
        VkDescriptorSetLayoutCreateInfo &ci = set_layouts_ci[set_index];
        ci.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        ci.pNext = nullptr;
        ci.flags = 0;
        ci.bindingCount = (i == 0) ? binding_offsets[i] : (binding_offsets[i] - binding_offsets[i - 1]);
        ci.pBindings = bindings.data() + ((i == 0) ? 0 : binding_offsets[i - 1]);
        result = vkCreateDescriptorSetLayout(vk_ctx->device, &ci, nullptr, &vk_rs->set_layouts[set_index]);
        if (result != VK_SUCCESS) {
            return gal_error_code::ERR;
        }
    }

    // FIXME(hyl5): too many nested containers here
    ante::vector<ante::hash_map<VkDescriptorType, u32>> pool_sizes(&stack_memory);
    pool_sizes.resize(refl->sets.size());
    // fill descriptor_pool sizes;
    for (auto &resource : refl->m_resources) {
        if (resource.resource_type == ShaderResourceType::RESOURCE) {
            pool_sizes[set_index_map[resource.set]][utils_to_vk_descriptor_type(resource.descriptor_type)]++;
        }
    }

    for (auto &set : refl->sets) {
        u32 set_index = (set >> 16) & 0x0000ffff; // first 16bit
        // TODO(hyl5): don't forget to free
        VkDescriptorPoolSize *vk_pool_sizes = static_cast<VkDescriptorPoolSize *>(
            ante::memory::amalloc(pool_sizes[set_index_map[set_index]].size() * sizeof(VkDescriptorPoolSize)));
        VkDescriptorPoolSize *p = vk_pool_sizes;
        for (auto &size : pool_sizes[set_index_map[set_index]]) {
            p->type = size.first;
            p->descriptorCount = size.second;
            p++;
        }
        vk_rs->descriptor_pool_size[set_index].pool_size_count =
            static_cast<u32>(pool_sizes[set_index_map[set_index]].size());
        vk_rs->descriptor_pool_size[set_index].pool_sizes = vk_pool_sizes;
    }
    ante::fixed_array<VkDescriptorSetLayout, MAX_DESCRIPTOR_SET_COUNT> layouts;
    {
        int i = 0;
        for (auto &layout : vk_rs->set_layouts) {
            if (layout != VK_NULL_HANDLE) {
                layouts[i++] = layout;
            }
        }
    }

    VkPipelineLayoutCreateInfo plci{};
    plci.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    plci.pNext = nullptr;
    plci.flags = 0;
    plci.setLayoutCount = set_count;
    plci.pSetLayouts = layouts.data();
    plci.pushConstantRangeCount = static_cast<u32>(push_constants.size());
    plci.pPushConstantRanges = push_constants.data();

    result = vkCreatePipelineLayout(vk_ctx->device, &plci, nullptr, &vk_rs->pipeline_layout);
    if (result != VK_SUCCESS) {
        return gal_error_code::ERR;
    }

    // https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/vkUpdateDescriptorSetWithTemplate.html
    //ante::vector<VkDescriptorUpdateTemplateEntry> descriptor_update_template_entries(&stack_memory);
    //descriptor_update_template_entries.resize(total_binding_count);
    ante::vector<ante::vector<VkDescriptorUpdateTemplateEntry>> descriptor_update_template_entries(&stack_memory);
    descriptor_update_template_entries.resize(refl->sets.size());
    u32 offsets = 0;

    //auto get_offset = [](VkDescriptorType type) -> u32 {
    //    return sizeof(VkDescriptorUpdateData);
    //    //if (type == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER || type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
    //    //    return sizeof(VkDescriptorBufferInfo);
    //    //} else if (type == VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE || type == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE ||
    //    //           type == VK_DESCRIPTOR_TYPE_SAMPLER) {
    //    //    return sizeof(VkDescriptorImageInfo);
    //    //} else {
    //    //    return 0;
    //    //}
    //};


    set_binding_offsets.fill(0); // reset the arr
    for (auto &resource : refl->m_resources) {
        if (resource.resource_type == ShaderResourceType::RESOURCE) {
            u32 set_order = set_index_map[resource.set];

            VkDescriptorUpdateTemplateEntry entry{};

            entry.dstBinding =
                resource.reg;
            entry.descriptorType =
                utils_to_vk_descriptor_type(resource.descriptor_type);
            entry.descriptorCount = resource.array_size;
            entry.dstArrayElement = 0;
            entry.offset = set_binding_offsets[set_order]++;
            entry.stride =0; //bindings[i].descriptorCount == 1 ? 0 : bindings[i].binding;
            offsets += sizeof(VkDescriptorUpdateData); // more memory footprint but more flexible

            descriptor_update_template_entries[set_order].push_back(std::move(entry));
        }
    }
    for (u32 i = 0; i < refl->sets.size(); i++) {
        u32 set_index = (refl->sets[i] >> 16) & 0x0000ffff; // first 16bit

        VkDescriptorUpdateTemplateCreateInfo dsut_ci{};

        dsut_ci.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_CREATE_INFO;
        dsut_ci.pNext = nullptr;
        dsut_ci.flags = 0;
        dsut_ci.templateType = VK_DESCRIPTOR_UPDATE_TEMPLATE_TYPE_DESCRIPTOR_SET;
        dsut_ci.descriptorUpdateEntryCount =
            (i == 0) ? binding_offsets[i] : (binding_offsets[i] - binding_offsets[i - 1]);
        dsut_ci.pDescriptorUpdateEntries = descriptor_update_template_entries[set_index_map[set_index]].data();
           // descriptor_update_template_entries.data() + ((i == 0) ? 0 : binding_offsets[i - 1]);
        dsut_ci.descriptorSetLayout = vk_rs->set_layouts[set_index];

        dsut_ci.pipelineBindPoint = utils_to_vk_pipeline_bind_point(desc->type); // ignored by given templateType
        dsut_ci.pipelineLayout = vk_rs->pipeline_layout;                         // ignored by given templateType
        dsut_ci.set = set_index;                                                 // ignored by given templateType

        result = vkCreateDescriptorUpdateTemplate(vk_ctx->device, &dsut_ci, nullptr,
                                                  &vk_rs->descriptor_set_update_template[set_index]);
        if (result != VK_SUCCESS) {
            return gal_error_code::ERR;
        }
    }

    *root_signature = vk_rs;

    return gal_error_code::SUC;
}
gal_error_code vk_destroy_rootsignature(gal_context context, gal_rootsignature root_signature) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    if (root_signature != gal_null) {
        vk_rootsignature *vk_rs = reinterpret_cast<vk_rootsignature *>(root_signature);
        vkDestroyPipelineLayout(vk_ctx->device, vk_rs->pipeline_layout, nullptr);
        ante::memory::afree(vk_rs);
        root_signature = gal_null;
    }
    return gal_error_code::SUC;
}

// sync
gal_error_code vk_create_fence(gal_context context, gal_fence *fence) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vk_fence *vk_f = ante::memory::alloc<vk_fence>(nullptr);
    if (!vk_f) {
        return gal_error_code::ERR;
    }

    VkFenceCreateInfo fence_ci{};
    fence_ci.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_ci.pNext = NULL;
    fence_ci.flags = 0;
    VkResult result = vkCreateFence(vk_ctx->device, &fence_ci, nullptr, &vk_f->fence);
    if (result != VK_SUCCESS) {
        return gal_error_code::ERR;
    }
    vk_f->b_submitted = false;
    *fence = vk_f;
    return gal_error_code::SUC;
}
gal_error_code vk_wait_fences(gal_context context, gal_fence *fences, u32 count) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);

    ACQUIRE_STACK_MEMORY_RESOURCE(stack_memory, 64);
    ante::vector<VkFence> submit_fences(&stack_memory);
    for (u32 i = 0; i < count; ++i) {
        if (reinterpret_cast<vk_fence *>(fences[i])->b_submitted == true) {
            submit_fences.push_back(reinterpret_cast<vk_fence *>(fences[i])->fence);
        }
    }

    VkResult result;
    if (!submit_fences.empty()) {
        result = vkWaitForFences(vk_ctx->device, static_cast<u32>(submit_fences.size()), submit_fences.data(), VK_TRUE,
                                 UINT64_MAX);
        if (result != VK_SUCCESS) {
            return gal_error_code::ERR;
        }
        result = vkResetFences(vk_ctx->device, static_cast<u32>(submit_fences.size()), submit_fences.data());
        if (result != VK_SUCCESS) {
            return gal_error_code::ERR;
        }
    }

    for (u32 i = 0; i < count; ++i) {
        reinterpret_cast<vk_fence *>(fences[i])->b_submitted = false;
    }
    return gal_error_code::SUC;
}

gal_error_code vk_destroy_fence(gal_context context, gal_fence fence) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    if (fence != gal_null) {
        vk_fence *vk_f = reinterpret_cast<vk_fence *>(fence);
        vkDestroyFence(vk_ctx->device, vk_f->fence, nullptr);
        ante::memory::afree(vk_f);
        fence = gal_null;
    }
    return gal_error_code::SUC;
}
//gal_error_code vk_wait_gpu() {
//    //vkDeviceWaitIdle();
//    return gal_error_code::SUC;
//}
gal_error_code vk_create_semaphore(gal_context context, gal_semaphore *semaphore) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vk_semaphore *vk_s = ante::memory::alloc<vk_semaphore>(nullptr);
    if (!vk_s) {
        return gal_error_code::ERR;
    }

    VkSemaphoreCreateInfo semaphore_ci{};
    semaphore_ci.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphore_ci.pNext = NULL;
    semaphore_ci.flags = 0;
    VkResult result = vkCreateSemaphore(vk_ctx->device, &semaphore_ci, nullptr, &vk_s->semaphore);
    if (result != VK_SUCCESS) {
        return gal_error_code::ERR;
    }
    // Set signal initial state.
    vk_s->b_signaled = false;

    *semaphore = vk_s;
    return gal_error_code::SUC;
}

gal_error_code vk_destroy_semaphore(gal_context context, gal_semaphore semaphore) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    if (semaphore != gal_null) {
        vk_semaphore *vk_s = reinterpret_cast<vk_semaphore *>(semaphore);
        vkDestroySemaphore(vk_ctx->device, vk_s->semaphore, nullptr);
        ante::memory::afree(vk_s);
        semaphore = gal_null;
    }
    return gal_error_code::SUC;
}
// cmds
gal_error_code vk_create_command_pool(gal_context context, gal_command_pool_desc *desc,
                                      gal_command_pool *command_pool) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vk_command_pool *vk_cmd_pool = ante::memory::alloc<ante::gal::vk_command_pool>(nullptr);
    if (!vk_cmd_pool) {
        return gal_error_code::ERR;
    }
    VkCommandPoolCreateInfo ci{};
    ci.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    ci.pNext = nullptr;
    ci.flags = 0;

    ci.queueFamilyIndex = utils_to_vk_queue_family_index(desc->queue_type, vk_ctx);
    if (desc->b_transient) {
        ci.flags |= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    }

    VkResult result = vkCreateCommandPool(vk_ctx->device, &ci, nullptr, &vk_cmd_pool->m_cmd_pool);
    if (result != VK_SUCCESS || vk_cmd_pool->m_cmd_pool == VK_NULL_HANDLE) {
        return gal_error_code::ERR;
    }
    if (vk_ctx->queues[utils_to_vk_queue_index(desc->queue_type)].used == false) {
        return gal_error_code::ERR;
    }
    vk_cmd_pool->queue = &vk_ctx->queues[utils_to_vk_queue_index(desc->queue_type)];
    *command_pool = vk_cmd_pool;
    return gal_error_code::SUC;
}
gal_error_code vk_reset_command_pool(gal_context context, gal_command_pool command_pool) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vk_command_pool *vk_cmd_pool = reinterpret_cast<vk_command_pool *>(command_pool);
    VkResult result = vkResetCommandPool(vk_ctx->device, vk_cmd_pool->m_cmd_pool, 0);
    if (result != VK_SUCCESS) {
        return gal_error_code::ERR;
    }
    return gal_error_code::SUC;
}
gal_error_code vk_destroy_command_pool(gal_context context, gal_command_pool command_pool) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    if (command_pool != gal_null) {
        vk_command_pool *vk_cmd_pool = reinterpret_cast<vk_command_pool *>(command_pool);
        vkDestroyCommandPool(vk_ctx->device, vk_cmd_pool->m_cmd_pool, nullptr);
        ante::memory::afree(vk_cmd_pool);
        command_pool = gal_null;
    }
    return gal_error_code::SUC;
}

gal_error_code vk_get_command_list(gal_context context, gal_command_list_desc *desc, gal_command_list *command) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vk_command_list *vk_cmd = ante::memory::alloc<ante::gal::vk_command_list>(nullptr);
    vk_command_pool *vk_pool = reinterpret_cast<vk_command_pool *>(desc->command_pool);
    if (desc->queue_type != vk_pool->queue->m_type) {
        return gal_error_code::ERR;
    }
    VkCommandBufferAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.pNext = nullptr;
    alloc_info.commandPool = vk_pool->m_cmd_pool;
    alloc_info.level = desc->b_secondary ? VK_COMMAND_BUFFER_LEVEL_SECONDARY : VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = 1;
    VkResult result = vkAllocateCommandBuffers(vk_ctx->device, &alloc_info, &vk_cmd->m_command);
    if (result != VK_SUCCESS || vk_cmd->m_command == gal_null) {
        return gal_error_code::ERR;
    }
    vk_cmd->m_cmd_pool = vk_pool;
    *command = vk_cmd;
    return gal_error_code::SUC;
}

gal_error_code vk_free_command_list(gal_context context, gal_command_list command) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);

    if (command != gal_null) {
        vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);
        vkFreeCommandBuffers(vk_ctx->device, reinterpret_cast<vk_command_pool *>(vk_cmd->m_cmd_pool)->m_cmd_pool, 1,
                             &vk_cmd->m_command);
        ante::memory::afree(vk_cmd);
        command = gal_null;
    }
    return gal_error_code::SUC;
}

gal_error_code vk_mapbuffer(gal_context context, gal_buffer buffer, read_range *range) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vk_buffer *vk_buf = reinterpret_cast<vk_buffer *>(buffer);
    if (((buffer->m_gal_buffer_desc.memory_flags & gal_memory_flag::CPU_UPLOAD) == gal_memory_flag::UNDEFINED &&
         (buffer->m_gal_buffer_desc.memory_flags & gal_memory_flag::GPU_DOWNLOAD) == gal_memory_flag::UNDEFINED) ||
        (buffer->m_gal_buffer_desc.memory_flags & gal_memory_flag::GPU_DEDICATED) == gal_memory_flag::GPU_DEDICATED) {
        return gal_error_code::ERR;
    }

    if (range) {
        vk_buf->cpu_mapped_address = static_cast<u8 *>(vk_buf->cpu_mapped_address) + range->offset;
    }

    VkResult result = vmaMapMemory(vk_ctx->vma_allocator, vk_buf->m_allocation, &vk_buf->cpu_mapped_address);
    if (result != VK_SUCCESS) {
        return gal_error_code::ERR;
    }
    return gal_error_code::SUC;
}

gal_error_code vk_unmapbuffer(gal_context context, gal_buffer buffer) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vk_buffer *vk_buf = reinterpret_cast<vk_buffer *>(buffer);
    if (((buffer->m_gal_buffer_desc.memory_flags & gal_memory_flag::CPU_UPLOAD) == gal_memory_flag::UNDEFINED &&
         (buffer->m_gal_buffer_desc.memory_flags & gal_memory_flag::GPU_DOWNLOAD) == gal_memory_flag::UNDEFINED) ||
        (buffer->m_gal_buffer_desc.memory_flags & gal_memory_flag::GPU_DEDICATED) == gal_memory_flag::GPU_DEDICATED) {
        return gal_error_code::ERR;
    }
    vmaUnmapMemory(vk_ctx->vma_allocator, vk_buf->m_allocation);
    vk_buf->cpu_mapped_address = nullptr;
    return gal_error_code::SUC;
}

// TODO(hylu) : gal_error_code vk_create_command_list;

gal_error_code vk_cmd_begin(gal_command_list command) {
    vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);
    VkCommandBufferBeginInfo begin_info{};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.pNext = nullptr;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    begin_info.pInheritanceInfo = nullptr;

    VkResult result = vkBeginCommandBuffer(vk_cmd->m_command, &begin_info);
    if (result != VK_SUCCESS) {
        return gal_error_code::ERR;
    }
    return gal_error_code::SUC;
}

gal_error_code vk_cmd_end(gal_command_list command) {
    vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);

    if (vk_cmd->b_render_pass_bound) {
        vkCmdEndRendering(vk_cmd->m_command);
        vk_cmd->b_render_pass_bound = false;
    }
    VkResult result = vkEndCommandBuffer(vk_cmd->m_command);
    if (result != VK_SUCCESS) {
        return gal_error_code::ERR;
    }
    return gal_error_code::SUC;
}
gal_error_code vk_cmd_set_render_target() { return gal_error_code::SUC; }
gal_error_code vk_cmd_set_viewport(gal_command_list command, float x, float y, float width, float height,
                                   float minDepth, float maxDepth) {
    vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);

    VkViewport viewport{};
    viewport.x = x;
    viewport.y = y + height;
    viewport.width = width;
    viewport.height = -height;
    viewport.minDepth = minDepth;
    viewport.maxDepth = maxDepth;
    vkCmdSetViewport(vk_cmd->m_command, 0, 1, &viewport);
    return gal_error_code::SUC;
}

gal_error_code vk_cmdSetScissor(gal_command_list command, u32 x, u32 y, u32 width, u32 height) {
    vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);

    VkRect2D rect{};
    rect.offset.x = x;
    rect.offset.y = y;
    rect.extent.width = width;
    rect.extent.height = height;
    vkCmdSetScissor(vk_cmd->m_command, 0, 1, &rect);
    return gal_error_code::SUC;
}

gal_error_code vk_cmdSetStencilReferenceValue(gal_command_list command, u32 val) {
    vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);

    vkCmdSetStencilReference(vk_cmd->m_command, VK_STENCIL_FRONT_AND_BACK, val);
    return gal_error_code::SUC;
}

gal_error_code vk_cmd_resource_barrier(gal_command_list command, u32 buffer_barrier_count,
                                       gal_buffer_barrier *buffer_barriers, u32 texture_barrier_count,
                                       gal_texture_barrier *texture_barriers) {
    vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);

    VkAccessFlags srcAccessFlags = 0;
    VkAccessFlags dstAccessFlags = 0;
    ACQUIRE_STACK_MEMORY_RESOURCE(stack_memory, 256);
    ante::vector<VkBufferMemoryBarrier> bbs(&stack_memory);

    for (u32 i = 0; i < buffer_barrier_count; i++) {
        gal_buffer_barrier *buffer_barrier = &buffer_barriers[i];
        vk_buffer *vk_b = reinterpret_cast<vk_buffer *>(buffer_barrier->buffer);
        VkBufferMemoryBarrier b{};
        b.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
        b.pNext = nullptr;
        b.buffer = vk_b->m_buffer;
        if (gal_resource_state::RW_BUFFER == buffer_barrier->src_state &&
            gal_resource_state::RW_BUFFER == buffer_barrier->dst_state) {
            b.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT;
            b.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT;
        } else {
            b.srcAccessMask = utils_to_vk_access_flags(buffer_barrier->src_state);
            b.dstAccessMask = utils_to_vk_access_flags(buffer_barrier->dst_state);
        }
        b.size = VK_WHOLE_SIZE;
        b.offset = 0; // FIXME(hyl5): more options?

        if (buffer_barrier->op == queue_op::ACQUIRE) {
            b.srcQueueFamilyIndex = reinterpret_cast<vk_queue *>(buffer_barrier->target_queue)->queue_family_index;
            b.dstQueueFamilyIndex = reinterpret_cast<vk_queue *>(vk_cmd->m_cmd_pool->queue)->queue_family_index;
        }
        if (buffer_barrier->op == queue_op::RELEASE) {
            b.dstAccessMask = reinterpret_cast<vk_queue *>(buffer_barrier->target_queue)->queue_family_index;
            b.srcAccessMask = reinterpret_cast<vk_queue *>(vk_cmd->m_cmd_pool->queue)->queue_family_index;
        }

        srcAccessFlags |= b.srcAccessMask;
        dstAccessFlags |= b.dstAccessMask;

        bbs.push_back(b);
    }

    ante::vector<VkImageMemoryBarrier> tbs(&stack_memory);

    for (u32 i = 0; i < texture_barrier_count; i++) {
        gal_texture_barrier *texture_barrier = &texture_barriers[i];
        vk_texture *vk_t = reinterpret_cast<vk_texture *>(texture_barrier->texture);

        VkImageMemoryBarrier t{};
        t.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        t.pNext = nullptr;
        t.image = vk_t->m_image;
        if (gal_resource_state::RW_TEXTURE == texture_barrier->src_state &&
            gal_resource_state::RW_TEXTURE == texture_barrier->dst_state) {

            t.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
            t.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT;
            t.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
            t.newLayout = VK_IMAGE_LAYOUT_GENERAL;
        } else {

            t.srcAccessMask = utils_to_vk_access_flags(texture_barrier->src_state);
            t.dstAccessMask = utils_to_vk_access_flags(texture_barrier->dst_state);
            t.oldLayout = util_to_vk_image_layout(texture_barrier->src_state);
            t.newLayout = util_to_vk_image_layout(texture_barrier->dst_state);
        }

        t.subresourceRange.aspectMask = utils_to_vk_aspect_mask(vk_t->m_desc.format, false);
        t.subresourceRange.baseMipLevel = texture_barrier->mSubresourceBarrier ? texture_barrier->mMipLevel : 0;
        t.subresourceRange.levelCount = texture_barrier->mSubresourceBarrier ? 1 : VK_REMAINING_MIP_LEVELS;
        t.subresourceRange.baseArrayLayer = texture_barrier->mSubresourceBarrier ? texture_barrier->mArrayLayer : 0;
        t.subresourceRange.layerCount = texture_barrier->mSubresourceBarrier ? 1 : VK_REMAINING_ARRAY_LAYERS;
        if (texture_barrier->op == queue_op::ACQUIRE && texture_barrier->src_state != gal_resource_state::UNDEFINIED) {
            t.srcQueueFamilyIndex = reinterpret_cast<vk_queue *>(texture_barrier->target_queue)->queue_family_index;
            t.dstQueueFamilyIndex =
                reinterpret_cast<vk_queue *>(reinterpret_cast<vk_command_list *>(command)->m_cmd_pool->queue)
                    ->queue_family_index;
        }
        if (texture_barrier->op == queue_op::RELEASE && texture_barrier->src_state != gal_resource_state::UNDEFINIED) {
            t.dstAccessMask = reinterpret_cast<vk_queue *>(texture_barrier->target_queue)->queue_family_index;
            t.srcAccessMask =
                reinterpret_cast<vk_queue *>(reinterpret_cast<vk_command_list *>(command)->m_cmd_pool->queue)
                    ->queue_family_index;
        }

        srcAccessFlags |= t.srcAccessMask;
        dstAccessFlags |= t.dstAccessMask;
        tbs.push_back(t);
    }

    // Commit barriers
    VkPipelineStageFlags srcStageMask =
        util_determine_pipeline_stage_flags(srcAccessFlags, vk_cmd->m_cmd_pool->queue->m_type);
    VkPipelineStageFlags dstStageMask =
        util_determine_pipeline_stage_flags(dstAccessFlags, vk_cmd->m_cmd_pool->queue->m_type);
    if (!bbs.empty() || !tbs.empty()) {
        vkCmdPipelineBarrier(vk_cmd->m_command, srcStageMask, dstStageMask, 0, 0, nullptr, static_cast<u32>(bbs.size()),
                             bbs.data(), static_cast<u32>(tbs.size()), tbs.data());
    }
    return gal_error_code::SUC;
}

gal_error_code vk_cmd_bind_index_buffer(gal_command_list command, gal_buffer index_buffer, gal_index_type index_type,
                                        u64 offset) {
    vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);
    VkIndexType vk_index_type = utils_to_vk_index_type(index_type);
    vkCmdBindIndexBuffer(vk_cmd->m_command, reinterpret_cast<vk_buffer *>(index_buffer)->m_buffer, offset,
                         vk_index_type);
    return gal_error_code::SUC;
}
gal_error_code vk_cmd_bind_vertex_buffer(gal_command_list command, u32 vertex_buffer_count, gal_buffer *vertex_buffers,
                                         [[maybe_unused]] u32 *stides, u64 *offsets) {
    vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);

    if (vertex_buffer_count > MAX_VERTEX_BUFFER_BINDING) {
        LOG_ERROR("cannot bind more than {} vertex buffers, current is: {}", MAX_VERTEX_BUFFER_BINDING,
                  vertex_buffer_count);
        return gal_error_code::ERR;
    }

    ante::fixed_array<VkBuffer, MAX_VERTEX_BUFFER_BINDING> buffers;
    ante::fixed_array<VkDeviceSize, MAX_VERTEX_BUFFER_BINDING> vb_offsets;
    for (u32 i = 0; i < vertex_buffer_count; i++) {
        if (vertex_buffers[i] == nullptr) {
            LOG_ERROR("invalid vertex buffers");
            return gal_error_code::ERR;
        }
        buffers[i] = reinterpret_cast<vk_buffer *>(vertex_buffers[i])->m_buffer;
        vb_offsets[i] = (offsets ? offsets[i] : 0);
    }

    vkCmdBindVertexBuffers(vk_cmd->m_command, 0, vertex_buffer_count, buffers.data(), vb_offsets.data());
    return gal_error_code::SUC;
}
gal_error_code vk_cmd_bind_descriptor_set() {
    //vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);

    //vkCmdBindDescriptorSets();
    return gal_error_code::SUC;
}
gal_error_code vk_cmd_bind_pipeline(gal_command_list command, gal_pipeline pipeline) {
    vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);
    vkCmdBindPipeline(vk_cmd->m_command, utils_to_vk_pipeline_bind_point(pipeline->m_type),
                      reinterpret_cast<vk_pipeline *>(pipeline)->pipeline);
    return gal_error_code::SUC;
}
gal_error_code vk_cmd_begin_renderpass(gal_command_list command, gal_renderpass_begin_desc *desc) {
    vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);
    if (desc) {

        return gal_error_code::ERR;
    }
    // using dynamic rendering
    //vkCmdBeginRendering();
    VkRenderingInfo ri{};
    vkCmdBeginRendering(vk_cmd->m_command, &ri);
    return gal_error_code::SUC;
}
gal_error_code vk_cmd_end_renderpass(gal_command_list command) {
    vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);

    vkCmdEndRendering(vk_cmd->m_command);
    return gal_error_code::SUC;
}
gal_error_code vk_cmd_dispatch(gal_command_list command, u32 group_count_x, u32 group_count_y, u32 group_count_z) {
    vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);

    vkCmdDispatch(vk_cmd->m_command, group_count_x, group_count_y, group_count_z);
    return gal_error_code::SUC;
}

gal_error_code vk_cmd_draw_instanced(gal_command_list command, u32 vertex_count, u32 first_vertex, u32 instance_count,
                                     u32 first_instance) {
    vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);

    vkCmdDraw(vk_cmd->m_command, vertex_count, instance_count, first_vertex, first_instance);
    return gal_error_code::SUC;
}
gal_error_code vk_cmd_draw_indexed_instanced(gal_command_list command, u32 index_count, u32 first_index,
                                             u32 instance_count, u32 first_instance, u32 first_vertex) {
    vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);

    vkCmdDrawIndexed(vk_cmd->m_command, index_count, instance_count, first_index, first_vertex, first_instance);
    return gal_error_code::SUC;
}

//gal_error_code vk_cmd_dispatch_indirect() {
//    //vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);
//    //vkCmdDispatchIndirect();
//    return gal_error_code::SUC;
//}
//gal_error_code vk_cmd_draw_indirect_instanced() {
//    //vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);
//    //vkCmdDrawIndirect();
//    return gal_error_code::SUC;
//}
//gal_error_code vk_cmd_draw_indirect_indexed_instanced() {
//    //vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);
//    //vkCmdDrawIndexedIndirect();
//    return gal_error_code::SUC;
//}
//
//gal_error_code vk_cmd_draw_indirect_mesh_task() {
//    //vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);
//    // vkCmdDrawMeshTasksEXT
//    return gal_error_code::SUC;
//}

gal_error_code vk_cmd_draw_mesh_task(gal_command_list command) {
    //vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);
    if (command) {
        return gal_error_code::ERR;
    }
    // vkCmdDrawMeshTasksEXT
    return gal_error_code::SUC;
}
gal_error_code vk_cmd_copy_texture(gal_command_list command) {
    //vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);
    if (command) {
        return gal_error_code::ERR;
    }
    //vkCmdCopyImage();
    //vkCmdCopyImage2();
    return gal_error_code::SUC;
}

gal_error_code vk_cmd_copy_buffer(gal_command_list command, gal_buffer src, gal_buffer dst, u64 src_offset,
                                  u64 dst_offset, u64 size) {
    vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);
    VkBufferCopy bc{};
    bc.size = size;
    bc.srcOffset = src_offset;
    bc.dstOffset = dst_offset;
    vkCmdCopyBuffer(vk_cmd->m_command, reinterpret_cast<vk_buffer *>(src)->m_buffer,
                    reinterpret_cast<vk_buffer *>(dst)->m_buffer, 1, &bc);
    return gal_error_code::SUC;
}
gal_error_code vk_cmd_fill_buffer(gal_command_list command) {
    if (command) {
        return gal_error_code::ERR;
    }
    //vkCmdFillBuffer();
    return gal_error_code::SUC;
}
gal_error_code vk_cmd_fill_texture(gal_command_list command) {
    if (command) {
        return gal_error_code::ERR;
    }
    //vkCmdClearColorImage();
    //vkCmdClearDepthStencilImage();
    //vkCmdClearAttachments();
    return gal_error_code::SUC;
}
gal_error_code vk_cmd_upload_buffer(gal_command_list command) {
    if (command) {
        return gal_error_code::ERR;
    }
    return gal_error_code::SUC;
}
gal_error_code vk_cmd_upload_texture(gal_command_list command) {
    if (command) {
        return gal_error_code::ERR;
    }
    return gal_error_code::SUC;
}
gal_error_code vk_cmd_update_subresources(gal_command_list command, gal_texture dst, gal_buffer src,
                                          u32 subresource_count, gal_texture_subresource_desc *descs) {
    vk_command_list *vk_cmd = reinterpret_cast<vk_command_list *>(command);
    if (subresource_count > MAX_TEXTURE_SUBRESOURCE_COUNT) {
        return gal_error_code::ERR;
    }
    const gal_texture_format fmt = dst->m_desc.format;

    if (!gal_tf_is_single_plane(fmt)) {
        return gal_error_code::ERR;
    }
    ACQUIRE_STACK_MEMORY_RESOURCE(stack_memory, MAX_TEXTURE_SUBRESOURCE_COUNT * sizeof(VkBufferCopy));
    ante::vector<VkBufferImageCopy> regions(subresource_count, &stack_memory);

    for (u32 i = 0; i < subresource_count; i++) {
        if (!(descs + i)) {
            LOG_ERROR("invalid subsource desc");
            return gal_error_code::ERR;
        }
        gal_texture_subresource_desc &desc = descs[i];
        u32 width = _max<u32>(1, dst->m_desc.width >> desc.mip_level);
        u32 height = _max<u32>(1, dst->m_desc.height >> desc.mip_level);
        u32 depth = _max<u32>(1, dst->m_desc.depth >> desc.mip_level);

        regions[i].imageOffset.x = 0;
        regions[i].imageOffset.y = 0;
        regions[i].imageOffset.z = 0;
        regions[i].imageExtent.width = width;
        regions[i].imageExtent.height = height;
        regions[i].imageExtent.depth = depth;
        regions[i].bufferOffset = desc.src_offset;
        regions[i].imageSubresource.aspectMask = utils_to_vk_aspect_mask(fmt, false);
        regions[i].imageSubresource.mipLevel = desc.mip_level; // update level 0
        regions[i].imageSubresource.baseArrayLayer = desc.array_layer;
        regions[i].imageSubresource.layerCount = 1;
    }

    vkCmdCopyBufferToImage(vk_cmd->m_command, reinterpret_cast<vk_buffer *>(src)->m_buffer,
                           reinterpret_cast<vk_texture *>(dst)->m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                           subresource_count, regions.data());
    return gal_error_code::SUC;
}
gal_error_code vk_cmd_copy_texture_to_buffer(gal_command_list command) {
    if (command) {
        return gal_error_code::ERR;
    }
    //vkCmdCopyImageToBuffer();
    //vkCmdCopyImageToBuffer2();
    return gal_error_code::SUC;
}

gal_error_code vk_add_queue(gal_context context, gal_queue_desc *desc, gal_queue *queue) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    vk_queue *t_queue = &vk_ctx->queues[utils_to_vk_queue_index(desc->type)];
    if (t_queue->used == false) {
        t_queue->used = true;
    } else {
        return gal_error_code::ERR;
    }

    vk_queue *vk_q = ante::memory::alloc<ante::gal::vk_queue>(nullptr);
    if (!vk_q) {
        return gal_error_code::ERR;
    }
    *vk_q = *t_queue;
    *queue = vk_q;
    return gal_error_code::SUC;
}

gal_error_code vk_remove_queue(gal_context context, gal_queue queue) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
    if (queue != gal_null) {
        vk_queue *vk_q = reinterpret_cast<vk_queue *>(queue);
        vk_ctx->queues[utils_to_vk_queue_index(queue->m_type)].used = false;
        ante::memory::afree(vk_q);
        queue = gal_null;
    }
    return gal_error_code::SUC;
}

gal_error_code vk_queue_submit(gal_queue queue, gal_queue_submit_desc *desc) {
    //vk_context *vk_ctx = reinterpret_cast<vk_context *> (context);
    vk_queue *vk_q = reinterpret_cast<vk_queue *>(queue);

    u32 cmdCount = desc->cmd_count;
    gal_command_list *ppCmds = desc->cmds;
    gal_fence pFence = desc->pSignalFence;
    u32 waitSemaphoreCount = desc->mWaitSemaphoreCount;
    gal_semaphore *ppWaitSemaphores = desc->ppWaitSemaphores;
    u32 signalSemaphoreCount = desc->mSignalSemaphoreCount;
    gal_semaphore *ppSignalSemaphores = desc->ppSignalSemaphores;

    // some parameter check
    // ...
    //

    ACQUIRE_STACK_MEMORY_RESOURCE(stack_emmory, 256);
    ante::vector<VkCommandBuffer> cmds(&stack_emmory);
    for (u32 i = 0; i < cmdCount; ++i) {
        cmds.push_back(reinterpret_cast<vk_command_list *>(ppCmds[i])->m_command);
    }

    ante::vector<VkSemaphore> wait_semaphores(&stack_emmory);
    ante::vector<VkPipelineStageFlags> wait_masks(&stack_emmory);
    for (u32 i = 0; i < waitSemaphoreCount; ++i) {
        vk_semaphore *vk_s = reinterpret_cast<vk_semaphore *>(ppWaitSemaphores[i]);
        if (vk_s->b_signaled) {
            wait_semaphores.push_back(vk_s->semaphore);
            wait_masks.push_back(VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);
            vk_s->b_signaled = false;
        }
    }

    ante::vector<VkSemaphore> signal_semaphores(&stack_emmory);
    for (u32 i = 0; i < signalSemaphoreCount; ++i) {
        vk_semaphore *vk_s = reinterpret_cast<vk_semaphore *>(ppSignalSemaphores[i]);
        if (vk_s->b_signaled) {
            signal_semaphores.push_back(reinterpret_cast<vk_semaphore *>(ppSignalSemaphores[i])->semaphore);
            vk_s->b_signaled = true;
        }
    }

    VkSubmitInfo submit_info{};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext = NULL;
    submit_info.waitSemaphoreCount = static_cast<u32>(wait_semaphores.size());
    submit_info.pWaitSemaphores = wait_semaphores.data();
    submit_info.pWaitDstStageMask = wait_masks.data();
    submit_info.commandBufferCount = cmdCount;
    submit_info.pCommandBuffers = cmds.data();
    submit_info.signalSemaphoreCount = static_cast<u32>(signal_semaphores.size());
    submit_info.pSignalSemaphores = signal_semaphores.data();

    VkDeviceGroupSubmitInfo deviceGroupSubmitInfo{};
    deviceGroupSubmitInfo.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_SUBMIT_INFO_KHR;

    // Lightweight lock to make sure multiple threads dont use the same queue simultaneously
    // Many setups have just one queue family and one queue. In this case, async compute, async transfer doesn't exist and we end up using
    // the same queue for all three operations
    //std::lock_guard<std::mutex>(vk_q->submit_mutex)
    VkResult result = vkQueueSubmit(vk_q->queue, 1, &submit_info,
                                    pFence ? reinterpret_cast<vk_fence *>(pFence)->fence : VK_NULL_HANDLE);
    if (result != VK_SUCCESS) {
        return gal_error_code::ERR;
    }
    if (pFence) {
        reinterpret_cast<vk_fence *>(pFence)->b_submitted = true;
    }
    return gal_error_code::SUC;
}

//gal_error_code vk_create_srvuav(gal_context context, gal_texture texture)
//gal_error_code vk_create_srvuav(gal_context context, gal_buffer buffer)
//{
//    // create view form shader resources
//    if (desc->descriptor_types & gal_descriptor_type::TEXTURE ||
//        desc->descriptor_types & gal_descriptor_type::RW_TEXTURE) {
//
//        VkImageViewType view_type = VK_IMAGE_VIEW_TYPE_MAX_ENUM;
//
//        if (array_size > 1) {
//            if (desc->dimension == gal_texture_dimension::td_1D) {
//                view_type = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
//            }
//            if (desc->dimension == gal_texture_dimension::td_2D) {
//
//                view_type = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
//            }
//            if (b_is_cubemap) {
//                view_type = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
//            }
//        } else {
//            if (desc->dimension == gal_texture_dimension::td_1D) {
//                view_type = VK_IMAGE_VIEW_TYPE_1D;
//            }
//            if (desc->dimension == gal_texture_dimension::td_2D) {
//
//                view_type = VK_IMAGE_VIEW_TYPE_2D;
//            }
//            if (desc->dimension == gal_texture_dimension::td_3D) {
//                view_type = VK_IMAGE_VIEW_TYPE_3D;
//            }
//            if (b_is_cubemap) {
//                view_type = VK_IMAGE_VIEW_TYPE_CUBE;
//            }
//        }
//
//        VkImageViewCreateInfo srvDesc = {};
//        // SRV
//        srvDesc.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
//        srvDesc.pNext = nullptr;
//        srvDesc.flags = 0;
//        srvDesc.image = vk_tex->image;
//        srvDesc.viewType = view_type;
//        srvDesc.format = galtextureformat_to_vkformat(desc->format);
//        srvDesc.components.r = VK_COMPONENT_SWIZZLE_R;
//        srvDesc.components.g = VK_COMPONENT_SWIZZLE_G;
//        srvDesc.components.b = VK_COMPONENT_SWIZZLE_B;
//        srvDesc.components.a = VK_COMPONENT_SWIZZLE_A;
//        srvDesc.subresourceRange.aspectMask =
//            util_vk_determine_aspect_mask(srvDesc.format, gal_tf_has_stencil(desc->format));
//        srvDesc.subresourceRange.baseMipLevel = 0;
//        srvDesc.subresourceRange.levelCount = desc->mip_level;
//        srvDesc.subresourceRange.baseArrayLayer = 0;
//        srvDesc.subresourceRange.layerCount = array_size;
//
//        result = (vkCreateImageView(vk_ctx->device, &srvDesc, nullptr, &vk_tex->image_view));
//        if (result != VK_SUCCESS || vk_tex->image == VK_NULL_HANDLE || vk_tex->allocation == VK_NULL_HANDLE) {
//            return gal_error_code::ERR;
//        }
//    }
//}
// pipeline
//gal_error_code vk_create_shader(gal_context context, gal_shader_desc *desc, gal_shader *_shader) {
//    vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
//    *_shader = reinterpret_cast<gal_shader>(ante::memory::alloc<vk_shader>());
//    vk_shader *vk_s = reinterpret_cast<vk_shader *>(*_shader);
//    VkShaderModuleCreateInfo create_info{};
//    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
//    create_info.codeSize = desc->size;
//    create_info.pCode = reinterpret_cast<u32 *>(desc->data);
//    VkResult result = vkCreateShaderModule(vk_ctx->device, &create_info, nullptr, &vk_s->shader);
//
//    if (result != VK_SUCCESS || vk_s->shader == VK_NULL_HANDLE) {
//        return gal_error_code::ERR;
//    }
//    return gal_error_code::SUC;
//}
//gal_error_code vk_destroy_shader(gal_context context, gal_shader _shader) {
//    if (_shader != gal_null) {
//        vk_context *vk_ctx = reinterpret_cast<vk_context *>(context);
//        vk_shader *vk_s = reinterpret_cast<vk_shader *>(_shader);
//        vkDestroyShaderModule(vk_ctx->device, vk_s->shader, nullptr);
//        ante::memory::afree(vk_s);
//        _shader = gal_null;
//    }
//    return gal_error_code::SUC;
//}

} // namespace ante::gal
