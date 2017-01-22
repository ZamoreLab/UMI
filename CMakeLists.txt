cmake_minimum_required(VERSION 2.8)
if (POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif ()
if (POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW)
endif ()

project(ZamoreUmiSeq CXX C)

# compiler flags
include(CheckCXXCompilerFlag)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y") # use C++14 for now
check_cxx_compiler_flag("-Wno-unused-local-typedefs" HAS_NO_UNUSED_LOCAL_TYPEDEFS)
if (HAS_NO_UNUSED_LOCAL_TYPEDEFS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-local-typedefs")
endif ()

# determine the version
foreach (level MAJOR MINOR PATCH)
    file(STRINGS bin/const.sh
            _define_${level}
            REGEX "PROG_${level}_VERSION=")
    string(REGEX MATCH "([0-9]+)" _${level} "${_define_${level}}")
endforeach ()
set(PROJECT_VERSION "${_MAJOR}.${_MINOR}.${_PATCH}")

# variables
set(${PROJECT_NAME}_ROOT_DIR ${PROJECT_SOURCE_DIR})
set(${PROJECT_NAME}_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(${PROJECT_NAME}_SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(${PROJECT_NAME}_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(${PROJECT_NAME}_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(${PROJECT_NAME}_TEST_DIR ${PROJECT_SOURCE_DIR}/tests)

# load colors for cmake
include(cmake/color.cmake)
message(STATUS "${Green}CMaking ${PROJECT_NAME} v${PROJECT_VERSION}${ColourReset}")

include(ExternalProject)
include_directories(${${PROJECT_NAME}_INCLUDE_DIR})

# thread
find_package(Threads)

# boost
find_package(Boost REQUIRED)
include_directories(${BOOST_INCLUDE_DIRS})

# load ZLIB for htslib, build one if not exist
find_package(ZLIB)
if (NOT ZLIB_FOUND)
    message(STATUS "${Red}Failed to find Zlib, compile it ourselves${ColourReset}")
    include(cmake/zlib.cmake)
endif ()

# install htslib, a must have
message(STATUS "${Green}CMaking htslib${ColourReset}")
include(cmake/htslib.cmake)

# install own binaries
message(STATUS "${Green}CMaking ${PROJECT_NAME} binaries${ColourReset}")
include_directories(${${PROJECT_NAME}_INCLUDE_DIR})
add_subdirectory(${${PROJECT_NAME}_SRC_DIR})

# tests
option(BUILD_TESTS "Build tests" ON)
if (BUILD_TESTS)
    message(STATUS "${Green}CMaking Tests${ColourReset}")
    enable_testing()
    add_subdirectory(${${PROJECT_NAME}_TEST_DIR})
endif ()

# install third-party binaries

# Bowtie2
find_program(HAS_BOWTIE2 "bowtie2" HINTS ${${PROJECT_NAME}_BIN_DIR})
if (NOT HAS_BOWTIE2 AND NOT CMAKE_BUILD_TYPE MATCHES "DEBUG")
    message(STATUS "${Yellow}CMaking optional target Bowtie2${ColourReset}")
    include(cmake/bowtie2.cmake)
endif ()

# samtools
find_program(HAS_SAMTOOLS "samtools" HINTS ${${PROJECT_NAME}_BIN_DIR})
if (NOT HAS_SAMTOOLS AND NOT CMAKE_BUILD_TYPE MATCHES "DEBUG")
    message(STATUS "${Yellow}CMaking optional target Samtools${ColourReset}")
    include(cmake/samtools.cmake)
endif ()

# macs2
find_program(HAS_MACS2 "macs2" HINTS ${${PROJECT_NAME}_BIN_DIR})
if (NOT HAS_MACS2 AND NOT CMAKE_BUILD_TYPE MATCHES "DEBUG")
    message(STATUS "${Yellow}CMaking optional target MACS2${ColourReset}")
    include(cmake/macs2.cmake)
endif ()

# umitools
find_program(HAS_UMITOOLS "umi_tools" HINTS ${${PROJECT_NAME}_BIN_DIR})
if (NOT HAS_UMITOOLS AND NOT CMAKE_BUILD_TYPE MATCHES "DEBUG")
    message(STATUS "${Yellow}CMaking optional target umi-tools${ColourReset}")
    include(cmake/umitools.cmake)
endif ()

