add_executable(bamdedup_test
        main.cpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/BamReader.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/Dedup.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/BamDedup.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/BamAux.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/Utils.hpp
        )

add_dependencies(bamdedup_test umilib)

target_link_libraries(bamdedup_test
        gtest
        ${htslib_LIBRARIES}
        ${ZLIB_LIBRARIES}
        umilib
        )

add_test(bamdedup_test bamdedup_test)