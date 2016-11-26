set(MyName zlib)
set(MyGithubAddress https://github.com/madler/zlib.git)
set(${MyName}_PREFIX ${CMAKE_BINARY_DIR}/third-party/${${MyName}}-prefix)
set(${MyName}_INSTALL ${${PROJECT_NAME}_ROOT_DIR})

ExternalProject_Add(${MyName}
        PREFIX ${${MyName}_PREFIX}
        GIT_REPOSITORY ${MyGithubAddress}
        BUILD_IN_SOURCE ON
        CONFIGURE_COMMAND ./configure --prefix=${${${MyName}_INSTALL}}
        BUILD_COMMAND make
        INSTALL_COMMAND make install prefix=${${MyName}_INSTALL}
        LOG_DOWNLOAD ON
        )

if (APPLE)
    set(ZLIB_LIBRARIES ${${PROJECT_NAME}_LIB_DIR}/libz.dylib)
else ()
    set(ZLIB_LIBRARIES ${${PROJECT_NAME}_LIB_DIR}/libz.a)
endif ()