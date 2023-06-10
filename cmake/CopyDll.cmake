macro(copy_dxil proj_name dst_dir)
    add_custom_command (TARGET ${proj_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${DXC_LIB_PATH}/bin/x64/dxil.dll ${dst_dir}
    )
    message("will copy dxil.dll from ${DXC_LIB_PATH}/bin/x64/dxil.dll to ${dst_dir} after build")
endmacro()