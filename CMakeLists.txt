cmake_minimum_required(VERSION 3.21)

# Set the project name and language
project(antumbra)

include(cmake/Doxygen.cmake)

include(ProjectOptions.cmake)

add_subdirectory(antumbra)

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
