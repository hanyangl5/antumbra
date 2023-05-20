#include "gal_vulkan.h"

#include <optional>

#define VMA_RECORDING_ENABLED 1
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#define VMA_IMPLEMENTATION 1
#include <vk_mem_alloc.h>

#include "framework/01core/io/file_system.h"
#include "framework/01core/logging/log.h"
#include "framework/01core/math/math.h"
#include "gal_vulkan_enum.h"
#include "gal_vulkan_utils.h"

#define ANT_VK_API_VERSION VK_API_VERSION_1_3

namespace ant::gal {

// gal_error_code vk_result_to_gal_error_code(VkResult res) {

//}
// TODO(hyl5): chain macro to enable physical device features
// #define ADD_TO_NEXT_CHAIN(condition, next)         \
//    if ((condition)) {                             \
//        base->pNext = (VkBaseOutStructure *)&next; \
//        base = (VkBaseOutStructure *)base->pNext;  \
//    }

gal_error_code vk_init_gal(gal_context *_context) {
    *_context = reinterpret_cast<gal_context>(new vk_context);
    load_gal_vk_functions();
    if (*_context == gal_null) {
        return gal_error_code::error;
    }
    return gal_error_code::success;
}

gal_error_code vk_destroy_gal(gal_context _context) {
    if (_context != gal_null) {
        vk_context *vk_ctx = reinterpret_cast<vk_context *>(_context);
        delete vk_ctx;
        _context = gal_null;
        offload_gal_vk_functions();
    }
    return gal_error_code::success;
}

gal_error_code vk_create_instance(gal_desc *gal_desc, gal_context *_context) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(*_context);

    auto stack_memory = ant::get_stack_memory_resource(2048);
    ant::vector<const char *> required_instance_layers(&stack_memory);
    ant::vector<const char *> required_instance_extensions(&stack_memory);

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

