#include "gal_d3d12.h"

#include <D3D12MemAlloc.h>
#include <Windows.h>
#include <d3dx12.h>
#include <dxgi1_6.h>

#include "framework/01core/logging/log.h"
#include "gal_d3d12_enum.h"
#include "gal_d3d12_utils.h"

#define ANT_DX12_API_VERSION D3D_FEATURE_LEVEL_12_1

namespace ante::gal {

gal_error_code d3d12_init_gal(gal_context *context) {
    *context = reinterpret_cast<gal_context>(new d3d12_context);
    load_gal_d3d12_functions();
    if (*context == gal_null) {
        return gal_error_code::ERR;
    }
    return gal_error_code::SUC;
}
gal_error_code d3d12_destroy_gal(gal_context context) {
    if (context != gal_null) {
        d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(context);
        delete d3d12_ctx;
        context = gal_null;
        offload_gal_d3d12_functions();
    }
    return gal_error_code::SUC;
}

gal_error_code d3d12_create_instance(gal_desc *desc, gal_context *context) {
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
        return gal_error_code::ERR;
    }
    return gal_error_code::SUC;
}

gal_error_code d3d12_destroy_instance(gal_context *context) {

    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);
    if (d3d12_ctx->factory != nullptr) {
        d3d12_ctx->factory->Release();
        d3d12_ctx->factory = nullptr;
    }
    return gal_error_code::SUC;
}

