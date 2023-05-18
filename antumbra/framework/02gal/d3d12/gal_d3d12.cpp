#include "gal_d3d12.h"

#include <D3D12MemAlloc.h>
#include <Windows.h>
#include <d3dx12.h>
#include <dxgi1_6.h>

#include <framework/01core/logging/log.h>

#define ANT_DX12_API_VERSION D3D_FEATURE_LEVEL_12_1

namespace ant::gal {

struct d3d12_context {
    IDXGIFactory6 *factory = nullptr;
    ID3D12Device *device = nullptr;
    IDXGIAdapter4 *active_gpu = nullptr;
    D3D12MA::Allocator *d3dma_allocator = nullptr;

    ID3D12CommandQueue *graphics_queue = nullptr;
    ID3D12CommandQueue *compute_queue = nullptr;
    ID3D12CommandQueue *transfer_queue = nullptr;
};

struct d3d12_buffer {
    ID3D12Resource *buffer = nullptr;
    D3D12MA::Allocation *allocation = nullptr;
};

struct d3d12_texture {
    ID3D12Resource *texture = nullptr;
    D3D12MA::Allocation *allocation = nullptr;
};

struct d3d12_sampler {
    ID3D12Resource *sampler = nullptr;
};

struct d3d12_rendertargte {};

struct d3d12_swapchain {
    IDXGISwapChain3 *gpu_swap_chain;
};

struct d3d12_fence {};

struct d3d12_semaphore {};

struct d3d12_command_list {
    ID3D12GraphicsCommandList6 *gpu_command_list;
};

D3D12_RESOURCE_STATES util_to_dx12_resource_state(gal_resource_states state) {
    D3D12_RESOURCE_STATES ret = D3D12_RESOURCE_STATE_COMMON;

    // These states cannot be combined with other states so we just do an == check
    if (state == gal_resource_state::rs_common)
        return D3D12_RESOURCE_STATE_COMMON;
    if (state == gal_resource_state::rs_present)
        return D3D12_RESOURCE_STATE_PRESENT;

    if (state & gal_resource_state::rs_vertex_buffer)
        ret |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
    if (state & gal_resource_state::rs_constant_buffer)
        ret |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
    if (state & gal_resource_state::rs_index_buffer)
        ret |= D3D12_RESOURCE_STATE_INDEX_BUFFER;
    if (state & gal_resource_state::rs_render_target)
        ret |= D3D12_RESOURCE_STATE_RENDER_TARGET;
    if (state & gal_resource_state::rs_rw_buffer)
        ret |= D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    if (state & gal_resource_state::rs_rw_texture)
        ret |= D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    if (state & gal_resource_state::rs_depth_write)
        ret |= D3D12_RESOURCE_STATE_DEPTH_WRITE;
    if (state & gal_resource_state::rs_depth_read)
        ret |= D3D12_RESOURCE_STATE_DEPTH_READ;
    if (state & gal_resource_state::rs_indiret_buffer)
        ret |= D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT;
    if (state & gal_resource_state::rs_copy_dst)
        ret |= D3D12_RESOURCE_STATE_COPY_DEST;
    if (state & gal_resource_state::rs_copy_src)
        ret |= D3D12_RESOURCE_STATE_COPY_SOURCE;
    if (state & gal_resource_state::rs_texture)
        ret |= D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    // TODO(hyl5):rt
    // #ifdef
    //     if (state & RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE)
    //         ret |= D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE;
    // #endif
    //
    // #ifdef ENABLE_VRS
    //     if (state & RESOURCE_STATE_SHADING_RATE_SOURCE) ret |= D3D12_RESOURCE_STATE_SHADING_RATE_SOURCE;
    // #endif

    return ret;
}

D3D12_RESOURCE_FLAGS util_to_dx12_resource_flags(gal_buffer_desc *desc) {
    D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE;

    if (desc->resource_types & gal_resource_type::rt_rw_buffer) {
        flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
    }
    if (desc->resource_types & gal_resource_type::rt_rw_texture) {
        flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
    }
    if (desc->resource_types & gal_resource_type::rt_color_rt) {
        flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    }
    if (desc->resource_types & gal_resource_type::rt_depth_stencil_rt) {
        flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    }
    if (desc->memory_flags == memory_flag::gpu_download) {
        flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
    }
    return flags;
}

void load_gal_d3d12_functions() {
    destroy_gal = &d3d12_destroy_gal;
    create_instance = &d3d12_create_instance;
    destroy_instance = &d3d12_destroy_instance;
    create_device = &d3d12_create_device;
    destroy_device = &d3d12_destroy_device;
    create_memory_allocator = &d3d12_create_memory_allocator;
    destroy_memory_allocator = &d3d12_destroy_memory_allocator;
    create_buffer = &d3d12_create_buffer;
    destroy_buffer = &d3d12_destroy_buffer;
    // create_texture = &d3d12_create_texture;
    // destroy_texture = &d3d12_destroy_texture;
    // create_sampler = &d3d12_create_sampler;
    // destroy_sampler = &d3d12_destroy_sampler;
    // create_rendertarget = &d3d12_create_rendertarget;
    // destroy_rendertarget = &d3d12_destroy_rendertarget;
    //#define D3D12_LOAD_FUNCTION_PTRS
    //#include "../helper/helper_macro.h"
    //#undef D3D12_LOAD_FUNCTION_PTRS
}
void offload_gal_d3d12_functions() {
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
gal_error_code d3d12_init_gal(gal_context *context) {
    *context = reinterpret_cast<gal_handle>(new d3d12_context);
    load_gal_d3d12_functions();
    if (*context == gal_null) {
        return gal_error_code::error;
    }
    return gal_error_code::success;
}
gal_error_code d3d12_destroy_gal(gal_context context) {
    if (context != gal_null) {
        d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(context);
        delete d3d12_ctx;
        context = gal_null;
        offload_gal_d3d12_functions();
    }
    return gal_error_code::success;
}
gal_error_code d3d12_create_instance(GalDesc *desc, gal_context *context) {
    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);

    u32 dxgi_factory_flags = 0;
    if (desc->b_debug_layer) {
        ID3D12Debug1 *pDebugController;
        if (D3D12GetDebugInterface(IID_PPV_ARGS(&pDebugController)) == S_OK) {
            // Enabling GPU Validation without enabling the debug layer does nothing

            pDebugController->EnableDebugLayer();
            pDebugController->SetEnableGPUBasedValidation(true);

            pDebugController->Release();
        }

        dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
    }

    // require directx12.1
    HRESULT hr = CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&d3d12_ctx->factory));
    if (FAILED(hr) || d3d12_ctx->factory == nullptr) {
        return gal_error_code::error;
    }
    return gal_error_code::success;
}

