project(framework)

file(GLOB_RECURSE HEADER_FILES "*.h")
file(GLOB_RECURSE SOURCE_FILES "*.cpp")

if(NOT ANTUMBRA_BUILD_D3D12)
    file(GLOB_RECURSE D3D12_SOURCES "*gal/d3d12/*.cpp")
    list(REMOVE_ITEM SOURCE_FILES ${D3D12_SOURCES})
endif()

# source_group(TREE "${ENGINE_ROOT_DIR}/schema" FILES ${SCHEMA_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES})

add_library(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/antumbra
    ${CMAKE_SOURCE_DIR}/antumbra/config/)

target_link_libraries(${PROJECT_NAME} PUBLIC ant_third_party)

configure_compile_options("${PROJECT_NAME}")

# # Generate header.hpp from your template file header.hpp.in
# configure_file(${CMAKE_CURRENT_LIST_DIR}/../config/config.hpp.in
# ${CMAKE_CURRENT_LIST_DIR}/../config/config.hpp COPYONLY
# )

# set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "antumbra/Runtime")