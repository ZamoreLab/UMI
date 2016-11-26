add_executable(phony
        ${HEADER_FILES}
        main.cpp
        )

target_link_libraries(phony
        ${htslib_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        )