    ant::vector<VkLayerProperties> available_layers(available_layer_count, &stack_memory);
    ant::vector<VkExtensionProperties> available_extensions(available_extension_count, &stack_memory);

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
        return gal_error_code::error;
    }
    for (u32 i = 0; i < available_extension_count; i++) {
        for (u32 j = 0; j < static_cast<u32>(required_instance_extensions.size()); j++) {
            if (std::strcmp(required_instance_extensions[j], available_extensions[i].extensionName) == 0) {
                required_extension_count--;
            }
        }
    }
    if (required_extension_count != 0) {
        return gal_error_code::error;
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
        return gal_error_code::success;
    } else {
        return gal_error_code::error;
    }
}
gal_error_code vk_destroy_instance(gal_context *_context) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(*_context);

    vkDestroyInstance(vk_ctx->instance, nullptr);
    return gal_error_code::success;
}
gal_error_code vk_create_device(gal_desc *gal_desc, gal_context *_context) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(*_context);

    auto stack_memory = ant::get_stack_memory_resource(1024u);
    ant::vector<const char *> required_device_extensions(&stack_memory);
    ant::vector<const char *> required_device_layers(&stack_memory);
    if (gal_desc->b_swap_chain) {
        required_device_extensions.emplace_back("VK_KHR_swapchain");
    }
    required_device_extensions.emplace_back("VK_KHR_maintenance1");
    required_device_extensions.emplace_back("VK_KHR_dynamic_rendering");

    u32 physical_device_count = 0;
    // get physical device count
    vkEnumeratePhysicalDevices(vk_ctx->instance, &physical_device_count, nullptr);

    ant::vector<VkPhysicalDevice> physical_devices(physical_device_count, &stack_memory);
    if (physical_device_count == 0) {
        LOG_ERROR("no available device");
        return gal_error_code::error;
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
    auto pick_gpu = [&](ant::fixed_array<u32, 3> &queue_family_indices) -> gal_error_code {
        for (const auto &physical_device : physical_devices) {
            u32 queue_family_count = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count,
                                                     nullptr); // Get queue family properties

            // early exist

            if (queue_family_count < required_queue_count) {
                // unsupported device
                continue;
            }
            ant::vector<VkQueueFamilyProperties> queue_family_properties(queue_family_count, &stack_memory);

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
            ant::vector<VkLayerProperties> available_device_layers(available_layer_count, &stack_memory);
            vkEnumerateDeviceLayerProperties(physical_device, &available_layer_count, available_device_layers.data());

            vkEnumerateDeviceExtensionProperties(physical_device, layer_name, &available_extension_count, nullptr);
            ant::vector<VkExtensionProperties> available_device_extensions(available_extension_count, &stack_memory);
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

            return gal_error_code::success;
        }
        return gal_error_code::error;
    };

    ant::fixed_array<u32, 3> queue_family_indices;

    gal_error_code result = pick_gpu(queue_family_indices);
    if (result != gal_error_code::success) {
        return gal_error_code::error;
    }
    if (vk_ctx->active_gpu == VK_NULL_HANDLE) {
        return gal_error_code::error;
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
    ant::set<u32> queue_family_indices_set(queue_family_indices.begin(), queue_family_indices.end(), &stack_memory);
    ant::vector<VkDeviceQueueCreateInfo> device_queue_create_info(queue_family_indices_set.size(), &stack_memory);
    u32 idx = 0;
    ant::fixed_array<u32, 3> required_queue_family_indices;
    for (auto &index : queue_family_indices_set) {
        required_queue_family_indices[idx++] = index;
    }

    for (u32 i = 0; i < static_cast<u32>(queue_family_indices_set.size()); i++) {
        device_queue_create_info[i] = {};
        device_queue_create_info[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        device_queue_create_info[i].pNext = NULL;
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

    // create device queue
    vkGetDeviceQueue(vk_ctx->device, queue_family_indices[0], 0, &vk_ctx->graphics_queue);

    if (gal_desc->b_async_compute) {
        vkGetDeviceQueue(vk_ctx->device, queue_family_indices[1], 0, &vk_ctx->compute_queue);
    } else {
        vkGetDeviceQueue(vk_ctx->device, queue_family_indices[0], 0, &vk_ctx->compute_queue);
    }

    if (gal_desc->b_async_transfer) {
        vkGetDeviceQueue(vk_ctx->device, queue_family_indices[2], 0, &vk_ctx->transfer_queue);
    } else {
        vkGetDeviceQueue(vk_ctx->device, queue_family_indices[0], 0, &vk_ctx->transfer_queue);
    }

    if (res != VK_SUCCESS || vk_ctx->device == VK_NULL_HANDLE) {
        return gal_error_code::error;
    }

    // TODO(hyl5): query supported memory type of gpu, platform differences
    return gal_error_code::success;
}

gal_error_code vk_destroy_device(gal_context *_context) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(*_context);

    vkDestroyDevice(vk_ctx->device, nullptr);
    return gal_error_code::success;
}

gal_error_code vk_create_memory_allocator(gal_context *_context) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(*_context);

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
        return gal_error_code::success;
    } else {
        return gal_error_code::error;
    }
}
gal_error_code vk_destroy_memory_allocator(gal_context *_context) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(*_context);

    vmaDestroyAllocator(vk_ctx->vma_allocator);
    return gal_error_code::success;
}

gal_error_code vk_create_buffer(gal_context _context, gal_buffer_desc *_desc, gal_buffer *buffer) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(_context);
    *buffer = reinterpret_cast<gal_buffer>(new ant::gal::vk_buffer);
    vk_buffer *vk_buf = reinterpret_cast<vk_buffer *>(*buffer);

    reinterpret_cast<vk_buffer *>(buffer);

    VkBufferCreateInfo buffer_create_info{};
    buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.size = _desc->size;
    buffer_create_info.usage = util_to_vk_buffer_usage(_desc->resource_types);

    VmaAllocationCreateInfo alloc_info = {};
    alloc_info.usage = VMA_MEMORY_USAGE_AUTO;

    if (_desc->memory_flags & gal_memory_flag::gpu_dedicated) {
        alloc_info.requiredFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    }
    if (_desc->memory_flags & gal_memory_flag::cpu_upload) {
        alloc_info.requiredFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    }
    if (_desc->memory_flags & gal_memory_flag::gpu_download) {
        alloc_info.requiredFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    }

    if (_desc->flags & gal_buffer_flag::bcf_own_memory) {
        alloc_info.flags |= VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
    } else if (_desc->flags & gal_buffer_flag::bcf_persistent_map) {
        alloc_info.flags |= VMA_ALLOCATION_CREATE_MAPPED_BIT;
    }

    VkResult result = vmaCreateBuffer(vk_ctx->vma_allocator, &buffer_create_info, &alloc_info, &vk_buf->buffer,
                                      &vk_buf->allocation, nullptr);

    if (result != VK_SUCCESS || vk_buf->buffer == VK_NULL_HANDLE || vk_buf->allocation == VK_NULL_HANDLE) {
        return gal_error_code::error;
    }

    return gal_error_code::success;
}

