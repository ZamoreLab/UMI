add_executable(clip_umi
        ClipUmiMain.cpp
        ${${PROJECT_NAME}_SRC_DIR}/SeqReader/SeqReader.cpp
        ${${PROJECT_NAME}_SRC_DIR}/Umi/Umi.cpp
        )

target_link_libraries(clip_umi
        ${htslib_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        )

install(TARGETS clip_umi
        RUNTIME DESTINATION ${${PROJECT_NAME}_BIN_DIR}
        )