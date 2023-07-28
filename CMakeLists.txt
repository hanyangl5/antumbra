cmake_minimum_required(VERSION 3.21)

# Set the project name and language
project(antumbra)

include(cmake/Doxygen.cmake)

include(cmake/ProjectOptions.cmake)

include(cmake/CopyDll.cmake)

set(ANTUMBRA_PROJECT_DIR .)
set(ANTUMBRA_SRC_DIR antumbra)
set(ANTUMBRA_THIR_PARTY_DIR ${ANTUMBRA_SRC_DIR}/third_party)
set(ANTUMBRA_BUILD_VK ON)

if(WIN32)
    set(ANTUMBRA_BUILD_D3D12 ON)
endif()
#option(BUILD_WITH_DEBUG_LAYER "enable debug layer(require vulkan sdk installed)" ON) 

add_subdirectory(third_party)
add_subdirectory(antumbra)
add_subdirectory(tests)

find_package(Doxygen)

if(DOXYGEN_FOUND)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs)
    doxygen_add_docs(
        doxygen
        antumbra/framework
        COMMENT "Generate html pages"
    )
endif(DOXYGEN_FOUND)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)