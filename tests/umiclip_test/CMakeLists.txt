add_executable(umiclip_test
        ${TEST_INCLUDE_DIR}/data.hpp
        main.cpp
        ${${PROJECT_NAME}_SRC_DIR}/SeqReader/SeqReader.cpp
        ${${PROJECT_NAME}_SRC_DIR}/Umi/Umi.cpp
        ${${PROJECT_NAME}_SRC_DIR}/Umi/UmiClipper.cpp
        )
target_link_libraries(umiclip_test
        gtest gtest_main
        ${htslib_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        )

add_dependencies(umiclip_test htslib)
add_test(umiclip_test umiclip_test)