gal_error_code d3d12_create_device(gal_desc *gal_desc, gal_context *context) {
    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);

    auto pick_gpu = [&](IDXGIFactory6 *factory, IDXGIAdapter4 **gpu, ID3D12Device **device) -> gal_error_code {
        IDXGIAdapter4 *adapter = NULL;
        IDXGIFactory6 *factory6;

        HRESULT hr;

        hr = (factory->QueryInterface(IID_PPV_ARGS(&factory6)));
        if (FAILED(hr)) {
            return gal_error_code::GAL_ERRORCODE_INVALID_DEVICE;
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
                        return gal_error_code::GAL_ERRORCODE_INVALID_DEVICE;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS1 featureDataOptions1 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS1, &featureDataOptions1,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS1)));
                    if (FAILED(hr)) {
                        return gal_error_code::GAL_ERRORCODE_INVALID_DEVICE;
                    }

                    bool b_wave_ops = featureDataOptions1.WaveOps;
                    if (!b_wave_ops) {
                        return gal_error_code::GAL_ERRORCODE_INVALID_DEVICE;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS2 featureDataOptions2 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS2, &featureDataOptions2,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS2)));
                    if (FAILED(hr)) {
                        return gal_error_code::GAL_ERRORCODE_INVALID_DEVICE;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS3 featureDataOptions3 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS3, &featureDataOptions3,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS3)));
                    if (FAILED(hr)) {
                        return gal_error_code::GAL_ERRORCODE_INVALID_DEVICE;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS4 featureDataOptions4 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS4, &featureDataOptions4,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS4)));
                    if (FAILED(hr)) {
                        return gal_error_code::GAL_ERRORCODE_INVALID_DEVICE;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS5 featureDataOptions5 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &featureDataOptions5,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS5)));
                    if (FAILED(hr)) {
                        return gal_error_code::GAL_ERRORCODE_INVALID_DEVICE;
                    }

                    bool b_rt11Supported = (featureDataOptions5.RaytracingTier & D3D12_RAYTRACING_TIER_1_1) != 0;
                    if (!b_rt11Supported) {
                        return gal_error_code::GAL_ERRORCODE_INVALID_DEVICE;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS6 featureDataOptions6 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS6, &featureDataOptions6,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS6)));
                    if (FAILED(hr)) {
                        return gal_error_code::GAL_ERRORCODE_INVALID_DEVICE;
                    }

                    if (gal_desc->b_variable_rate_shading) {
                        bool b_vrs2Supported =
                            (featureDataOptions6.VariableShadingRateTier & D3D12_VARIABLE_SHADING_RATE_TIER_2) != 0;
                        if (!b_vrs2Supported) {
                            return gal_error_code::GAL_ERRORCODE_INVALID_DEVICE;
                        }
                    }
                    D3D12_FEATURE_DATA_D3D12_OPTIONS7 featureDataOptions7 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS7, &featureDataOptions7,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS7)));
                    if (FAILED(hr)) {
                        return gal_error_code::GAL_ERRORCODE_INVALID_DEVICE;
                    }

                    if (gal_desc->b_mesh_shader) {
                        bool b_mesh_shader = (featureDataOptions7.MeshShaderTier & D3D12_MESH_SHADER_TIER_1) != 0;
                        if (!b_mesh_shader) {
                            return gal_error_code::GAL_ERRORCODE_INVALID_DEVICE;
                        }
                    }

                    // FIXME(hyl5): these code may not compile on some windows sdks
                    D3D12_FEATURE_DATA_D3D12_OPTIONS8 featureDataOptions8 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS8, &featureDataOptions8,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS8)));
                    if (FAILED(hr)) {
                        return gal_error_code::GAL_ERRORCODE_INVALID_DEVICE;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS9 featureDataOptions9 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS9, &featureDataOptions9,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS9)));
                    if (FAILED(hr)) {
                        return gal_error_code::GAL_ERRORCODE_INVALID_DEVICE;
                    }

                    if (gal_desc->b_mesh_shader) {
                        bool b_mesh_shader_pipeline = featureDataOptions9.MeshShaderPipelineStatsSupported;
                        if (!b_mesh_shader_pipeline) {
                            return gal_error_code::GAL_ERRORCODE_INVALID_DEVICE;
                        }
                    }
                    D3D12_FEATURE_DATA_D3D12_OPTIONS10 featureDataOptions10 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS10, &featureDataOptions10,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS10)));
                    if (FAILED(hr)) {
                        return gal_error_code::GAL_ERRORCODE_INVALID_DEVICE;
                    }

                    D3D12_FEATURE_DATA_D3D12_OPTIONS11 featureDataOptions11 = {};
                    hr = (t_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS11, &featureDataOptions11,
                                                        sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS11)));
                    if (FAILED(hr)) {
                        return gal_error_code::GAL_ERRORCODE_INVALID_DEVICE;
                    }

                    return gal_error_code::SUC;
                };

                if (check_features() != gal_error_code::SUC) {
                    t_device->Release();
                    continue;
                }
                *gpu = adapter;
                *device = t_device;
                return gal_error_code::SUC;
            }
        }
        return gal_error_code::GAL_ERRORCODE_INVALID_DEVICE;
    };

    gal_error_code result = pick_gpu(d3d12_ctx->factory, &d3d12_ctx->active_gpu, &d3d12_ctx->device);

    if (result != gal_error_code::SUC || d3d12_ctx->active_gpu == nullptr || d3d12_ctx->device == nullptr) {
        return gal_error_code::GAL_ERRORCODE_INVALID_DEVICE;
    }
    // create queue
    D3D12_COMMAND_QUEUE_DESC graphics_command_queue_desc{};
    graphics_command_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    graphics_command_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    HRESULT hr =
        (d3d12_ctx->device->CreateCommandQueue(&graphics_command_queue_desc, IID_PPV_ARGS(&d3d12_ctx->graphics_queue)));
    if (FAILED(hr) || d3d12_ctx->graphics_queue == nullptr) {
        return gal_error_code::ERR;
    }
    if (gal_desc->b_async_compute) {
        D3D12_COMMAND_QUEUE_DESC compute_command_queue_desc{};
        compute_command_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        compute_command_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;

        hr = (d3d12_ctx->device->CreateCommandQueue(&compute_command_queue_desc,
                                                    IID_PPV_ARGS(&d3d12_ctx->compute_queue)));

        if (FAILED(hr) || d3d12_ctx->compute_queue == nullptr) {
            return gal_error_code::ERR;
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
            return gal_error_code::ERR;
        }
    } else {

        d3d12_ctx->transfer_queue = d3d12_ctx->graphics_queue;
    }
    if (result == gal_error_code::SUC && d3d12_ctx->active_gpu != nullptr && d3d12_ctx->device != nullptr) {
        return gal_error_code::SUC;
    } else {
        return gal_error_code::ERR;
    }
}

gal_error_code d3d12_destroy_device(gal_context *context) {
    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);
    if (d3d12_ctx->device != nullptr) {
        d3d12_ctx->device->Release();
        d3d12_ctx->device = nullptr;
    }
    return gal_error_code::SUC;
}
gal_error_code d3d12_create_memory_allocator(gal_context *context) {
    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);
    D3D12MA::ALLOCATOR_DESC allocatorDesc{};
    allocatorDesc.pDevice = d3d12_ctx->device;
    allocatorDesc.pAdapter = d3d12_ctx->active_gpu;

    HRESULT hr = D3D12MA::CreateAllocator(&allocatorDesc, &d3d12_ctx->d3dma_allocator);

    if (FAILED(hr) || d3d12_ctx->d3dma_allocator == nullptr) {
        return gal_error_code::ERR;
    }
    return gal_error_code::SUC;
}

