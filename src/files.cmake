set(umilib_PUBLIC_HEADER_FILES
        ${${PROJECT_NAME}_INCLUDE_DIR}/SeqReader.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/Umi.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/BamReader.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/BamDedup.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/Utils.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/KernelColor.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/UmiClipper.hpp
        )

set(umilib_HEADER_FILES
        ${umilib_PUBLIC_HEADER_FILES}
        )

set(umilib_SOURCE_FILES
        ${${PROJECT_NAME}_SRC_DIR}/BamAux.cpp
        ${${PROJECT_NAME}_SRC_DIR}/BamDedup.cpp
        ${${PROJECT_NAME}_SRC_DIR}/BamReader.cpp
        ${${PROJECT_NAME}_SRC_DIR}/Dedup.cpp
        ${${PROJECT_NAME}_SRC_DIR}/SeqReader.cpp
        ${${PROJECT_NAME}_SRC_DIR}/TwoBit.cpp
        ${${PROJECT_NAME}_SRC_DIR}/Umi.cpp
        ${${PROJECT_NAME}_SRC_DIR}/UmiClipper.cpp
        ${${PROJECT_NAME}_SRC_DIR}/Utils.cpp
        )