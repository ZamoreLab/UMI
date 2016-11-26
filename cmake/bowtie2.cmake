set(MyName bowtie2)
set(MyGithubAddress https://github.com/BenLangmead/bowtie2.git)
set(${MyName}_PREFIX  ${CMAKE_BINARY_DIR}/third-party/${MyName}-prefix)
set(${MyName}_INSTALL ${${PROJECT_NAME}_BIN_DIR})

set(${MyName}_Bins 
    bowtie2-build-l
    bowtie2-inspect-l
    bowtie2-build-s
    bowtie2-align-l
    bowtie2-build
    bowtie2-inspect
    bowtie2-inspect-s
    bowtie2
    bowtie2-align-s
)

MESSAGE(STATUS "${${MyName}_Bins}")
MESSAGE(STATUS "${${PROJECT_NAME}_BIN_DIR}")

ExternalProject_Add(${MyName}
    PREFIX ${${MyName}_PREFIX}
    GIT_REPOSITORY ${MyGithubAddress}
    BUILD_IN_SOURCE 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make
    INSTALL_COMMAND mv ${${MyName}_Bins} ${${PROJECT_NAME}_BIN_DIR}
    LOG_DOWNLOAD 1
)
    