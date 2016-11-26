add_custom_target(MACS2 ALL)
add_custom_command(TARGET MACS2 PRE_BUILD
        COMMAND pip
        ARGS install numpy
        COMMAND pip
        ARGS install macs2
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
