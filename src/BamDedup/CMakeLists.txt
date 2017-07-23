add_executable(bam_dedup
        BamDedupMain.cpp
        )


target_link_libraries(bam_dedup
        PRIVATE
        umilib
        )

install(TARGETS bam_dedup
        RUNTIME DESTINATION bin
        )