gal_error_code d3d12_destroy_memory_allocator(gal_context *context) {
    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(*context);
    if (d3d12_ctx->d3dma_allocator != nullptr) {
        d3d12_ctx->d3dma_allocator->Release();
        d3d12_ctx->d3dma_allocator = nullptr;
    }
    return gal_error_code::SUC;
}

gal_error_code d3d12_create_buffer(gal_context context, gal_buffer_desc *desc, gal_buffer *buffer) {
    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(context);
    d3d12_buffer *d3d12_buf = ante::memory::alloc<ante::gal::d3d12_buffer>(nullptr);
    if (d3d12_buf == nullptr) {
        return gal_error_code::ERR;
    }

    CD3DX12_RESOURCE_DESC buffer_desc =
        CD3DX12_RESOURCE_DESC::Buffer(desc->size, D3D12_RESOURCE_FLAG_NONE, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT);

    buffer_desc.Flags = util_to_dx12_resource_flags(desc);

    D3D12MA::ALLOCATION_DESC allocation_desc{};

    D3D12_RESOURCE_STATES initial_state = util_to_dx12_resource_state(desc->initial_state);

    if (desc->memory_flags == gal_memory_flag::GPU_DEDICATED) {
        allocation_desc.HeapType = D3D12_HEAP_TYPE_DEFAULT;
    } else if (desc->memory_flags == gal_memory_flag::CPU_UPLOAD) {
        allocation_desc.HeapType = D3D12_HEAP_TYPE_UPLOAD;
        initial_state = D3D12_RESOURCE_STATE_GENERIC_READ; // genric read is a required start state for upload heap
    } else if (desc->memory_flags == gal_memory_flag::GPU_DOWNLOAD) {
        allocation_desc.HeapType = D3D12_HEAP_TYPE_READBACK;
        initial_state = D3D12_RESOURCE_STATE_COPY_DEST;
    } else {
        return gal_error_code::GAL_ERRORCODE_INVALID_PARAMETER;
    }
    if ((desc->flags & gal_buffer_flag::OWN_MEMORY) != gal_buffer_flag::UNDEFINED) {
        allocation_desc.Flags = D3D12MA::ALLOCATION_FLAGS::ALLOCATION_FLAG_COMMITTED;
    }

    HRESULT hr = (d3d12_ctx->d3dma_allocator->CreateResource(&allocation_desc, &buffer_desc, initial_state, NULL,
                                                             &d3d12_buf->allocation, IID_PPV_ARGS(&d3d12_buf->buffer)));

    if (FAILED(hr) || d3d12_buf->allocation == nullptr) {
        return gal_error_code::ERR;
    }

    // create descriptor
    //if (!desc->flags & buffer_creation_flag::NO_DESCRIPTOR_VIEW_CREATION) {
    //    //TODO(hyl5): create resource descriptor
    //}
    *buffer = d3d12_buf;
    return gal_error_code::SUC;
}

gal_error_code d3d12_destroy_buffer([[maybe_unused]] gal_context context, gal_buffer buffer) {
    if (buffer != nullptr) {
        d3d12_buffer *d3d12_buf = reinterpret_cast<d3d12_buffer *>(buffer);
        d3d12_buf->buffer->Release();
        d3d12_buf->allocation->Release();
        delete d3d12_buf;
        buffer = nullptr;
    }
    return gal_error_code::SUC;
}

gal_error_code d3d12_create_texture(gal_context context, gal_texture_desc *desc, gal_texture *texture) {
    // TODO(hyl5): finish this
    if (context)
        return gal_error_code::SUC;
    else if (texture)
        return gal_error_code::SUC;
    else if (desc)
        return gal_error_code::SUC;
    return gal_error_code::SUC;
}

gal_error_code d3d12_destroy_texture(gal_context context, gal_texture texture) {
    // TODO(hyl5): finish this
    if (context)
        return gal_error_code::SUC;
    else if (texture)
        return gal_error_code::SUC;
    return gal_error_code::SUC;
}

