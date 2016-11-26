set(MyName samtools)
set(MyGithubAddress https://github.com/samtools/samtools.git)
set(${MyName}_PREFIX ${CMAKE_BINARY_DIR}/third-party/${${MyName}}-prefix)
set(${MyName}_INSTALL ${${PROJECT_NAME}_ROOT_DIR})

ExternalProject_Add(${MyName}
        PREFIX ${${MyName}_PREFIX}
        GIT_REPOSITORY ${MyGithubAddress}
        BUILD_IN_SOURCE ON
        CONFIGURE_COMMAND ""
        BUILD_COMMAND make
        INSTALL_COMMAND make install prefix=${${MyName}_INSTALL}
        LOG_DOWNLOAD ON
        )

add_dependencies(samtools htslib)