add_executable(clip_umi
        ClipUmiMain.cpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/Umi.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/SeqReader.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/UmiClipper.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/TwoBit.hpp
        ${${PROJECT_NAME}_INCLUDE_DIR}/Kmer.hpp
        )

add_dependencies(clip_umi umilib htslib)

target_link_libraries(clip_umi
        ${ZLIB_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        umilib
        )

install(TARGETS clip_umi
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        )
