#include "gal_vulkan.h"

#include <optional>

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#include <framework/01core/logging/log.h>

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
VkBufferUsageFlags util_to_vk_buffer_usage(resource_types types) {
    VkBufferUsageFlags flags = 0;

    if (types & resource_type::rt_constant_buffer) {
        flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    }
    if (types & resource_type::rt_rw_buffer) {
        flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    }

    if (types & resource_type::rt_vertex_buffer) {
        flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    }
    if (types & resource_type::rt_index_buffer) {
        flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    }
    if (types & resource_type::rt_indirect_buffer) {
        flags |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
    }

    // TODO(hyl5): rt;
    return flags;
}

void load_gal_vk_functions() {
    destroy_gal = &vk_destroy_gal;
    create_instance = &vk_create_instance;
    destroy_instance = &vk_destroy_instance;
    create_device = &vk_create_device;
    destroy_device = &vk_destroy_device;
    create_memory_allocator = &vk_create_memory_allocator;
    destroy_memory_allocator = &vk_destroy_memory_allocator;
    create_buffer = &vk_create_buffer;
    destroy_buffer = &vk_destroy_buffer;
    // TODO(hyl5): enable when all functions are availble;
    //#define VK_LOAD_FUNCTION_PTRS
    //#include "../helper/helper_macro.h"
    //#undef VK_LOAD_FUNCTION_PTRS
}

void offload_gal_vk_functions() {
    destroy_gal = nullptr;
    create_instance = nullptr;
    destroy_instance = nullptr;
    create_device = nullptr;
    destroy_device = nullptr;
    create_memory_allocator = nullptr;
    destroy_memory_allocator = nullptr;
    create_buffer = nullptr;
    destroy_buffer = nullptr;
    //#define VK_OFFLOAD_FUNCTION_PTRS
    //#include "../helper/helper_macro.h"
    //#undef VK_LOAD_FUNCTION_PTRS
}

gal_error_code vk_init_gal(gal_context *context) {
    *context = reinterpret_cast<gal_handle>(new vk_gal_context);
    load_gal_vk_functions();
    if (*context == gal_null) {
        return gal_error_code::error;
    }
    return gal_error_code::success;
}

gal_error_code vk_destroy_gal(gal_context context) {
    if (context != gal_null) {
        vk_gal_context *vk_handle = reinterpret_cast<vk_gal_context *>(context);
        delete vk_handle;
        context = gal_null;
        offload_gal_vk_functions();
    }
    return gal_error_code::success;
}

