# data
configure_file(
        ${${PROJECT_NAME}_TEST_DIR}/include/data.hpp.in
        ${${PROJECT_NAME}_TEST_DIR}/include/data.hpp
)
set(TEST_INCLUDE_DIR ${${PROJECT_NAME}_TEST_DIR}/include)
include_directories(${TEST_INCLUDE_DIR})
# gtest
add_subdirectory(googletest EXCLUDE_FROM_ALL)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# add individual tests
add_subdirectory(htslib_test)
add_subdirectory(seq_reader_test)
add_subdirectory(umiclip_test)
add_subdirectory(bam_se_reader_test)
add_subdirectory(bam_pe_reader_test)
add_subdirectory(bam_dedup_test)
add_subdirectory(kmer_test)