gal_error_code d3d12_create_sampler(gal_context context, gal_sampler_desc *sampler_desc, gal_sampler *sampler) {
    if (context)
        return gal_error_code::SUC;
    else if (sampler)
        return gal_error_code::SUC;
    else if (sampler_desc)
        return gal_error_code::SUC;
    return gal_error_code::SUC;
    //    d3d12_context *d3d12_ctx = reinterpret_cast<d3d12_context *>(context);
    //*sampler = reinterpret_cast<gal_handle>(new ante::gal::d3d12_sampler);
    //d3d12_buffer *d3d12_spl = reinterpret_cast<d3d12_buffer *>(*sampler);
    //// d3d12_context *d3d12_ctx;
    //d3d12_ctx->device->CreateSampler();
    return gal_error_code::SUC;
}
gal_error_code d3d12_destroy_sampler(gal_context context, gal_sampler sampler) {
    //d3d12_context *d3d12_ctx;
    return gal_error_code::SUC;
}
gal_error_code d3d12_create_render_target(gal_context context, gal_render_target_desc *desc,
                                          gal_render_target *render_target) {
    //d3d12_context *d3d12_ctx;
    return gal_error_code::SUC;
}
gal_error_code d3d12_destroy_render_target(gal_context context, gal_render_target render_target) {
    // d3d12_context *d3d12_ctx;
    return gal_error_code::SUC;
}
// surface
gal_error_code d3d12_create_swap_chain(gal_context context, gal_swap_chain_desc *desc, gal_swap_chain *swap_chain) {
    //d3d12_context *d3d12_ctx;
    //d3d12_ctx->factory->CreateSwapChain();
    //
    //	SwapChain* pSwapChain = (SwapChain*)tf_calloc(1, sizeof(SwapChain) + pDesc->mImageCount * sizeof(RenderTarget*));
    //	ASSERT(pSwapChain);
    //	pSwapChain->ppRenderTargets = (RenderTarget**)(pSwapChain + 1);
    //	ASSERT(pSwapChain->ppRenderTargets);
    //
    //	pSwapChain->mD3D12.mDxSyncInterval = pDesc->mEnableVsync ? 1 : 0;
    //
    //	DXGI_SWAP_CHAIN_DESC1 desc = {};
    //	desc.Width = pDesc->mWidth;
    //	desc.Height = pDesc->mHeight;
    //	desc.Format = util_to_dx12_swap_chain_format(pDesc->mColorFormat);
    //	desc.Stereo = false;
    //	desc.SampleDesc.Count = 1;    // If multisampling is needed, we'll resolve it later
    //	desc.SampleDesc.Quality = 0;
    //	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    //	desc.BufferCount = pDesc->mImageCount;
    //	desc.Scaling = DXGI_SCALING_STRETCH;
    //	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    //	desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    //	desc.Flags = 0;
    //
    //	BOOL allowTearing = FALSE;
    //	pRenderer->mD3D12.pDXGIFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
    //	desc.Flags |= allowTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
    //
    //	pSwapChain->mD3D12.mFlags |= (!pDesc->mEnableVsync && allowTearing) ? DXGI_PRESENT_ALLOW_TEARING : 0;
    //
    //	IDXGISwapChain1* swap_chain;
    //
    //	HWND hwnd = (HWND)pDesc->mWindowHandle.window;
    //
    //	(pRenderer->mD3D12.pDXGIFactory->CreateSwapChainForHwnd(
    //		pDesc->ppPresentQueues[0]->mD3D12.pDxQueue, hwnd, &desc, NULL, NULL, &swap_chain));
    //
    //	CHECK_HRESULT(pRenderer->mD3D12.pDXGIFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));
    //
    //	CHECK_HRESULT(swap_chain->QueryInterface(IID_ARGS(&pSwapChain->mD3D12.pDxSwapChain)));
    //	swap_chain->Release();
    //
    //
    //	ID3D12Resource** buffers = (ID3D12Resource**)alloca(pDesc->mImageCount * sizeof(ID3D12Resource*));
    //
    //	// Create rendertargets from swap_chain
    //	for (uint32_t i = 0; i < pDesc->mImageCount; ++i)
    //	{
    //		(pSwapChain->mD3D12.pDxSwapChain->GetBuffer(i, IID_ARGS(&buffers[i])));
    //	}
    //
    //
    //	RenderTargetDesc descColor = {};
    //	descColor.mWidth = pDesc->mWidth;
    //	descColor.mHeight = pDesc->mHeight;
    //	descColor.mDepth = 1;
    //	descColor.mArraySize = 1;
    //	descColor.mFormat = pDesc->mColorFormat;
    //	descColor.mClearValue = pDesc->mColorClearValue;
    //	descColor.mSampleCount = SAMPLE_COUNT_1;
    //	descColor.mSampleQuality = 0;
    //	descColor.pNativeHandle = NULL;
    //	descColor.mFlags = TEXTURE_CREATION_FLAG_ALLOW_DISPLAY_TARGET;
    //	descColor.mStartState = RESOURCE_STATE_PRESENT;
    //#if defined(XBOX)
    //	descColor.mFlags |= TEXTURE_CREATION_FLAG_OWN_MEMORY_BIT;
    //	pSwapChain->mD3D12.pPresentQueue = pDesc->mPresentQueueCount ? pDesc->ppPresentQueues[0] : NULL;
    //#endif
    //
    //	for (uint32_t i = 0; i < pDesc->mImageCount; ++i)
    //	{
    //#if !defined(XBOX)
    //		descColor.pNativeHandle = (void*)buffers[i];
    //#endif
    //		::addRenderTarget(pRenderer, &descColor, &pSwapChain->ppRenderTargets[i]);
    //	}

    return gal_error_code::SUC;
}
gal_error_code d3d12_destroy_swap_chain(gal_context context, gal_swap_chain swap_chain) { return gal_error_code::SUC; }

