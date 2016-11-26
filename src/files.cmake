SET(HEADER_FILES
        ${${PROJECT_NAME}_INCLUDE_DIR}/const.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/seq_reader.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/umi.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/bam_reader.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/bam_dedup.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/utils.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/kernel_color.hpp

        )


set(SOURCE_FILES
        ${${PROJECT_NAME}_SRC_DIR}/seq_reader/seq_reader.cpp
        ${${PROJECT_NAME}_SRC_DIR}/umi/umi.cpp
        ${${PROJECT_NAME}_SRC_DIR}/clip_umi/clip_umi.cpp
        ${${PROJECT_NAME}_SRC_DIR}/bam_reader/bam_reader.cpp
        ${${PROJECT_NAME}_SRC_DIR}/bam_dedup/bam_dedup.cpp
        ${${PROJECT_NAME}_SRC_DIR}/utils/utils.cpp
        )