gal_error_code vk_create_instance(GalDesc *gal_desc, gal_context *context) {
    vk_gal_context *vk_handle = reinterpret_cast<vk_gal_context *>(*context);

    auto stack_memory = ant::GetStackMemoryResource(2048);
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

    VkResult res = vkCreateInstance(&instance_create_info, nullptr, &(vk_handle->instance));
    if (res == VK_SUCCESS && vk_handle->instance != VK_NULL_HANDLE) {
        return gal_error_code::success;
    } else {
        return gal_error_code::error;
    }
}
gal_error_code vk_destroy_instance(gal_context *context) {
    vk_gal_context *vk_handle = reinterpret_cast<vk_gal_context *>(*context);

    vkDestroyInstance(vk_handle->instance, nullptr);
    return gal_error_code::success;
}
gal_error_code vk_create_device(GalDesc *gal_desc, gal_context *context) {
    vk_gal_context *vk_handle = reinterpret_cast<vk_gal_context *>(*context);

    auto stack_memory = ant::GetStackMemoryResource(1024u);
    ant::vector<const char *> required_device_extensions(&stack_memory);
    ant::vector<const char *> required_device_layers(&stack_memory);
    if (gal_desc->b_swap_chain) {
        required_device_extensions.emplace_back("VK_KHR_swapchain");
    }
    required_device_extensions.emplace_back("VK_KHR_maintenance1");
    required_device_extensions.emplace_back("VK_KHR_dynamic_rendering");

    u32 physical_device_count = 0;
    // get physical device count
    vkEnumeratePhysicalDevices(vk_handle->instance, &physical_device_count, nullptr);

    ant::vector<VkPhysicalDevice> physical_devices(physical_device_count, &stack_memory);
    if (physical_device_count == 0) {
        LOG_ERROR("no available device");
        return gal_error_code::error;
    }
    vkEnumeratePhysicalDevices(vk_handle->instance, &physical_device_count, physical_devices.data());

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

            auto getQueueFamilyIndex = [&](QueueType queue_type) -> std::optional<u32> {
                // For other queue types or if no separate compute queue is present, return the first one to support
                // the requested flags

                if (queue_type == QueueType::graphcis) {
                    return 0;
                }

                // Dedicated queue for compute
                // Try to find a queue family index that supports compute but not graphics
                if (queue_type == QueueType::compute) {
                    for (uint32_t i = 0; i < static_cast<uint32_t>(queue_family_properties.size()); i++) {
                        if ((queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) &&
                            ((queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)) {
                            return i;
                        }
                    }
                }

                // Dedicated queue for transfer
                // Try to find a queue family index that supports transfer but not graphics and compute
                if (queue_type == QueueType::transfer) {
                    for (uint32_t i = 0; i < static_cast<uint32_t>(queue_family_properties.size()); i++) {
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

            auto graphics_queue_family_index = getQueueFamilyIndex(QueueType::graphcis);
            auto compute_queue_family_index = graphics_queue_family_index;
            auto transfer_queue_family_index = graphics_queue_family_index;
            // TODO(hyl5): code is not clear
            if (gal_desc->b_async_compute) {
                auto t = getQueueFamilyIndex(QueueType::compute);
                if (!t.has_value()) {
                    continue;
                }
                compute_queue_family_index = t.value();
            }
            if (gal_desc->b_async_transfer) {
                auto t = getQueueFamilyIndex(QueueType::transfer);
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

            vk_handle->active_gpu = physical_device;

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
    if (vk_handle->active_gpu == VK_NULL_HANDLE) {
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
    vkGetPhysicalDeviceFeatures2(vk_handle->active_gpu, &device_features);
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
        device_queue_create_info[i].pQueuePriorities = &vk_handle->default_queue_property;
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

    VkResult res = vkCreateDevice(vk_handle->active_gpu, &device_create_info, nullptr, &vk_handle->device);

    // create device queue
    vkGetDeviceQueue(vk_handle->device, queue_family_indices[0], 0, &vk_handle->graphics_queue);

    if (gal_desc->b_async_compute) {
        vkGetDeviceQueue(vk_handle->device, queue_family_indices[1], 0, &vk_handle->compute_queue);
    } else {
        vkGetDeviceQueue(vk_handle->device, queue_family_indices[0], 0, &vk_handle->compute_queue);
    }

    if (gal_desc->b_async_transfer) {
        vkGetDeviceQueue(vk_handle->device, queue_family_indices[2], 0, &vk_handle->transfer_queue);
    } else {
        vkGetDeviceQueue(vk_handle->device, queue_family_indices[0], 0, &vk_handle->transfer_queue);
    }

    if (res != VK_SUCCESS || vk_handle->device == VK_NULL_HANDLE) {
        return gal_error_code::error;
    }

    // TODO(hyl5): query supported memory type of gpu, platform differences
    return gal_error_code::success;
}

gal_error_code vk_destroy_device(gal_context *context) {
    vk_gal_context *vk_handle = reinterpret_cast<vk_gal_context *>(*context);

    vkDestroyDevice(vk_handle->device, nullptr);
    return gal_error_code::success;
}

gal_error_code vk_create_memory_allocator(gal_context *context) {
    vk_gal_context *vk_handle = reinterpret_cast<vk_gal_context *>(*context);

    VmaVulkanFunctions vulkan_functions{};
    vulkan_functions.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
    vulkan_functions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo vma_create_info = {};
    vma_create_info.device = vk_handle->device;
    vma_create_info.physicalDevice = vk_handle->active_gpu;
    vma_create_info.instance = vk_handle->instance;
    vma_create_info.pVulkanFunctions = &vulkan_functions;
    vma_create_info.vulkanApiVersion = ANT_VK_API_VERSION;
    VkResult result = vmaCreateAllocator(&vma_create_info, &vk_handle->vma_allocator);

    if (result == VK_SUCCESS && vk_handle->vma_allocator != nullptr) {
        return gal_error_code::success;
    } else {
        return gal_error_code::error;
    }
}
gal_error_code vk_destroy_memory_allocator(gal_context *context) {
    vk_gal_context *vk_handle = reinterpret_cast<vk_gal_context *>(*context);

    vmaDestroyAllocator(vk_handle->vma_allocator);
    return gal_error_code::success;
}

gal_error_code vk_create_buffer(gal_context context, buffer_desc *desc, buffer *buf) {
    vk_gal_context *vk_handle = reinterpret_cast<vk_gal_context *>(context);
    *buf = reinterpret_cast<gal_handle>(new ant::gal::vk_buffer);
    vk_buffer *vk_buffer_handle = reinterpret_cast<vk_buffer *>(*buf);

    reinterpret_cast<vk_buffer *>(buf);

    VkBufferCreateInfo buffer_create_info{};
    buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.size = desc->size;
    buffer_create_info.usage = util_to_vk_buffer_usage(desc->resource_types);

    VmaAllocationCreateInfo alloc_info = {};
    alloc_info.usage = VMA_MEMORY_USAGE_AUTO;

    if (desc->memory_flags == memory_flag::gpu_dedicated) {
        alloc_info.requiredFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    } else if (desc->memory_flags == memory_flag::cpu_upload) {
        alloc_info.requiredFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    } else if (desc->memory_flags == memory_flag::gpu_download) {
        alloc_info.requiredFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

    } else {
        return gal_error_code::invalid_parameter;
    }

    if (desc->flags & buffer_creation_flag::bcf_own_memory) {
        alloc_info.flags |= VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
    } else if (desc->flags & buffer_creation_flag::bcf_persistent_map) {
        alloc_info.flags |= VMA_ALLOCATION_CREATE_MAPPED_BIT;
    }

    VkResult result = vmaCreateBuffer(vk_handle->vma_allocator, &buffer_create_info, &alloc_info,
                                      &vk_buffer_handle->buffer, &vk_buffer_handle->allocation, nullptr);

    if (result != VK_SUCCESS || vk_buffer_handle->buffer == VK_NULL_HANDLE ||
        vk_buffer_handle->allocation == VK_NULL_HANDLE) {
        return gal_error_code::error;
    }

    return gal_error_code::success;
}

gal_error_code vk_destroy_buffer(gal_context context, buffer buf) {
    vk_gal_context *vk_handle = reinterpret_cast<vk_gal_context *>(context);
    if (buf != gal_null) {
        vk_buffer *vk_buffer_handle = reinterpret_cast<vk_buffer *>(buf);

        vmaDestroyBuffer(vk_handle->vma_allocator, vk_buffer_handle->buffer, vk_buffer_handle->allocation);
        delete vk_buffer_handle;
        buf = gal_null;
    }
    return gal_error_code::success;
}
} // namespace ant::gal