// pipeline
gal_error_code d3d12_create_shader_program(gal_context context, gal_shader_program_desc *desc,
                                           gal_shader_program *shader_program) {
    //d3d12_context *d3d12_ctx;

    return gal_error_code::SUC;
}
gal_error_code d3d12_destroy_shader_program(gal_context context, gal_shader_program shader_program) {
    return gal_error_code::SUC;
}

gal_error_code d3d12_create_pipeline_cache(gal_context context, gal_pipeline_cache_desc *desc,
                                           gal_pipeline_cache *pipeline_cache) {
    return gal_error_code::SUC;
}
gal_error_code d3d12_destroy_pipeline_cache(gal_context context, gal_pipeline_cache _pipeline_cache) {

    return gal_error_code::SUC;
}
gal_error_code d3d12_get_pipeline_cache_data(gal_context context, gal_pipeline_cache _pipeline_cache, u64 *_size,
                                             void *_data) {

    return gal_error_code::SUC;
}
gal_error_code d3d12_create_compute_pipeline(gal_context context, gal_pipeline_desc *desc, gal_pipeline *pipeline) {
    // d3d12_context *d3d12_ctx;
    //d3d12_ctx->device->CreateGraphicsPipelineState();
    //d3d12_ctx->device->CreateComputePipelineState();
    return gal_error_code::SUC;
}

gal_error_code d3d12_create_graphics_pipeline(gal_context context, gal_pipeline_desc *desc, gal_pipeline *pipeline) {

    // d3d12_context *d3d12_ctx;
    //d3d12_ctx->device->CreateGraphicsPipelineState();
    //d3d12_ctx->device->CreateComputePipelineState();
    return gal_error_code::SUC;
}
gal_error_code d3d12_destroy_pipeline(gal_context context, gal_pipeline pipeline) {
    //d3d12_context *d3d12_ctx;
    return gal_error_code::SUC;
}
// gal_error_code d3d12_create_descriptor_pool() {
//    //d3d12_context *d3d12_ctx;
//    //d3d12_ctx->device->CreateDescriptorHeap();
//    return gal_error_code::SUC;
// }
// gal_error_code d3d12_destroy_descriptor_pool() {
//    //d3d12_context *d3d12_ctx;

//    return gal_error_code::SUC;
// }
gal_error_code d3d12_get_descriptor_set(gal_context context, gal_descriptor_set_desc *desc, u32 set_count,
                                        gal_descriptor_set *sets) {
    //d3d12_context *d3d12_ctx;
    return gal_error_code::SUC;
}
gal_error_code d3d12_free_descriptor_set(gal_context context, gal_descriptor_set set, bool free_all_pool) {
    //d3d12_context *d3d12_ctx;
    return gal_error_code::SUC;
}

gal_error_code d3d12_update_descriptor_set(gal_context context, gal_descriptor_set_update_desc *update_desc,
                                           gal_descriptor_set set) {
    return gal_error_code::SUC;
}

