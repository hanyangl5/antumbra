project(tests)

file(GLOB_RECURSE HEADER_FILES "*.h")
file(GLOB_RECURSE SOURCE_FILES "*.cpp")

# source_group(TREE "${ENGINE_ROOT_DIR}/schema" FILES ${SCHEMA_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

copy_dxil(${PROJECT_NAME} ${CMAKE_CURRENT_BINARY_DIR})

configure_compile_options_no_werror(${PROJECT_NAME})

# include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    framework)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    ${CMAKE_SOURCE_DIR}/third_party/doctest/doctest)

# link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC
    doctest_with_main
    framework
)