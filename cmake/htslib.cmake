set(MyName htslib)
set(MyGithubAddress https://github.com/Intellia/htslib.git)
set(${MyName}_PREFIX ${CMAKE_BINARY_DIR}/third-party/${${MyName}}-prefix)
set(${MyName}_INSTALL ${${PROJECT_NAME}_ROOT_DIR})

ExternalProject_Add(${MyName}
        PREFIX ${${MyName}_PREFIX}
        GIT_REPOSITORY ${MyGithubAddress}
        BUILD_IN_SOURCE ON
        CONFIGURE_COMMAND ""
        BUILD_COMMAND make
        INSTALL_COMMAND make install prefix=${CMAKE_INSTALL_PREFIX}
        LOG_DOWNLOAD ON
        )

if (NOT ZLIB_FOUND)
    add_dependencies(${MyName} zlib)
endif ()

if (APPLE)
    set(htslib_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libhts.dylib)
else ()
    set(htslib_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libhts.so)
endif ()

set(htslib_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)