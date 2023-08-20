#pragma once

#include <Windows.h>
#include <d3dx12.h>
#include <dxgi1_6.h>

#include "../enum.h"
#include "../gal.h"

namespace ante::gal {

#define DECLARE_D3D12_HANDLE(name) struct d3d12_##name : public gal_##name##_T

DECLARE_D3D12_HANDLE(queue) {

};

DECLARE_D3D12_HANDLE(context) {
    IDXGIFactory6 *factory = nullptr;
    ID3D12Device *device = nullptr;
    IDXGIAdapter4 *active_gpu = nullptr;
    D3D12MA::Allocator *d3dma_allocator = nullptr;

    ID3D12CommandQueue *graphics_queue = nullptr;
    ID3D12CommandQueue *compute_queue = nullptr;
    ID3D12CommandQueue *transfer_queue = nullptr;
};

using dx_descriptor_id = i32;

DECLARE_D3D12_HANDLE(buffer) {
    ID3D12Resource *buffer = nullptr;
    D3D12MA::Allocation *allocation = nullptr;
    D3D12_GPU_VIRTUAL_ADDRESS mDxGpuAddress;
    /// Descriptor handle of the CBV in a CPU visible descriptor heap (applicable to BUFFER_USAGE_UNIFORM)
    dx_descriptor_id mDescriptors;
    /// Offset from mDxDescriptors for srv descriptor handle
    u8 mSrvDescriptorOffset;
    /// Offset from mDxDescriptors for uav descriptor handle
    u8 mUavDescriptorOffset;
};


DECLARE_D3D12_HANDLE(texture) {
    /// Native handle of the underlying resource
    ID3D12Resource *texture = nullptr;
    D3D12MA::Allocation *allocation = nullptr;
    dx_descriptor_id mDescriptors;
    u32 mHandleCount : 24;
    u32 mUavStartIndex;
};

DECLARE_D3D12_HANDLE(sampler) {
    ID3D12Resource *sampler = nullptr;
    D3D12MA::Allocation *allocation = nullptr;
};

DECLARE_D3D12_HANDLE(render_target){
};

DECLARE_D3D12_HANDLE(swap_chain) { IDXGISwapChain3 *gpu_swap_chain; };

DECLARE_D3D12_HANDLE(fence) {
    ID3D12Fence *fence = nullptr;
    HANDLE wait_idle_fence_event = nullptr;
    UINT64 fence_value = 0;
};

DECLARE_D3D12_HANDLE(semaphore){};

DECLARE_D3D12_HANDLE(command_pool){};

DECLARE_D3D12_HANDLE(command_list) { ID3D12GraphicsCommandList6 *command_list; };

DECLARE_D3D12_HANDLE(shader_program){};

DECLARE_D3D12_HANDLE(root_signature){};

DECLARE_D3D12_HANDLE(pipeline){};
DECLARE_D3D12_HANDLE(pipeline_cache){};

DECLARE_D3D12_HANDLE(descriptor_set){};
} // namespace ante::gal