include(files.cmake)

add_library(umilib
        SHARED
        ${umilib_HEADER_FILES}
        ${umilib_SOURCE_FILES}
        )

set_target_properties(umilib PROPERTIES
        PUBLIC_HEADER "${umilib_PUBLIC_HEADER_FILES}"
        )

target_link_libraries(umilib
        PUBLIC
        ${htslib_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        )

install(TARGETS umilib
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        )

add_subdirectory(BamSort)
add_subdirectory(ClipUmi)
add_subdirectory(BamDedup)