add_executable(bamsereader_test
        main.cpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/bam_reader.hpp
        ${${PROJECT_NAME}_SRC_DIR}/bam_reader/bam_reader.cpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/utils.hpp
        ${${PROJECT_NAME}_SRC_DIR}/utils/utils.cpp
        )
target_link_libraries(bamsereader_test
        gtest
        ${htslib_LIBRARIES}
        ${ZLIB_LIBRARIES}
        )

add_test(bamsereader_test bamsereader_test)