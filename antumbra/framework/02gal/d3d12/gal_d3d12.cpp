#include "gal_d3d12.h"

#include <framework/01core/logging/log.h>

namespace ant::gal {

void load_d3d12_fp()
{
    create_instance = &d3d12_create_instance;
    //create_device = &d3d12_create_device;
}

gal_error_code d3d12_init_gal(gal_context *context)
{
    context->handle = new d3d12_gal_context;
    load_d3d12_fp();
    return context->handle != nullptr ? gal_error_code::success : gal_error_code::error;
}

gal_error_code d3d12_create_instance(GalDesc *desc, gal_context *context)
{
    d3d12_gal_context *d3d12_handle = reinterpret_cast<d3d12_gal_context *>(context->handle);
    u32 dxgi_factory_flags = 0;
    if (desc->b_debug_layer) { dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG; }

    // require directx12.1
    CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&d3d12_handle->factory));
    return d3d12_handle->factory != nullptr ? gal_error_code::success : gal_error_code::error;
}

//gal_error_code d3d12_create_device(GalDesc* gal_desc, gal_context *context)
//{
//    LOG_DEBUG("create d3d12 device");
//    d3d12_gal_context *d3d12_handle = reinterpret_cast<d3d12_gal_context *>(context->handle);
//    return d3d12_handle->device != nullptr ? gal_error_code::success : gal_error_code::error;
//}

gal_error_code d3d12_init_memory_allocator(d3d12_gal_context *context)
{

    D3D12MA::ALLOCATOR_DESC allocatorDesc{};
    allocatorDesc.pDevice = context->device;
    allocatorDesc.pAdapter = context->active_gpu;

    HRESULT hr = D3D12MA::CreateAllocator(&allocatorDesc, &context->d3dma_allocator);

    if (FAILED(hr)) {
        // TODO(hylu): print error
        return gal_error_code::error;
    } else {
        return gal_error_code::success;
    }
}

// gal_error_code d3d12_cmd_begin_recording(const command_list &cmd_list)
//{
//     // HRESULT hr = cmd_list.d3d12->gpu_command_list->Reset();
//     return gal_error_code();
// }

}// namespace ant::gal