gal_error_code d3d12_create_root_signature(gal_context context, gal_root_signature_desc *desc,
                                           gal_root_signature *root_signature) {
    //d3d12_context *d3d12_ctx;
    //d3d12_ctx->device->Createroot_signature();
    return gal_error_code::SUC;
}
gal_error_code d3d12_destroy_root_signature(gal_context context, gal_root_signature root_signature) {
    //d3d12_context *d3d12_ctx;
    return gal_error_code::SUC;
}
// sync
gal_error_code d3d12_create_fence(gal_context context, gal_fence *fence) {
    //d3d12_context *d3d12_ctx;
    //d3d12_fence *ded12_fc;
    //(d3d12_ctx->device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&ded12_fc->fence)));
    //ded12_fc->fence_value = 1;

    //ded12_fc->wait_idle_fence_event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    //if (ded12_fc->wait_idle_fence_event == nullptr) {
    //    (HRESULT_FROM_WIN32(GetLastError()));
    //}
    return gal_error_code::SUC;
}
gal_error_code d3d12_wait_fences(gal_context context, gal_fence *fences, u32 count) { return gal_error_code::SUC; }
gal_error_code d3d12_getFenceStatus(gal_context context, gal_fence fence, gal_fence_status *fence_status) {
    return gal_error_code::SUC;
}

gal_error_code d3d12_destroy_fence(gal_context context, gal_fence fence) {
    //    (gpu_fence->Release());
    //CloseHandle(wait_idle_fence_event);
    return gal_error_code::SUC;
}
gal_error_code d3d12_wait_gpu() {
    //d3d12_context *d3d12_ctx;
    return gal_error_code::SUC;
}
gal_error_code d3d12_create_semaphore(gal_context context, gal_semaphore *semaphore) {
    //d3d12_context *d3d12_ctx;
    return gal_error_code::SUC;
}
gal_error_code d3d12_destroy_semaphore(gal_context context, gal_semaphore semaphore) {
    //d3d12_context *d3d12_ctx;
    return gal_error_code::SUC;
}
// cmds
gal_error_code d3d12_create_command_pool(gal_context context, gal_command_pool_desc *desc,
                                         gal_command_pool *command_pool) {
    //d3d12_context *d3d12_ctx;
    //d3d12_ctx->device->CreateCommandAllocator();
    return gal_error_code::SUC;
}
gal_error_code d3d12_reset_command_pool(gal_context context, gal_command_pool command_pool) {
    //d3d12_context *d3d12_ctx;

    return gal_error_code::SUC;
}
gal_error_code d3d12_destroy_command_pool(gal_context context, gal_command_pool command_pool) {
    //d3d12_context *d3d12_ctx;
    return gal_error_code::SUC;
}
gal_error_code d3d12_get_command_list(gal_context context, gal_command_list_desc *desc, gal_command_list *command) {
    //d3d12_context *d3d12_ctx;

    return gal_error_code::SUC;
}
gal_error_code d3d12_free_command_list(gal_context context, gal_command_list command) {
    //d3d12_context *d3d12_ctx;
    //d3d12_command_list *d3d12_cmd;
    //d3d12_cmd->command_list->Release();
    //d3d12_cmd->command_list = nullptr;

    return gal_error_code::SUC;
}
gal_error_code d3d12_cmd_begin(gal_command_list command) {
    //d3d12_context *d3d12_ctx;
    //d3d12_command_list *d3d12_cmd;
    //d3d12_cmd->command_list->Reset();
    return gal_error_code::SUC;
}
gal_error_code d3d12_cmd_end(gal_command_list command) {
    //d3d12_context *d3d12_ctx;
    //d3d12_command_list *d3d12_cmd;
    //d3d12_cmd->command_list->Close();
    return gal_error_code::SUC;
}
gal_error_code d3d12_cmd_resource_barrier(gal_command_list command, u32 buffer_barrier_count,
                                          gal_buffer_barrier *buffer_barriers, u32 texture_barrier_count,
                                          gal_texture_barrier *texture_barriers) {
    //d3d12_context *d3d12_ctx;
    //d3d12_command_list *d3d12_cmd;
    //d3d12_cmd->command_list->ResourceBarrier();
    return gal_error_code::SUC;
}
gal_error_code d3d12_cmd_bind_index_buffer(gal_command_list command, gal_buffer index_buffer, gal_index_type index_type,
                                           u64 offset) {
    //d3d12_context *d3d12_ctx;
    //d3d12_command_list *d3d12_cmd;
    //d3d12_cmd->command_list->IASetIndexBuffer();
    return gal_error_code::SUC;
}
gal_error_code d3d12_cmd_bind_vertex_buffer(gal_command_list command, u32 vertex_buffer_count,
                                            gal_buffer *vertex_buffers, [[maybe_unused]] u32 *strides, u64 *offsets) {
    //d3d12_context *d3d12_ctx;
    //d3d12_command_list *d3d12_cmd;
    //d3d12_cmd->command_list->IASetVertexBuffers();
    return gal_error_code::SUC;
}
gal_error_code d3d12_cmd_bind_descriptor_set() {
    //d3d12_context *d3d12_ctx;
    //d3d12_command_list *d3d12_cmd;
    return gal_error_code::SUC;
}

