add_executable(bam_dedup
        bam_dedup.cpp
        )

target_link_libraries(bam_dedup
        ${htslib_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        )

install(TARGETS bam_dedup
        RUNTIME DESTINATION ${${PROJECT_NAME}_BIN_DIR}
        )