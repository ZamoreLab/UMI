set(MyName bowtie)
set(MyGithubAddress https://github.com/BenLangmead/bowtie.git)
set(${MyName}_PREFIX ${CMAKE_BINARY_DIR}/third-party/${${MyName}}-prefix)
set(${MyName}_INSTALL ${IntelliaPipeline_ROOT_DIR})

set(${MyName}_Bins
        bowtie-build-s
        bowtie-inspect-s
        bowtie-build-l
        bowtie-inspect
        bowtie-inspect-l
        bowtie-build
        bowtie
        bowtie-align-s
        bowtie-align-l
        )

ExternalProject_Add(${MyName}
        PREFIX ${${MyName}_PREFIX}
        GIT_REPOSITORY ${MyGithubAddress}
        BUILD_IN_SOURCE ON
        CONFIGURE_COMMAND ""
        BUILD_COMMAND make
        INSTALL_COMMAND mv ${${MyName}_Bins} ${${MyName}_INSTALL}/bin
        LOG_DOWNLOAD ON
        )
    