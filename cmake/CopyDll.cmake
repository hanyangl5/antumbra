macro(copy_dxil proj_name dst_dir)
if(WIN32)
    add_custom_command (TARGET ${proj_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${DXC_LIB_PATH}/bin/x64/dxil.dll ${dst_dir}
    )
elseif(LINUX)
    add_custom_command (TARGET ${proj_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${DXC_LIB_PATH}/lib/libdxil.so ${dst_dir}
    )
    add_custom_command (TARGET ${proj_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${DXC_LIB_PATH}/lib/libdxcompiler.so ${dst_dir}
    )
elseif(APPLE)
    # add_custom_command (TARGET ${proj_name} POST_BUILD
    # COMMAND ${CMAKE_COMMAND} -E copy_if_different
    # ${DXC_LIB_PATH}/lib/libdxcompiler.dylib ${dst_dir})

endif()

endmacro()