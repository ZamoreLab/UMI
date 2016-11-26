add_custom_target(UMITOOLS ALL)
add_custom_command(TARGET UMITOOLS PRE_BUILD
        COMMAND pip
        ARGS install cython pysam
        COMMAND pip
        ARGS install umi_tools
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
