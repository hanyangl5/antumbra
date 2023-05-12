project(ant_third_party)

add_library(${PROJECT_NAME} third_party_dummy.cpp)

find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${Vulkan_LIBRARIES})

add_subdirectory(glfw)
add_subdirectory(json)
add_subdirectory(mimalloc)
add_subdirectory(spdlog)
add_subdirectory(SPIRV-Cross)
add_subdirectory(VulkanMemoryAllocator)
add_subdirectory(Catch2)
add_subdirectory(flecs)

if(WIN32)
    add_subdirectory(D3D12MemoryAllocator)
    add_subdirectory(DirectXTK12)
endif()

# add include dirs
target_include_directories(${PROJECT_NAME} PUBLIC
    spdlog/include
    glm/glm
    glfw/include
    json/include
    VulkanMemoryAllocator/include
)

if(WIN32)
    target_include_directories(${PROJECT_NAME} PUBLIC
        D3D12MemoryAllocator/include
        DirectXTK12/Inc
    )
endif()

# link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC
    mimalloc
    glfw
    spdlog
    mimalloc-static
    flecs_static
    VulkanMemoryAllocator
    nlohmann_json
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        D3D12MemoryAllocator
        DirectXTK12
    )
endif(WIN32)

set_property(TARGET
    ${PROJECT_NAME}
    mimalloc
    spdlog
    spirv-cross
    spirv-cross-c
    spirv-cross-core
    spirv-cross-cpp
    spirv-cross-glsl
    spirv-cross-hlsl
    spirv-cross-msl
    spirv-cross-reflect
    spirv-cross-util
    VulkanMemoryAllocator
    glfw
    mimalloc-static
    mimalloc-obj
    spirv-cross-c
    Catch2
    Catch2WithMain
    flecs
    flecs_static
    PROPERTY FOLDER "third_party")

if(WIN32)
    set_property(TARGET D3D12MemoryAllocator DirectXTK12
        PROPERTY FOLDER "third_party")
endif(WIN32)