gal_error_code vk_destroy_buffer(gal_context _context, gal_buffer buffer) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(_context);

    if (buffer != gal_null) {
        vk_buffer *vk_buf = reinterpret_cast<vk_buffer *>(buffer);

        vmaDestroyBuffer(vk_ctx->vma_allocator, vk_buf->buffer, vk_buf->allocation);
        delete vk_buf;
        buffer = gal_null;
    }
    return gal_error_code::success;
}

gal_error_code vk_create_texture(gal_context _context, gal_texture_desc *_desc, gal_texture *texture) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(_context);
    *texture = reinterpret_cast<gal_texture>(new ant::gal::vk_texture);
    vk_texture *vk_tex = reinterpret_cast<vk_texture *>(*texture);

    u32 array_size = _desc->array_size > 1u ? _desc->array_size : 1u;
    u32 depth = _desc->depth > 1u ? _desc->depth : 1u;
    bool b_is_cubemap = (_desc->resource_types & gal_resource_type::rt_texture_cube) && _desc->array_size == 6;
    VkImageCreateInfo image_create_info{};
    image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.imageType = utils_to_vk_image_type(_desc->dimension);
    image_create_info.extent.width = _desc->width;
    image_create_info.extent.height = _desc->height;
    image_create_info.extent.depth = depth;
    image_create_info.mipLevels = _desc->mip_level;
    image_create_info.arrayLayers = array_size;
    image_create_info.format = galtextureformat_to_vkformat(_desc->format);
    image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_create_info.flags = 0; //utils_to_vk_image_creation_flags(_desc->resource_types);
    image_create_info.usage = utils_to_vk_image_usage(_desc->resource_types, _desc->memory_flags);
    image_create_info.samples = utils_to_vk_sample_count_flags(_desc->sample_count);

    if (_desc->dimension == gal_texture_dimension::td_2D) {
        image_create_info.flags |= VK_IMAGE_CREATE_2D_VIEW_COMPATIBLE_BIT_EXT;
    }

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
        return gal_error_code::error;
    }
    VmaAllocationCreateInfo allocation_create_info{};
    allocation_create_info.usage = VMA_MEMORY_USAGE_AUTO;

    if (_desc->memory_flags & gal_memory_flag::gpu_dedicated) {
        allocation_create_info.requiredFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    }
    if (_desc->memory_flags & gal_memory_flag::cpu_upload) {
        allocation_create_info.requiredFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    }
    if (_desc->memory_flags & gal_memory_flag::gpu_download) {
        allocation_create_info.requiredFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    }

    VkResult result = vmaCreateImage(vk_ctx->vma_allocator, &image_create_info, &allocation_create_info, &vk_tex->image,
                                     &vk_tex->allocation, nullptr);
    if (result != VK_SUCCESS || vk_tex->image == VK_NULL_HANDLE || vk_tex->allocation == VK_NULL_HANDLE) {
        return gal_error_code::error;
    }

    return gal_error_code::success;
}
//gal_error_code vk_create_srvuav(gal_context _context, gal_texture texture)
//gal_error_code vk_create_srvuav(gal_context _context, gal_buffer buffer)
//{
//    // create view form shader resources
//    if (_desc->resource_types & gal_resource_type::rt_texture ||
//        _desc->resource_types & gal_resource_type::rt_rw_texture) {
//
//        VkImageViewType view_type = VK_IMAGE_VIEW_TYPE_MAX_ENUM;
//
//        if (array_size > 1) {
//            if (_desc->dimension == gal_texture_dimension::td_1D) {
//                view_type = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
//            }
//            if (_desc->dimension == gal_texture_dimension::td_2D) {
//
//                view_type = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
//            }
//            if (b_is_cubemap) {
//                view_type = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
//            }
//        } else {
//            if (_desc->dimension == gal_texture_dimension::td_1D) {
//                view_type = VK_IMAGE_VIEW_TYPE_1D;
//            }
//            if (_desc->dimension == gal_texture_dimension::td_2D) {
//
//                view_type = VK_IMAGE_VIEW_TYPE_2D;
//            }
//            if (_desc->dimension == gal_texture_dimension::td_3D) {
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
//        srvDesc.format = galtextureformat_to_vkformat(_desc->format);
//        srvDesc.components.r = VK_COMPONENT_SWIZZLE_R;
//        srvDesc.components.g = VK_COMPONENT_SWIZZLE_G;
//        srvDesc.components.b = VK_COMPONENT_SWIZZLE_B;
//        srvDesc.components.a = VK_COMPONENT_SWIZZLE_A;
//        srvDesc.subresourceRange.aspectMask =
//            util_vk_determine_aspect_mask(srvDesc.format, gal_tf_has_stencil(_desc->format));
//        srvDesc.subresourceRange.baseMipLevel = 0;
//        srvDesc.subresourceRange.levelCount = _desc->mip_level;
//        srvDesc.subresourceRange.baseArrayLayer = 0;
//        srvDesc.subresourceRange.layerCount = array_size;
//
//        result = (vkCreateImageView(vk_ctx->device, &srvDesc, nullptr, &vk_tex->image_view));
//        if (result != VK_SUCCESS || vk_tex->image == VK_NULL_HANDLE || vk_tex->allocation == VK_NULL_HANDLE) {
//            return gal_error_code::error;
//        }
//    }
//}