gal_error_code d3d12_destroy_instance(gal_context *context) {

    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);
    if (d3d12_ctx->factory != nullptr) {
        d3d12_ctx->factory->Release();
        d3d12_ctx->factory = nullptr;
    }
    return gal_error_code::success;
}

gal_error_code d3d12_create_device(GalDesc *gal_desc, gal_context *context) {
    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);

    auto pick_gpu = [&](IDXGIFactory6 *factory, IDXGIAdapter4 **gpu, ID3D12Device **device) -> gal_error_code {
        IDXGIAdapter4 *adapter = NULL;
        IDXGIFactory6 *factory6;

        HRESULT hr;

        hr = (factory->QueryInterface(IID_PPV_ARGS(&factory6)));
        if (FAILED(hr)) {
            return gal_error_code::unsupported_device;
        }

        // Find number of usable GPUs
        // Use DXGI6 interface which lets us specify gpu preference so we dont need
        // to use NVOptimus or AMDPowerExpress exports
        for (u32 i = 0; DXGI_ERROR_NOT_FOUND != factory6->EnumAdapterByGpuPreference(
                                                    i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter));
             ++i) {
            DXGI_ADAPTER_DESC3 desc;
            adapter->GetDesc3(&desc);
            ID3D12Device *t_device;

            if (SUCCEEDED(D3D12CreateDevice(adapter, ANT_DX12_API_VERSION, IID_PPV_ARGS(&t_device)))) {
                // if (SUCCEEDED(D3D12CreateDevice(adapter, ANT_DX12_API_VERSION, _uuidof(ID3D12Device), nullptr))) {

                auto check_features = [&]() -> gal_error_code {
                    HRESULT hr = 0;
                    D3D12_FEATURE_DATA_D3D12_OPTIONS featureDataOptions = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &featureDataOptions,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS1 featureDataOptions1 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS1, &featureDataOptions1,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS1)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    bool b_wave_ops = featureDataOptions1.WaveOps;
                    if (!b_wave_ops) {
                        return gal_error_code::unsupported_device;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS2 featureDataOptions2 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS2, &featureDataOptions2,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS2)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS3 featureDataOptions3 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS3, &featureDataOptions3,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS3)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS4 featureDataOptions4 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS4, &featureDataOptions4,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS4)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS5 featureDataOptions5 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &featureDataOptions5,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS5)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    bool b_rt11Supported = (featureDataOptions5.RaytracingTier & D3D12_RAYTRACING_TIER_1_1) != 0;
                    if (!b_rt11Supported) {
                        return gal_error_code::unsupported_device;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS6 featureDataOptions6 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS6, &featureDataOptions6,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS6)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    if (gal_desc->b_variable_rate_shading) {
                        bool b_vrs2Supported =
                            (featureDataOptions6.VariableShadingRateTier & D3D12_VARIABLE_SHADING_RATE_TIER_2) != 0;
                        if (!b_vrs2Supported) {
                            return gal_error_code::unsupported_device;
                        }
                    }
                    D3D12_FEATURE_DATA_D3D12_OPTIONS7 featureDataOptions7 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS7, &featureDataOptions7,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS7)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    if (gal_desc->b_mesh_shader) {
                        bool b_mesh_shader = (featureDataOptions7.MeshShaderTier & D3D12_MESH_SHADER_TIER_1) != 0;
                        if (!b_mesh_shader) {
                            return gal_error_code::unsupported_device;
                        }
                    }

                    // FIXME(hyl5): these code may not compile on some windows sdks
                    D3D12_FEATURE_DATA_D3D12_OPTIONS8 featureDataOptions8 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS8, &featureDataOptions8,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS8)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS9 featureDataOptions9 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS9, &featureDataOptions9,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS9)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    if (gal_desc->b_mesh_shader) {
                        bool b_mesh_shader_pipeline = featureDataOptions9.MeshShaderPipelineStatsSupported;
                        if (!b_mesh_shader_pipeline) {
                            return gal_error_code::unsupported_device;
                        }
                    }
                    D3D12_FEATURE_DATA_D3D12_OPTIONS10 featureDataOptions10 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS10, &featureDataOptions10,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS10)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS11 featureDataOptions11 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS11, &featureDataOptions11,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS11)));
                    if (FAILED(hr)) {
                        return gal_error_code::unsupported_device;
                    }

                    return gal_error_code::success;
                };

                if (check_features() != gal_error_code::success) {
                    t_device->Release();
                    continue;
                }
                *gpu = adapter;
                *device = t_device;
                return gal_error_code::success;
            }
        }
        return gal_error_code::unsupported_device;
    };

    gal_error_code result = pick_gpu(d3d12_ctx->factory, &d3d12_ctx->active_gpu, &d3d12_ctx->device);

    if (result != gal_error_code::success || d3d12_ctx->active_gpu == nullptr || d3d12_ctx->device == nullptr) {
        return gal_error_code::unsupported_device;
    }
    // create queue
    D3D12_COMMAND_QUEUE_DESC graphics_command_queue_desc{};
    graphics_command_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    graphics_command_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    HRESULT hr =
        (d3d12_ctx->device->CreateCommandQueue(&graphics_command_queue_desc, IID_PPV_ARGS(&d3d12_ctx->graphics_queue)));
    if (FAILED(hr) || d3d12_ctx->graphics_queue == nullptr) {
        return gal_error_code::error;
    }
    if (gal_desc->b_async_compute) {
        D3D12_COMMAND_QUEUE_DESC compute_command_queue_desc{};
        compute_command_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        compute_command_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;

        hr = (d3d12_ctx->device->CreateCommandQueue(&compute_command_queue_desc,
                                                    IID_PPV_ARGS(&d3d12_ctx->compute_queue)));

        if (FAILED(hr) || d3d12_ctx->compute_queue == nullptr) {
            return gal_error_code::error;
        }
    } else {
        d3d12_ctx->compute_queue = d3d12_ctx->graphics_queue;
    }
    if (gal_desc->b_async_transfer) {
        D3D12_COMMAND_QUEUE_DESC transfer_command_queue_desc{};
        transfer_command_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        transfer_command_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_COPY;

        hr = (d3d12_ctx->device->CreateCommandQueue(&transfer_command_queue_desc,
                                                    IID_PPV_ARGS(&d3d12_ctx->transfer_queue)));
        if (FAILED(hr) || d3d12_ctx->transfer_queue == nullptr) {
            return gal_error_code::error;
        }
    } else {

        d3d12_ctx->transfer_queue = d3d12_ctx->graphics_queue;
    }
    if (result == gal_error_code::success && d3d12_ctx->active_gpu != nullptr && d3d12_ctx->device != nullptr) {
        return gal_error_code::success;
    } else {
        return gal_error_code::error;
    }
}

