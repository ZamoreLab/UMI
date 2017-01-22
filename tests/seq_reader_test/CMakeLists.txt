add_executable(reader_test
        ${TEST_INCLUDE_DIR}/data.hpp
        main.cpp
        ${${PROJECT_NAME}_SRC_DIR}/SeqReader/SeqReader.cpp
        )
target_link_libraries(reader_test
        gtest gtest_main
        ${htslib_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        )

add_dependencies(reader_test htslib)
add_test(reader_test reader_test)