gal_error_code vk_destroy_texture(gal_context _context, gal_texture texture) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(_context);
    if (texture != gal_null) {
        vk_texture *vk_tex = reinterpret_cast<vk_texture *>(texture);
        vmaDestroyImage(vk_ctx->vma_allocator, vk_tex->image, vk_tex->allocation);
        //vkDestroyImageView(vk_ctx->device, vk_tex->image_view, nullptr);
        delete vk_tex;
        texture = gal_null;
    }
    return gal_error_code::success;
}

gal_error_code vk_create_sampler(gal_context _context, gal_sampler_desc *sampler_desc, gal_sampler *sampler) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(_context);
    *sampler = reinterpret_cast<gal_sampler>(new ant::gal::vk_sampler);
    vk_sampler *vk_spl = reinterpret_cast<vk_sampler *>(*sampler);
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
    sampler_create_info.compareEnable = sampler_desc->compare_mode != gal_compare_mode::undefined;
    sampler_create_info.compareOp = utils_to_vk_compare_op(sampler_desc->compare_mode);
    sampler_create_info.mipLodBias = sampler_desc->mip_lod_bias;
    VkResult result = vkCreateSampler(vk_ctx->device, &sampler_create_info, nullptr, &vk_spl->sampler);
    if (result != VK_SUCCESS || vk_spl->sampler == VK_NULL_HANDLE) {

        return gal_error_code::error;
    }
    return gal_error_code::success;
}

gal_error_code vk_destroy_sampler(gal_context _context, gal_sampler sampler) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(_context);
    if (sampler != gal_null) {
        vk_sampler *vk_spl = reinterpret_cast<vk_sampler *>(sampler);
        vkDestroySampler(vk_ctx->device, vk_spl->sampler, nullptr);
        delete vk_spl;
        sampler = gal_null;
    }
    return gal_error_code::success;
}
gal_error_code vk_create_rendertarget() {
    // fill some rendertarget _desc
    //vk_create_texture();
    return gal_error_code::success;
}
gal_error_code vk_destroy_rendertarget() {
    //vk_destroy_texture();
    return gal_error_code::success;
}
// surface
gal_error_code vk_create_swapchain() {
    //vkCreateSwapchainKHR();
    return gal_error_code::success;
}
gal_error_code vk_destroy_swapchain() {
    //vkDestroySwapchainKHR();
    return gal_error_code::success;
}
gal_error_code vk_create_surface() {
    // //#include <GLFW/glfw3.h>
    //glfwCreateWindowSurface();
    return gal_error_code::success;
}
gal_error_code vk_destroy_surface() {
    //vkDestroySurfaceKHR();
    return gal_error_code::success;
}
// pipeline
gal_error_code vk_create_shader() {
    //vkCreateShaderModule();
    return gal_error_code::success;
}
gal_error_code vk_destroy_shader() {
    //vkDestroyShaderModule();
    return gal_error_code::success;
}

gal_error_code vk_create_pipelinecache(gal_context _context, gal_pipelinecache_desc *_desc,
                                       gal_pipelinecache *pipelinecache) {
    if (_context && _desc && pipelinecache) {

        return gal_error_code::success;
    }
    return gal_error_code::success;
#if 0
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(_context);
    *pipelinecache = reinterpret_cast<gal_pipelinecache>(new ant::gal::vk_pipelinecache);
    vk_pipelinecache *vk_pc = reinterpret_cast<vk_pipelinecache *>(*pipelinecache);

    if (ant::io::is_file(_desc->filename)) {
        ant::vector<u8> file_bin = ant::io::read_binary_file(_desc->filename);
        VkPipelineCacheCreateInfo ci{};
        ci.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        ci.initialDataSize = file_bin.size();
        ci.pInitialData = file_bin.data();
        VkResult result = vkCreatePipelineCache(vk_ctx->device, &ci, nullptr, &vk_pc->pipeline_cache);
        if (result != VK_SUCCESS || vk_pc->pipeline_cache == VK_NULL_HANDLE) {

            return gal_error_code::error;
        }
    }
    return gal_error_code::success;
}

