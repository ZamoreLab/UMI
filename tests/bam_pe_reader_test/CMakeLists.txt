add_executable(bampereader_test
        main.cpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/BamReader.hpp
        ${${PROJECT_NAME}_SRC_DIR}/BamReader/BamReader.cpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/Utils.hpp
        ${${PROJECT_NAME}_SRC_DIR}/Utils/Utils.cpp
        )
target_link_libraries(bampereader_test
        gtest
        ${htslib_LIBRARIES}
        ${ZLIB_LIBRARIES}
        )

add_test(bampereader_test bampereader_test)