gal_error_code d3d12_cmd_bind_push_constant(gal_command_list command, gal_root_signature root_signature,
                                            const char *name, void *data, u64 size) {
    return gal_error_code::SUC;
}

gal_error_code d3d12_cmd_bind_pipeline(gal_command_list command, gal_pipeline pipeline) {
    //d3d12_context *d3d12_ctx;
    //d3d12_command_list *d3d12_cmd;
    //d3d12_cmd->command_list->SetPipelineState();
    //d3d12_cmd->command_list->SetPipelineState1();
    return gal_error_code::SUC;
}

gal_error_code d3d12_cmd_begin_renderpass(gal_command_list command, gal_renderpass_begin_desc *desc) {
    return gal_error_code::SUC;
}

gal_error_code d3d12_cmd_end_renderpass(gal_command_list command) {
    //d3d12_context *d3d12_ctx;
    //d3d12_command_list *d3d12_cmd;
    //d3d12_cmd->command_list->BeginRenderPass();
    return gal_error_code::SUC;
}
// gal_error_code d3d12_cmd_end_renderpass() {
//     //d3d12_context *d3d12_ctx;
//     //d3d12_command_list *d3d12_cmd;
//     //d3d12_cmd->command_list->EndRenderPass();
//     return gal_error_code::SUC;
// }
gal_error_code d3d12_cmd_dispatch(gal_command_list command, u32 group_count_x, u32 group_count_y, u32 group_count_z) {
    //d3d12_context *d3d12_ctx;
    //d3d12_command_list *d3d12_cmd;
    //
    //d3d12_cmd->command_list->Dispatch();
    return gal_error_code::SUC;
}
// gal_error_code d3d12_cmd_dispatch_indirect() {
//     //d3d12_context *d3d12_ctx;
//     //d3d12_command_list *d3d12_cmd;
//     //d3d12_cmd->command_list->ExecuteIndirect();
//     return gal_error_code::SUC;
// }
gal_error_code d3d12_cmd_draw_instanced(gal_command_list command, u32 vertex_count, u32 first_vertex,
                                        u32 instance_count, u32 first_instance) {
    //d3d12_context *d3d12_ctx;
    //d3d12_command_list *d3d12_cmd;
    //d3d12_cmd->command_list->DrawInstanced();
    return gal_error_code::SUC;
}
gal_error_code d3d12_cmd_draw_indexed_instanced(gal_command_list command, u32 index_count, u32 first_index,
                                                u32 instance_count, u32 first_instance, u32 first_vertex) {
    //d3d12_context *d3d12_ctx;
    //d3d12_command_list *d3d12_cmd;
    //d3d12_cmd->command_list->DrawIndexedInstanced();
    return gal_error_code::SUC;
}
// gal_error_code d3d12_cmd_draw_indirect_instanced() {
//     // d3d12_context *d3d12_ctx;
//     return gal_error_code::SUC;
// }
// gal_error_code d3d12_cmd_draw_indirect_indexed_instanced() {
//     //d3d12_context *d3d12_ctx;
//     return gal_error_code::SUC;
// }
// gal_error_code d3d12_cmd_draw_mesh_task() {
//     //d3d12_context *d3d12_ctx;
//     //d3d12_command_list *d3d12_cmd;
//     //d3d12_cmd->command_list->DispatchMesh();
//     return gal_error_code::SUC;
// }
// gal_error_code d3d12_cmd_draw_indirect_mesh_task() {
//     //d3d12_context *d3d12_ctx;
//     return gal_error_code::SUC;
// }
gal_error_code d3d12_cmd_copy_texture(gal_command_list command) {
    //d3d12_context *d3d12_ctx;
    //d3d12_command_list *d3d12_cmd;
    //d3d12_cmd->command_list->CopyBufferRegion();
    //d3d12_cmd->command_list->CopyResource();
    return gal_error_code::SUC;
}
gal_error_code d3d12_cmd_copy_buffer(gal_command_list command, gal_buffer src, gal_buffer dst, u64 src_offset,
                                     u64 dst_offset, u64 size) {
    //d3d12_context *d3d12_ctx;
    //d3d12_cmd->command_list->CopyTextureRegion();
    //d3d12_cmd->command_list->CopyResource();
    return gal_error_code::SUC;
}
// TODO(hyl5): deprecate the method, use compute shader instead
gal_error_code d3d12_cmd_fill_buffer(gal_command_list command) {
    //d3d12_context *d3d12_ctx;
    //d3d12_command_list *d3d12_cmd;
    //d3d12_cmd->command_list->ClearUnorderedAccessViewFloat();
    //d3d12_cmd->command_list->ClearUnorderedAccessViewUint();
    return gal_error_code::SUC;
}
gal_error_code d3d12_cmd_fill_texture(gal_command_list command) {
    //d3d12_context *d3d12_ctx;
    //d3d12_command_list *d3d12_cmd;
    //d3d12_cmd->command_list->ClearUnorderedAccessViewFloat();
    //d3d12_cmd->command_list->ClearUnorderedAccessViewUint();
    //d3d12_cmd->command_list->ClearRenderTargetView();
    //d3d12_cmd->command_list->ClearDepthStencilView();
    return gal_error_code::SUC;
}