gal_error_code vk_create_compute_pipeline(gal_context _context, gal_compute_pipeline_desc *_desc,
                                          gal_pipeline *pipeline) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(_context);
    *pipeline = reinterpret_cast<gal_pipeline>(new ant::gal::vk_pipeline);
    vk_pipeline *vk_pipe = reinterpret_cast<vk_pipeline *>(*pipeline);
    vk_shader *vk_shader;

    vk_rootsignature *vk_rootsignature;
    VkPipelineShaderStageCreateInfo shader_stage_create_info{};
    shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stage_create_info.flags = 0;
    shader_stage_create_info.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    shader_stage_create_info.module = vk_shader->shader;
    shader_stage_create_info.pName = vk_shader->entry.c_str();
    shader_stage_create_info.pSpecializationInfo = nullptr;

    VkComputePipelineCreateInfo pipeline_create_info{};
    pipeline_create_info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipeline_create_info.flags = 0;
    pipeline_create_info.layout = vk_rootsignature->pipeline_layout;
    pipeline_create_info.stage = shader_stage_create_info;

    VkPipelineCache pipeline_cache = VK_NULL_HANDLE;

    if (_desc->pipeline_cache != gal_null) {
        pipeline_cache = reinterpret_cast<vk_pipelinecache *>(_desc->pipeline_cache)->pipeline_cache;
    }

    VkResult result =
        vkCreateComputePipelines(vk_ctx->device, pipeline_cache, 1, &pipeline_create_info, nullptr, &vk_pipe->pipeline);
    if (result != VK_SUCCESS || vk_pipe->pipeline == VK_NULL_HANDLE) {
        return gal_error_code::error;
    }
    return gal_error_code::success;
}
gal_error_code vk_create_graphics_pipeline(gal_context _context, gal_graphics_pipeline_desc *_desc,
                                           gal_pipeline *pipeline) {
    vk_context *vk_ctx = reinterpret_cast<vk_context *>(_context);
    *pipeline = reinterpret_cast<gal_pipeline>(new ant::gal::vk_pipeline);
    vk_pipeline *vk_pipe = reinterpret_cast<vk_pipeline *>(*pipeline);
    VkGraphicsPipelineCreateInfo pipeline_create_info{};
    gal_shader *pShaderProgram = _desc->pShaderProgram;
    VertexLayout *pVertexLayout = _desc->pVertexLayout;
    u32 stage_count = 0;
    VkPipelineShaderStageCreateInfo stages[5];
    for (u32 i = 0; i < 5; ++i) {
        ShaderStage stage_mask = (ShaderStage)(1 << i);
        if (stage_mask == (pShaderProgram->mStages & stage_mask)) {
            stages[stage_count].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            stages[stage_count].pNext = NULL;
            stages[stage_count].flags = 0;
            stages[stage_count].pSpecializationInfo = nullptr;
            switch (stage_mask) {
            case SHADER_STAGE_VERT: {
                stages[stage_count].pName =
                    pShaderProgram->pReflection->mStageReflections[pShaderProgram->pReflection->mVertexStageIndex]
                        .pEntryPoint;
                stages[stage_count].stage = VK_SHADER_STAGE_VERTEX_BIT;
                stages[stage_count].module =
                    pShaderProgram->mVulkan.pShaderModules[pShaderProgram->pReflection->mVertexStageIndex];
            } break;
            case SHADER_STAGE_TESC: {
                stages[stage_count].pName =
                    pShaderProgram->pReflection->mStageReflections[pShaderProgram->pReflection->mHullStageIndex]
                        .pEntryPoint;
                stages[stage_count].stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
                stages[stage_count].module =
                    pShaderProgram->mVulkan.pShaderModules[pShaderProgram->pReflection->mHullStageIndex];
            } break;
            case SHADER_STAGE_TESE: {
                stages[stage_count].pName =
                    pShaderProgram->pReflection->mStageReflections[pShaderProgram->pReflection->mDomainStageIndex]
                        .pEntryPoint;
                stages[stage_count].stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
                stages[stage_count].module =
                    pShaderProgram->mVulkan.pShaderModules[pShaderProgram->pReflection->mDomainStageIndex];
            } break;
            case SHADER_STAGE_GEOM: {
                stages[stage_count].pName =
                    pShaderProgram->pReflection->mStageReflections[pShaderProgram->pReflection->mGeometryStageIndex]
                        .pEntryPoint;
                stages[stage_count].stage = VK_SHADER_STAGE_GEOMETRY_BIT;
                stages[stage_count].module =
                    pShaderProgram->mVulkan.pShaderModules[pShaderProgram->pReflection->mGeometryStageIndex];
            } break;
            case SHADER_STAGE_FRAG: {
                stages[stage_count].pName =
                    pShaderProgram->pReflection->mStageReflections[pShaderProgram->pReflection->mPixelStageIndex]
                        .pEntryPoint;
                stages[stage_count].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
                stages[stage_count].module =
                    pShaderProgram->mVulkan.pShaderModules[pShaderProgram->pReflection->mPixelStageIndex];
            } break;
            default:
                assert(false && "Shader Stage not supported!");
                break;
            }
            ++stage_count;
        }
    }

    // Make sure there's a shader
    assert(0 != stage_count);

    u32 input_binding_count = 0;
    VkVertexInputBindingDescription input_bindings[MAX_VERTEX_BINDINGS] = {{0}};
    u32 input_attribute_count = 0;
    VkVertexInputAttributeDescription input_attributes[MAX_VERTEX_ATTRIBS] = {{0}};

    // Make sure there's attributes
    if (pVertexLayout != NULL) {
        // Ignore everything that's beyond max_vertex_attribs
        u32 attrib_count =
            pVertexLayout->mAttribCount > MAX_VERTEX_ATTRIBS ? MAX_VERTEX_ATTRIBS : pVertexLayout->mAttribCount;
        u32 binding_value = UINT32_MAX;

        // Initial values
        for (u32 i = 0; i < attrib_count; ++i) {
            const VertexAttrib *attrib = &(pVertexLayout->mAttribs[i]);

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
    vi.pNext = NULL;
    vi.flags = 0;
    vi.vertexBindingDescriptionCount = input_binding_count;
    vi.pVertexBindingDescriptions = input_bindings;
    vi.vertexAttributeDescriptionCount = input_attribute_count;
    vi.pVertexAttributeDescriptions = input_attributes;

    VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    switch (_desc->mPrimitiveTopo) {
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
        assert(false && "Primitive Topo not supported!");
        break;
    }
    VkPipelineInputAssemblyStateCreateInfo ia{};
    ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    ia.pNext = NULL;
    ia.flags = 0;
    ia.topology = topology;
    ia.primitiveRestartEnable = VK_FALSE;

    VkPipelineTessellationStateCreateInfo ts{};
    if ((pShaderProgram->mStages & SHADER_STAGE_TESC) && (pShaderProgram->mStages & SHADER_STAGE_TESE)) {
        ts.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
        ts.pNext = NULL;
        ts.flags = 0;
        ts.patchControlPoints =
            pShaderProgram->pReflection->mStageReflections[pShaderProgram->pReflection->mHullStageIndex]
                .mNumControlPoint;
    }

    VkPipelineViewportStateCreateInfo vs{};
    vs.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vs.pNext = NULL;
    vs.flags = 0;
    // we are using dynamic viewports but we must set the count to 1
    vs.viewportCount = 1;
    vs.pViewports = NULL;
    vs.scissorCount = 1;
    vs.pScissors = NULL;
    VkPipelineMultisampleStateCreateInfo ms{};
    ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    ms.pNext = NULL;
    ms.flags = 0;
    ms.rasterizationSamples = util_to_vk_sample_count(_desc->mSampleCount);
    ms.sampleShadingEnable = VK_FALSE;
    ms.minSampleShading = 0.0f;
    ms.pSampleMask = 0;
    ms.alphaToCoverageEnable = VK_FALSE;
    ms.alphaToOneEnable = VK_FALSE;

    VkPipelineRasterizationStateCreateInfo rs;
    rs = _desc->pRasterizerState ? util_to_rasterizer_desc(_desc->pRasterizerState) : gDefaultRasterizerDesc;

    /// TODO: Dont create depth state if no depth stencil bound
    VkPipelineDepthStencilStateCreateInfo ds{};
    ds = _desc->pDepthState ? util_to_depth_desc(_desc->pDepthState) : gDefaultDepthDesc;

    VkPipelineColorBlendStateCreateInfo cb{};
    VkPipelineColorBlendAttachmentState cbAtt[MAX_RENDER_TARGET_ATTACHMENTS];
    cb = _desc->pBlendState ? util_to_blend_desc(_desc->pBlendState, cbAtt) : gDefaultBlendDesc;
    cb.attachmentCount = _desc->mRenderTargetCount;

    VkDynamicState dyn_states[] = {
        VK_DYNAMIC_STATE_VIEWPORT,     VK_DYNAMIC_STATE_SCISSOR,           VK_DYNAMIC_STATE_BLEND_CONSTANTS,
        VK_DYNAMIC_STATE_DEPTH_BOUNDS, VK_DYNAMIC_STATE_STENCIL_REFERENCE,
    };
    VkPipelineDynamicStateCreateInfo dy{};
    dy.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dy.pNext = NULL;
    dy.flags = 0;
    dy.dynamicStateCount = sizeof(dyn_states) / sizeof(dyn_states[0]);
    dy.pDynamicStates = dyn_states;

    VkGraphicsPipelineCreateInfo add_info{};
    add_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    add_info.pNext = NULL;
    add_info.flags = 0;
    add_info.stageCount = stage_count;
    add_info.pStages = stages;
    add_info.pVertexInputState = &vi;
    add_info.pInputAssemblyState = &ia;

    if ((pShaderProgram->mStages & SHADER_STAGE_TESC) && (pShaderProgram->mStages & SHADER_STAGE_TESE))
        add_info.pTessellationState = &ts;
    else
        add_info.pTessellationState = NULL; // set tessellation state to null if we have no tessellation

    add_info.pViewportState = &vs;
    add_info.pRasterizationState = &rs;
    add_info.pMultisampleState = &ms;
    add_info.pDepthStencilState = &ds;
    add_info.pColorBlendState = &cb;
    add_info.pDynamicState = &dy;
    add_info.layout = _desc->pRootSignature->mVulkan.pPipelineLayout;
    add_info.subpass = 0;
    result = (vkCreateGraphicsPipelines(pRenderer->mVulkan.pVkDevice, psoCache, 1, &add_info, &gVkAllocationCallbacks,
                                        &(pPipeline->mVulkan.pVkPipeline)));
#endif
}
//gal_error_code vk_create_raytracing_pipeline(gal_context _context, gal_raytracing_pipeline_desc *_desc,
//                                           gal_pipeline *pipeline) {
//    vk_context *vk_ctx = reinterpret_cast<vk_context *>(_context);
//    *pipeline = reinterpret_cast<gal_pipeline>(new ant::gal::vk_pipeline);
//    vk_pipeline *vk_spl = reinterpret_cast<vk_pipeline *>(*pipeline);
//    VkRayTracingPipelineCreateInfoKHR pipeline_create_info{};
//    pipeline_create_info.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR;
//    pipeline_create_info.
//}

gal_error_code vk_destroy_pipeline() {
    //vkDestroyPipeline();
    return gal_error_code::success;
}

struct vk_descriptorpool {
    VkDescriptorPool pool;
};
//gal_error_code vk_create_descriptorpool(gal_context _context, gal_descriptorpool_desc* _desc, gal_descriptorpool* descriptorpool) {
//    return gal_error_code::success;
//}
gal_error_code vk_destroy_descriptorpool() {
    //vkDestroyDescriptorPool();
    return gal_error_code::success;
}
gal_error_code vk_consume_descriptorset() {
    //vkAllocateDescriptorSets();
    return gal_error_code::success;
}
gal_error_code vk_free_descriptorset() {
    //vkFreeDescriptorSets()
    return gal_error_code::success;
}
gal_error_code vk_create_rootsignature() {
    //vkCreatePipelineLayout();
    return gal_error_code::success;
}
gal_error_code vk_destroy_rootsignature() {
    //vkDestroyPipelineLayout();
    return gal_error_code::success;
}
// sync
gal_error_code vk_create_fence() {
    //vkCreateFence();
    return gal_error_code::success;
}
gal_error_code vk_wait_fence() {
    //vkWaitForFences();
    return gal_error_code::success;
}
gal_error_code vk_destroy_fence() {
    //vkDestroyFence();
    return gal_error_code::success;
}
gal_error_code vk_wait_gpu() {
    //vkDeviceWaitIdle();
    return gal_error_code::success;
}
gal_error_code vk_create_semaphore() {
    //vkCreateSemaphore();
    return gal_error_code::success;
}
gal_error_code vk_destroy_semaphore() {
    //vkDestroySemaphore();
    return gal_error_code::success;
}
// cmds
gal_error_code vk_create_commandpool() {
    //vkCreateCommandPool();
    return gal_error_code::success;
}
gal_error_code vk_reset_commandpool() {
    //vkResetCommandPool();
    return gal_error_code::success;
}
gal_error_code vk_destroy_commandpool() {
    //vkDestroyCommandPool();
    return gal_error_code::success;
}
gal_error_code vk_allocate_commandlist() {
    //vkAllocateCommandBuffers();
    return gal_error_code::success;
}
gal_error_code vk_free_commandlist() {
    //vkFreeCommandBuffers();
    return gal_error_code::success;
}
gal_error_code vk_cmd_begin_recording() {
    //vkBeginCommandBuffer();
    return gal_error_code::success;
}
gal_error_code vk_cmd_end_recording() {
    //vkEndCommandBuffer();
    return gal_error_code::success;
}
gal_error_code vk_cmd_resource_barrier() {
    //vkCmdPipelineBarrier();
    //vkCmdPipelineBarrier2();
    return gal_error_code::success;
}

gal_error_code vk_cmd_bind_index_buffer() {
    //vkCmdBindIndexBuffer();
    return gal_error_code::success;
}
gal_error_code vk_cmd_bind_vertex_buffer() {
    //vkCmdBindVertexBuffers();
    return gal_error_code::success;
}
gal_error_code vk_cmd_bind_descriptorset() {
    //vkCmdBindDescriptorSets();
    return gal_error_code::success;
}
gal_error_code vk_cmd_bind_pipeline() {
    //vkCmdBindPipeline();
    return gal_error_code::success;
}
gal_error_code vk_cmd_begin_renderpass() {
    // using dynamic rendering
    //vkCmdBeginRendering();
    return gal_error_code::success;
}
gal_error_code vk_cmd_end_renderpass() {
    //vkCmdEndRendering();
    return gal_error_code::success;
}
gal_error_code vk_cmd_dispatch() {
    //vkCmdDispatch();
    return gal_error_code::success;
}
gal_error_code vk_cmd_dispatch_indirect() {
    //vkCmdDispatchIndirect();
    return gal_error_code::success;
}
gal_error_code vk_cmd_draw_instanced() {
    //vkCmdDraw();
    return gal_error_code::success;
}
gal_error_code vk_cmd_draw_indexed_instanced() {
    //vkCmdDrawIndexed();
    return gal_error_code::success;
}
gal_error_code vk_cmd_draw_indirect_instanced() {
    //vkCmdDrawIndirect();
    return gal_error_code::success;
}
gal_error_code vk_cmd_draw_indirect_indexed_instanced() {
    //vkCmdDrawIndexedIndirect();
    return gal_error_code::success;
}
gal_error_code vk_cmd_draw_mesh_task() {
    // vkCmdDrawMeshTasksEXT
    return gal_error_code::success;
}
gal_error_code vk_cmd_draw_indirect_mesh_task() {
    // vkCmdDrawMeshTasksEXT
    return gal_error_code::success;
}
gal_error_code vk_cmd_copy_texture() {
    //vkCmdCopyImage();
    //vkCmdCopyImage2();
    return gal_error_code::success;
}
gal_error_code vk_cmd_copy_buffer() {
    //vkCmdCopyBuffer();
    // vkCmdCopyBuffer2()
    return gal_error_code::success;
}
gal_error_code vk_cmd_fill_buffer() {
    //vkCmdFillBuffer();
    return gal_error_code::success;
}
gal_error_code vk_cmd_fill_texture() {
    //vkCmdClearColorImage();
    //vkCmdClearDepthStencilImage();
    //vkCmdClearAttachments();
    return gal_error_code::success;
}
gal_error_code vk_cmd_upload_buffer() { return gal_error_code::success; }
gal_error_code vk_cmd_upload_texture() { return gal_error_code::success; }
gal_error_code vk_cmd_copy_buffer_to_texture() {
    //vkCmdCopyBufferToImage();
    //vkCmdCopyBufferToImage2();
    return gal_error_code::success;
}
gal_error_code vk_cmd_copy_texture_to_buffer() {
    //vkCmdCopyImageToBuffer();
    //vkCmdCopyImageToBuffer2();
    return gal_error_code::success;
}

} // namespace ant::gal
