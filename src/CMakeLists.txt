include(files.cmake)


add_library(umilib SHARED ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(umilib
        ${htslib_LIBRARIES}
        ${ZLIB_LIBRARIES}
        )

install(TARGETS umilib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        )

add_subdirectory(BamSort)
add_subdirectory(ClipUmi)
add_subdirectory(BamDedup)