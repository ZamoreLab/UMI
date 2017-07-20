add_executable(bam_sort
        BamSortMain.cpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/BamReader.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/BamAux.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/Utils.hpp
        )

add_dependencies(bam_sort umilib)

target_link_libraries(bam_sort
        ${htslib_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        )

install(TARGETS bam_sort
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        )
