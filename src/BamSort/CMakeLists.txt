add_executable(bam_sort
        BamSortMain.cpp
        )

add_dependencies(bam_sort umilib)

target_link_libraries(bam_sort
        PRIVATE
        umilib
        )

install(TARGETS bam_sort
        RUNTIME DESTINATION bin
        )
