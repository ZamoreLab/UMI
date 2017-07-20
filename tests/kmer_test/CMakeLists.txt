add_executable(kmer_test
        ../include/data.hpp
        main.cpp
        )

target_link_libraries(kmer_test
        gtest gtest_main
        umilib
        )
