if (ANTUMBRA_BUILD_D3D12)
    set(__ANTUMBRA_BUILD_D3D12__ 1)
else()
    set(__ANTUMBRA_BUILD_D3D12__ 0)
endif()

if(ANTUMBRA_BUILD_VK)
    set(__ANTUMBRA_BUILD_VK__ 1)
else()
    set(__ANTUMBRA_BUILD_VK__ 1)
endif()

configure_file(config.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp)