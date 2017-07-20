add_executable(umiclip_test
        ${TEST_INCLUDE_DIR}/data.hpp
        main.cpp
        )

add_dependencies(umiclip_test umilib)

target_link_libraries(umiclip_test
        gtest gtest_main
        ${htslib_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        umilib
        )

add_test(umiclip_test umiclip_test)
