add_executable(bam_dedup
        BamDedupMain.cpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/BamReader.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/BamAux.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/Dedup.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/Utils.hpp
        ${${PROJECT_NAME}_SRC_DIR}/BamDedup.cpp
        ${${PROJECT_NAME}_SRC_DIR}/BamAux.cpp
        ${${PROJECT_NAME}_SRC_DIR}/BamReader.cpp
        ${${PROJECT_NAME}_SRC_DIR}/Dedup.cpp
        ${${PROJECT_NAME}_SRC_DIR}/Utils.cpp
        )

target_link_libraries(bam_dedup
        ${htslib_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        )

install(TARGETS bam_dedup
        RUNTIME DESTINATION ${${PROJECT_NAME}_BIN_DIR}
        )