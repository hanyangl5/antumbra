project(ant_third_party)

add_subdirectory(glfw) # CMakeLists of glfw added some useless projects/folders in root directory
#add_subdirectory(json)
option(MI_BUILD_TESTS "Build test executables" OFF)
option(MI_BUILD_SHARED "Build shared library" OFF)
option(MI_BUILD_OBJECT "" OFF)

add_subdirectory(spdlog)
option(SPIRV_CROSS_CLI "Build the CLI binary. Requires SPIRV_CROSS_STATIC." OFF)
option(SPIRV_CROSS_ENABLE_TESTS "Enable SPIRV-Cross tests." OFF)
option(SPIRV_CROSS_ENABLE_GLSL "Enable GLSL support." OFF)
option(SPIRV_CROSS_ENABLE_HLSL "Enable HLSL target support." OFF)
option(SPIRV_CROSS_ENABLE_MSL "Enable MSL target support." OFF)
option(SPIRV_CROSS_ENABLE_CPP "Enable C++ target support." off)
option(SPIRV_CROSS_ENABLE_REFLECT "Enable JSON reflection target support." OFF)
option(SPIRV_CROSS_ENABLE_C_API "Enable C API wrapper support in static library." OFF)
option(SPIRV_CROSS_ENABLE_UTIL "Enable util module support." OFF)

add_subdirectory(SPIRV-Cross)
add_subdirectory(doctest)

add_library(${PROJECT_NAME} third_party_dummy.cpp)


# dxc

set(ANT_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/antumbralibs)
set(VK_LIB_PATH ${ANT_LIB_PATH}/vulkan)

if(WIN32)
    set(DXC_LIB_PATH ${ANT_LIB_PATH}/dxc_2023_03_01/windows CACHE INTERNAL "dxc lib path")
    find_library(DXC_LIBRARY dxcompiler PATHS ${DXC_LIB_PATH}/lib/x64)
elseif(APPLE)
    set(DXC_LIB_PATH ${ANT_LIB_PATH}/dxc_2023_03_01/macos CACHE INTERNAL "dxc lib path")
    find_library(DXC_LIBRARY dxcompiler PATHS ${DXC_LIB_PATH}/lib)
elseif(LINUX)
    set(DXC_LIB_PATH ${ANT_LIB_PATH}/dxc_2023_03_01/linux CACHE INTERNAL "dxc lib path")
    find_library(DXC_LIBRARY dxcompiler PATHS ${DXC_LIB_PATH}/lib)
endif()


if(DXC_LIBRARY)
    message("found dxcompiler in ${DXC_LIB_PATH}")
    target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${DXC_LIB_PATH}/include)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${DXC_LIBRARY})
else()
    message("dxcompiler not found")
endif()

if(LINUX)
    set(DXC_LIB_PATH ${ANT_LIB_PATH}/dxc_2023_03_01/linux CACHE INTERNAL "dxil lib path")
    find_library(DXIL_LIBRARY dxil PATHS ${DXC_LIB_PATH}/lib)
endif()


if(DXIL_LIBRARY)
    message("found libdxil in ${DXIL_LIBRARY}")
    target_link_libraries(${PROJECT_NAME} PUBLIC ${DXIL_LIBRARY})
else()
    message("libdxil not found")
endif()

if(ANTUMBRA_BUILD_VK)
    # prefer to use system installed vulkan sdk
    find_package(Vulkan)
    if(Vulkan_FOUND)
        message("found vulkan sdk")
        target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
        target_link_libraries(${PROJECT_NAME} PUBLIC ${Vulkan_LIBRARIES})
    else()
        target_include_directories(${PROJECT_NAME} PUBLIC
        Vulkan-Headers/include)
        if(WIN32)
            find_library(Vulkan_LIBRARY NAMES vulkan-1 PATHS ${VK_LIB_PATH}/windows) 
        elseif(LINUX)
            find_library(Vulkan_LIBRARY NAMES vulkan PATHS ${VK_LIB_PATH}/linux) 
        endif()
        if (Vulkan_LIBRARY)
            message("using bundled vulkan in ${VK_LIB_PATH}")
            target_link_libraries(${PROJECT_NAME} PUBLIC ${Vulkan_LIBRARY})
        else()
            message("failed to find vulkan library")
        endif()
    endif()
endif()

if(ANTUMBRA_BUILD_D3D12)
    add_subdirectory(D3D12MemoryAllocator)
    option(BUILD_TESTING "Build testing" OFF)
    #add_subdirectory(DirectXTK12)
endif()

# add include dirs
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
    spdlog/include
    glm/glm
    glfw/include
    #json/include
    #doctest/doctest
    VulkanMemoryAllocator/include
    SPIRV-Cross/
    lua
)

if(ANTUMBRA_BUILD_D3D12)
    target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
        D3D12MemoryAllocator/include
        DirectXTK12/Inc
        DirectXTK12/Src
    )
endif()

# link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC
    #mimalloc-static
    glfw
    spdlog
    #doctest_with_main
    #nlohmann_json
    #Jolt
    spirv-cross-core
)

if(ANTUMBRA_BUILD_D3D12)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        D3D12MemoryAllocator
        #DirectXTK12
    )
endif()

set_property(TARGET
    ${PROJECT_NAME}
    spdlog

    # Catch2
    # Catch2WithMain
    doctest_with_main
    glfw

    # spirv-cross
    # spirv-cross-c
    # spirv-cross-c
    spirv-cross-core

    # spirv-cross-cpp
    # spirv-cross-glsl
    # spirv-cross-hlsl
    # spirv-cross-msl
    # spirv-cross-reflect
    # spirv-cross-util

    # Jolt
    PROPERTY FOLDER "third_party")

if(ANTUMBRA_BUILD_D3D12)
    set_property(TARGET D3D12MemoryAllocator
        PROPERTY FOLDER "third_party")
endif(ANTUMBRA_BUILD_D3D12)


 
# boost pmr
if(APPLE)
    find_package(Boost REQUIRED COMPONENTS container)
    if (Boost_FOUND)
        message("found boost container :${Boost_INCLUDE_DIR}")
        target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${Boost_INCLUDE_DIR})
        target_link_libraries (${PROJECT_NAME} PUBLIC  Boost::container)
    endif()
endif()