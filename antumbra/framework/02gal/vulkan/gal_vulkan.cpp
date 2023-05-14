#include "gal_vulkan.h"

#include <framework/01core/logging/log.h>

namespace ant::gal {

void vk_pfn_loader()
{
    create_instance = &vk_create_instance;
    select_gpu = &vk_select_gpu;
    //create_device = &vk_create_device;
}

// gal_error_code vk_result_to_gal_error_code(VkResult res) {

//}

// TODO:hide vk namespace, piml?
gal_error_code vk_init_gal(gal_context *context)
{
    context->handle = new vk_gal_context;
    vk_pfn_loader();
    return context->handle != nullptr ? gal_error_code::success : gal_error_code::error;
}
gal_error_code vk_destroy_gal(gal_context *context)
{
    vk_gal_context *vk_handle = reinterpret_cast<vk_gal_context *>(context->handle);
    if (context->handle != nullptr) {
        delete vk_handle;
        context->handle = nullptr;
    }
    return gal_error_code::success;
}

gal_error_code vk_create_instance(GalDesc *gal_desc, gal_context *context)
{
    vk_gal_context *vk_handle = reinterpret_cast<vk_gal_context *>(context->handle);

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
#endif// WIN32
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

    if (required_layer_count != 0) { return gal_error_code::error; }
    for (u32 i = 0; i < available_extension_count; i++) {
        for (u32 j = 0; j < static_cast<u32>(required_instance_extensions.size()); j++) {
            if (std::strcmp(required_instance_extensions[j], available_extensions[i].extensionName) == 0) {
                required_extension_count--;
            }
        }
    }
    if (required_extension_count != 0) { return gal_error_code::error; }

    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = nullptr;
    app_info.applicationVersion = 0u;
    app_info.pEngineName = nullptr;
    app_info.engineVersion = 0u;
    app_info.apiVersion = VK_API_VERSION_1_3;

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

gal_error_code vk_select_gpu(GalDesc *gal_desc, gal_context *context)
{
    vk_gal_context *vk_handle = reinterpret_cast<vk_gal_context *>(context->handle);
    u32 physical_device_count = 0;

    auto stack_memory = ant::GetStackMemoryResource(1024);

    vkEnumeratePhysicalDevices(vk_handle->instance, &physical_device_count, nullptr);

    ant::vector<VkPhysicalDevice> physical_devices(physical_device_count, &stack_memory);
    if (physical_device_count == 0) {
        LOG_ERROR("no available device");
        return gal_error_code::error;
    }
    vkEnumeratePhysicalDevices(vk_handle->instance, &physical_device_count, physical_devices.data());

    if (gal_desc->b_async_compute) {}
    if (gal_desc->b_async_transfer) {}
    // pick gpu

    for (const auto &physical_device : physical_devices) {

        u32 queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device,
            &queue_family_count,
            nullptr);// Get queue family properties

        // early exist
        u32 required_queue_count = 1u;
        if (gal_desc->b_async_compute) { required_queue_count++; }
        if (gal_desc->b_async_transfer) { required_queue_count++; }

        if (queue_family_count < required_queue_count) {
            // unsupported device
            return gal_error_code::error;
        }
        ant::vector<VkQueueFamilyProperties> queue_family_properties(queue_family_count, &stack_memory);

        queue_family_properties.resize(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device,
            &queue_family_count,
            queue_family_properties.data());// Get queue family properties
        vk_handle->active_gpu = physical_device;
        // for (u32 i = 0; i < queue_family_count; i++) {
        //     // TOOD: print gpu info, runtime swith gpu

        //    // graphics queue
        //    if (queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT
        //        && queue_family_properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT
        //        && queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
        //        m_vulkan.command_queue_familiy_indices[CommandQueueType::GRAPHICS] = i;
        //    }

        //    // dedicate compute queue
        //    if (!(queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        //        && queue_family_properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT
        //        && queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
        //        m_vulkan.command_queue_familiy_indices[CommandQueueType::COMPUTE] = i;
        //        if (m_vulkan.command_queue_familiy_indices[CommandQueueType::GRAPHICS]
        //            != m_vulkan.command_queue_familiy_indices[CommandQueueType::COMPUTE]) {
        //            gpu_support_async_compute = true;
        //        }
        //    }
        //    // dedicate transfer queue
        //    if (!(queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        //        && queue_family_properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT
        //        && !(queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)) {
        //        m_vulkan.command_queue_familiy_indices[CommandQueueType::TRANSFER] = i;
        //        if (m_vulkan.command_queue_familiy_indices[CommandQueueType::GRAPHICS]
        //            != m_vulkan.command_queue_familiy_indices[CommandQueueType::COMPUTE]) {
        //            gpu_support_async_transfer = true;
        //        }
        //    }
        //    *gpu = physical_device;
        //}
        // if (gpu != VK_NULL_HANDLE) { break; }
    }
    if (vk_handle->active_gpu == VK_NULL_HANDLE) { LOG_ERROR("no suitable gpu found"); }
    return gal_error_code::success;
}
// gal_error_code vk_create_device(GalDesc* gal_desc, gal_context *context)
//{
//     LOG_DEBUG("create vk device");
//     vk_gal_context *vk_handle = reinterpret_cast<vk_gal_context *>(context->handle);
//     ant::vector<const char *> required_device_extensions(&stack_memory);
//required_device_extensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
//required_device_extensions.emplace_back(VK_KHR_MAINTENANCE1_EXTENSION_NAME);
//required_device_extensions.emplace_back(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);
//required_device_extensions.emplace_back(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME);
//     return vk_handle->device != VK_NULL_HANDLE ? gal_error_code::success : gal_error_code::error;
// }
}// namespace ant::gal
