project(framework)


file(GLOB_RECURSE HEADER_FILES "*.h")
file(GLOB_RECURSE SOURCE_FILES "*.cpp")

# source_group(TREE "${ENGINE_ROOT_DIR}/schema" FILES ${SCHEMA_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES})

add_library(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/antumbra)

target_link_libraries(${PROJECT_NAME} PUBLIC ant_third_party)

configure_compile_options("${PROJECT_NAME}")

#set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "antumbra/Runtime")