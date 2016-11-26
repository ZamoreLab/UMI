add_executable(htslib_test
        ../include/data.hpp
        main.cpp
        )
target_link_libraries(htslib_test
        gtest gtest_main
        ${htslib_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        )

add_dependencies(htslib_test htslib)
add_test(htslib_test htslib_test)