gal_error_code d3d12_destroy_device(gal_context *context) {
    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);
    if (d3d12_ctx->device != nullptr) {
        d3d12_ctx->device->Release();
        d3d12_ctx->device = nullptr;
    }
    return gal_error_code::success;
}
gal_error_code d3d12_create_memory_allocator(gal_context *context) {
    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);
    D3D12MA::ALLOCATOR_DESC allocatorDesc{};
    allocatorDesc.pDevice = d3d12_ctx->device;
    allocatorDesc.pAdapter = d3d12_ctx->active_gpu;

    HRESULT hr = D3D12MA::CreateAllocator(&allocatorDesc, &d3d12_ctx->d3dma_allocator);

    if (FAILED(hr) || d3d12_ctx->d3dma_allocator == nullptr) {
        return gal_error_code::error;
    }
    return gal_error_code::success;
}

gal_error_code d3d12_destroy_memory_allocator(gal_context *context) {
    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);
    if (d3d12_ctx->d3dma_allocator != nullptr) {
        d3d12_ctx->d3dma_allocator->Release();
        d3d12_ctx->d3dma_allocator = nullptr;
    }
    return gal_error_code::success;
}

gal_error_code d3d12_create_buffer(gal_context context, gal_buffer_desc *desc, gal_buffer *buffer) {
    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(context);
    *buffer = reinterpret_cast<gal_handle>(new ant::gal::d3d12_buffer);
    d3d12_buffer *d3d12_buf = reinterpret_cast<d3d12_buffer *>(*buffer);

    CD3DX12_RESOURCE_DESC buffer_desc =
        CD3DX12_RESOURCE_DESC::Buffer(desc->size, D3D12_RESOURCE_FLAG_NONE, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);

    buffer_desc.Flags = util_to_dx12_resource_flags(desc);

    D3D12MA::ALLOCATION_DESC allocation_desc{};

    D3D12_RESOURCE_STATES initial_state = util_to_dx12_resource_state(desc->initial_state);

    if (desc->memory_flags == memory_flag::gpu_dedicated) {
        allocation_desc.HeapType = D3D12_HEAP_TYPE_DEFAULT;
    } else if (desc->memory_flags == memory_flag::cpu_upload) {
        allocation_desc.HeapType = D3D12_HEAP_TYPE_UPLOAD;
        initial_state = D3D12_RESOURCE_STATE_GENERIC_READ; // genric read is a required start state for upload heap
    } else if (desc->memory_flags == memory_flag::gpu_download) {
        allocation_desc.HeapType = D3D12_HEAP_TYPE_READBACK;
        initial_state = D3D12_RESOURCE_STATE_COPY_DEST;
    } else {
        return gal_error_code::invalid_parameter;
    }
    if (desc->flags & gal_buffer_flag::bcf_own_memory) {
        allocation_desc.Flags |= D3D12MA::ALLOCATION_FLAGS::ALLOCATION_FLAG_COMMITTED;
    }

    HRESULT hr = (d3d12_ctx->d3dma_allocator->CreateResource(&allocation_desc, &buffer_desc, initial_state, NULL,
                                                             &d3d12_buf->allocation, IID_PPV_ARGS(&d3d12_buf->buffer)));

    if (FAILED(hr) || d3d12_buf->allocation == nullptr) {
        return gal_error_code::error;
    }

    // create descriptor
    //if (!desc->flags & buffer_creation_flag::bcf_no_descriptor_view_creation) {
    //    //TODO(hyl5): create resource descriptor
    //}

    return gal_error_code::success;
}

gal_error_code d3d12_destroy_buffer([[maybe_unused]] gal_context context, gal_buffer buffer) {
    if (buffer != nullptr) {
        d3d12_buffer *d3d12_buf = reinterpret_cast<d3d12_buffer *>(buffer);
        d3d12_buf->buffer->Release();
        d3d12_buf->allocation->Release();
        delete d3d12_buf;
        buffer = nullptr;
    }
    return gal_error_code::success;
}
} // namespace ant::gal
