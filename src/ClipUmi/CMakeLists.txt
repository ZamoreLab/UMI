add_executable(clip_umi
        ClipUmiMain.cpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/Umi.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/SeqReader.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/UmiClipper.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/TwoBit.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/Kmer.hpp
        ${${PROJECT_NAME}_SRC_DIR}/Umi/Umi.cpp
        ${${PROJECT_NAME}_SRC_DIR}/SeqReader/SeqReader.cpp
        ${${PROJECT_NAME}_SRC_DIR}/Umi/UmiClipper.cpp
        )

target_link_libraries(clip_umi
        ${htslib_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        )

install(TARGETS clip_umi
        RUNTIME DESTINATION ${${PROJECT_NAME}_BIN_DIR}
        )