gal_error_code d3d12_cmd_update_subresources(gal_command_list command, gal_texture dst, gal_buffer src,
                                             u32 subresource_count, gal_texture_subresource_desc *descs) {
    //d3d12_context *d3d12_ctx;
    //d3d12_command_list *d3d12_cmd;
    //d3d12_cmd->command_list->CopyBufferRegion();
    return gal_error_code::SUC;
}
gal_error_code d3d12_cmd_copy_texture_to_buffer(gal_command_list command) {
    //d3d12_context *d3d12_ctx;
    //d3d12_command_list *d3d12_cmd;
    //d3d12_cmd->command_list->CopyTextureRegion();
    return gal_error_code::SUC;
}

gal_error_code d3d12_unmapbuffer(gal_context context, gal_buffer buffer) { return gal_error_code::SUC; }

gal_error_code d3d12_mapbuffer(gal_context context, gal_buffer buffer, read_range *range) {
    return gal_error_code::SUC;
}

gal_error_code d3d12_add_queue(gal_context context, gal_queue_desc *desc, gal_queue *queue) {
    return gal_error_code::SUC;
}
gal_error_code d3d12_remove_queue(gal_context context, gal_queue queue) { return gal_error_code::SUC; }

gal_error_code d3d12_queue_submit(gal_queue queue, gal_queue_submit_desc *desc) { return gal_error_code::SUC; }

gal_error_code d3d12_queue_present(gal_queue queue, gal_queue_present_desc *desc) { return gal_error_code::SUC; }

gal_error_code d3d12_cmd_upload_buffer(gal_command_list command) {
    if (command != nullptr) {
        return gal_error_code::ERR;
    }
    return gal_error_code::SUC;
}
gal_error_code d3d12_cmd_upload_texture(gal_command_list command) {
    if (command != nullptr) {
        return gal_error_code::ERR;
    }
    return gal_error_code::SUC;
}

gal_error_code d3d12_cmd_set_viewport(gal_command_list command, f32 x, f32 y, f32 width, f32 height, f32 min_depth,
                                      f32 max_depth) {
    return gal_error_code::SUC;
}

gal_error_code d3d12_cmd_set_scissor(gal_command_list command, i32 x, i32 y, u32 width, u32 height) {

    return gal_error_code::SUC;
}

gal_error_code d3d12_cmd_set_stencil_reference_value(gal_command_list command, u32 val) { return gal_error_code::SUC; }

gal_error_code d3d12_acquire_next_image(gal_context context, gal_swap_chain swap_chain, gal_semaphore signal_semaphore,
                                     gal_fence fence, uint32_t *image_index) {
    return gal_error_code::SUC;
}

} // namespace ante::gal
