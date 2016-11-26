add_executable(bam_dedup
        bam_dedup.cpp
        bam_dedup_main.cpp
        ${${PROJECT_NAME}_SRC_DIR}/bam_reader/bam_reader.cpp
        ${${PROJECT_NAME}_SRC_DIR}/utils/utils.cpp
        )

target_link_libraries(bam_dedup
        ${htslib_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        )

install(TARGETS bam_dedup
        RUNTIME DESTINATION ${${PROJECT_NAME}_BIN_DIR}
        )