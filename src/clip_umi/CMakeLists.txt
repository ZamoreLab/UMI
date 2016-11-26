add_executable(clip_umi
        clip_umi.cpp
        ${${PROJECT_NAME}_SRC_DIR}/seq_reader/seq_reader.cpp
        ${${PROJECT_NAME}_SRC_DIR}/umi/umi.cpp
        )

target_link_libraries(clip_umi
        ${htslib_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        )

install(TARGETS clip_umi
        RUNTIME DESTINATION ${${PROJECT_NAME}_BIN_DIR}
        )