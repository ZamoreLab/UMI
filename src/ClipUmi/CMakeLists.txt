add_executable(clip_umi
        ClipUmiMain.cpp
        )

target_link_libraries(clip_umi
        PRIVATE
        umilib
        )

install(TARGETS clip_umi
        RUNTIME DESTINATION bin
        )
