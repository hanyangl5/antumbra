#pragma once

// standard libraries
#include <memory_resource>

// third party libraries
#include <D3D12MemAlloc.h>
#include <Windows.h>
#include <d3dx12.h>
#include <dxgi1_6.h>

// project headers
#include <framework/01core/utils/utils.h>
#include <framework/02gal/gal.h>

namespace ant::gal {

#define DECLARE_D3D12_FUNCTION(ret, name, ...) ret d3d12_##name(__VA_ARGS__);

struct d3d12_gal_context {
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

struct d3d12_command_list {
    ID3D12GraphicsCommandList6 *gpu_command_list;
};
gal_error_code d3d12_init_gal(gal_context *context);

#define D3D12_FUNCTION_DECLARE
#include "../helper/helper_macro.h"
#undef D3D12_FUNCTION_DECLARE

} // namespace ant::gal
