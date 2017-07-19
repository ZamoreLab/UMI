add_executable(kmer_test
        ../include/data.hpp
        ${${PROJECT_NAME}_SRC_DIR}/TwoBit.cpp
        main.cpp
        )
target_link_libraries(kmer_test
        gtest gtest_main
        )
