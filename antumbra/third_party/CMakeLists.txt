project(ant_third_party)

add_library(${PROJECT_NAME} third_party_dummy.cpp TracyClient.cpp)

add_subdirectory(glfw) # CMakeLists of glfw added some useless projects/folders in root directory
add_subdirectory(json)
option(MI_BUILD_TESTS "Build test executables" OFF)
option(MI_BUILD_SHARED "Build shared library" OFF)
option(MI_BUILD_OBJECT "" OFF)
add_subdirectory(mimalloc)
add_subdirectory(spdlog)
add_subdirectory(SPIRV-Cross)
add_subdirectory(Catch2)
option(FLECS_SHARED "Build static flecs lib" OFF)
add_subdirectory(flecs)
include(lua.cmake)
add_subdirectory(sol2)
option(TRACY_ENABLE "" ON)
option(TRACY_ONDEMAND "" ON)
add_subdirectory(tracy)
#add_subdirectory(JoltPhysics/Build)
option(TINYGLTF_BUILD_LOADER_EXAMPLE "" OFF)
add_subdirectory(tinygltf)

#message("downloading dxc")
#file(DOWNLOAD "https://github.com/microsoft/DirectXShaderCompiler/releases/download/v1.7.2212.1/dxc_2023_03_01.zip")



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
        find_library(Vulkan_LIBRARY NAMES vulkan-1 PATHS libs/windows/) 
        if (Vulkan_LIBRARY)
            message("using bundled vulkan")
            target_link_libraries(${PROJECT_NAME} PUBLIC ${Vulkan_LIBRARY})
        else()
            message("failed to find vulkan library")
        endif()
    endif()
endif()

if(ANTUMBRA_BUILD_D3D12)
    add_subdirectory(D3D12MemoryAllocator)
    option(BUILD_TESTING "Build testing" OFF)
    add_subdirectory(DirectXTK12)
endif()

# add include dirs
target_include_directories(${PROJECT_NAME} PUBLIC
    spdlog/include
    glm/glm
    glfw/include
    json/include
    VulkanMemoryAllocator/include
    tracy/public
    magic_enum/include
    sol2/include
    mimalloc/include
    #JoltPhysics/ # this will include some useless folder
    lua
)

if(ANTUMBRA_BUILD_D3D12)
    target_include_directories(${PROJECT_NAME} PUBLIC
        D3D12MemoryAllocator/include
        DirectXTK12/Inc
        DirectXTK12/Src
    )
endif()

# link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC
    mimalloc-static
    glfw
    spdlog
    flecs_static
    nlohmann_json
    TracyClient
    lua_static
    sol2
    #Jolt
    tinygltf
)

if(ANTUMBRA_BUILD_D3D12)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        D3D12MemoryAllocator
        DirectXTK12
    )
endif()

set_property(TARGET
    ${PROJECT_NAME}
    spdlog
    Catch2
    Catch2WithMain
    glfw
    mimalloc-static
    flecs_static
    spirv-cross
    spirv-cross-c
    spirv-cross-c
    spirv-cross-core
    spirv-cross-cpp
    spirv-cross-glsl
    spirv-cross-hlsl
    spirv-cross-msl
    spirv-cross-reflect
    spirv-cross-util
    TracyClient
    # Jolt
    tinygltf
    lua_static
    PROPERTY FOLDER "third_party")

if(ANTUMBRA_BUILD_D3D12)
    set_property(TARGET D3D12MemoryAllocator DirectXTK12
        PROPERTY FOLDER "third_party")
endif(ANTUMBRA_BUILD_D3D12)


 



