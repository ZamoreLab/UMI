add_executable(bamdedup_test
        main.cpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/BamReader.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/Dedup.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/BamDedup.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/BamAux.hpp

        ${${PROJECT_NAME}_SRC_DIR}/Dedup/Dedup.cpp
        ${${PROJECT_NAME}_SRC_DIR}/BamAux/BamAux.cpp
        ${${PROJECT_NAME}_SRC_DIR}/BamReader/BamReader.cpp
        ${${PROJECT_NAME}_SRC_DIR}/BamDedup/BamDedup.cpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/Utils.hpp
        ${${PROJECT_NAME}_SRC_DIR}/Utils/Utils.cpp
        )
target_link_libraries(bamdedup_test
        gtest
        ${htslib_LIBRARIES}
        ${ZLIB_LIBRARIES}
        )

add_test(bamdedup_test bamdedup_test)