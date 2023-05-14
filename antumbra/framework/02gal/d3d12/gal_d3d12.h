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

struct d3d12_gal_context
{
    IDXGIFactory6 *factory;
    ID3D12Device *device;
    IDXGIAdapter4 *active_gpu;
    D3D12MA::Allocator *d3dma_allocator;
};

struct d3d12_command_list
{
    ID3D12GraphicsCommandList6 *gpu_command_list;
};
gal_error_code d3d12_init_gal(gal_context *);

DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, create_instance, GalDesc *gal_desc, gal_context *context);
DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, destroy_instane, gal_context *);

DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, selete_gpu, GalDesc *desc, gal_context *context);
DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, create_device, GalDesc *gal_desc, gal_context *context);
DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, destroy_device, gal_context * context);
DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, init_memory_allocator, gal_context * CONTEXT);

DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, create_buffer);

DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, destroy_buffer);

DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, create_texture);

DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, destroy_texture);

DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, create_sampler);

DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, destroy_sampler);

DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, create_swap_chain);

DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, destroy_swap_chain);

DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, create_shader);

DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, destroy_shader);

DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, create_pipeline);

DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, destroy_pipeline);

DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, create_surface);

DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, destroy_surface);

// cmds
DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, cmd_begin_recording);
DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, cmd_end_recording);
DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, bind_descriptor_set);
DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, bind_index_buffer);
DECLARE_D3D12_FUNCTION(ant::gal::gal_error_code, bind_index_buffer);
}// namespace ant::gal
