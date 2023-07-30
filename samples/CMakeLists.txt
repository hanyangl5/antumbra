

macro(add_subproject project_folder)
    project(${project_folder})
    file(GLOB_RECURSE HEADER_FILES "${project_folder}/*.h")
    file(GLOB_RECURSE SOURCE_FILES "${project_folder}/*.cpp")
    source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES})

    add_executable(${project_folder} ${SOURCE_FILES} ${HEADER_FILES})

    set_property(TARGET ${project_folder} PROPERTY FOLDER "samples")
    # include directories
    target_include_directories(${project_folder} PRIVATE
        framework)

    # link libraries
    target_link_libraries(${project_folder} PUBLIC
        framework
    )
    configure_compile_options_no_werror(${project_folder})
    copy_dxil(${project_folder} ${CMAKE_CURRENT_BINARY_DIR})
    

endmacro()

add_subproject(01hellowindow)
add_subproject(02hellotriangle)
add_subproject(03cube)
