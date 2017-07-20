add_executable(bamsereader_test
        main.cpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/BamReader.hpp
        ${${PROJECT_NAME}_SRC_DIR}/BamReader.cpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/Utils.hpp
        ${${PROJECT_NAME}_SRC_DIR}/Utils.cpp
        )

target_link_libraries(bamsereader_test
        gtest
        ${htslib_LIBRARIES}
        ${ZLIB_LIBRARIES}
        umilib
        )

add_test(bamsereader_test bamsereader_test)