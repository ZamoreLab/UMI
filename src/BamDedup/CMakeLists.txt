add_executable(bam_dedup
        BamDedupMain.cpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/BamReader.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/BamAux.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/Dedup.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/Utils.hpp
        )

add_dependencies(bam_dedup umilib)

target_link_libraries(bam_dedup
        ${htslib_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        umilib
        )

install(TARGETS bam_dedup
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        )
