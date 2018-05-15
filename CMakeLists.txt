# -----------------------------------------------------------------------------
# Numerus cmake to use it as CLI.
# Copyright © 2015-2018, Matjaž Guštin <dev@matjaz.it> https://matjaz.it
# All rights reserved.
# This file is part of the Numerus project which is released under
# the BSD 3-clause license.
# -----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.3)
project(Numerus C)

set(CMAKE_C_STANDARD 11)
set(OUTPUT_FILE_NAME_NOEXTENSION numerus)
set(COMPILER_FLAGS_WARNINGS
        "-Wall \
        -Wextra \
        -pedantic \
        -Wconversion \
        -Wswitch-default \
        -Wswitch-enum \
        -Wuninitialized \
        -Wshadow \
        -Wpacked \
        -Wdouble-promotion
        -Werror \
        -pedantic-errors \
    ")
set(COMPILER_FLAGS_DEBUG "${COMPILER_FLAGS_WARNINGS} -O0 -g3 -gdwarf-2")
set(COMPILER_FLAGS_RELEASE "${COMPILER_FLAGS_WARNINGS} -O3 -DNDEBUG")
set(COMPILER_FLAGS_SMALL "${COMPILER_FLAGS_WARNINGS} -Os -DNDEBUG")

# Set a default build type for single-configuration
# CMake generators if no build type is set.
IF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

set(LIBRARY_SOURCE_FILES
    src/allocation.c
    src/analysis.c
    src/constants.c
    src/formatting.c
    src/numeral_to_value.c
    src/twelfths.c
    src/value_to_numeral.c)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


# Debug targets
set(DEBUG_SOURCE_FILES
    test/main.c
    test/numerus_test.c
    ${LIBRARY_SOURCE_FILES})
set(CMAKE_C_FLAGS ${COMPILER_FLAGS_DEBUG})
add_executable(${OUTPUT_FILE_NAME_NOEXTENSION}_debug ${DEBUG_SOURCE_FILES})


# Release targets
add_library(lib${OUTPUT_FILE_NAME_NOEXTENSION}.dll SHARED ${LIBRARY_SOURCE_FILES})

add_library(lib${OUTPUT_FILE_NAME_NOEXTENSION}.a STATIC ${LIBRARY_SOURCE_FILES})

set(CLI_SOURCE_FILES
    src/cli.c
    ${LIBRARY_SOURCE_FILES})
add_executable(${OUTPUT_FILE_NAME_NOEXTENSION} ${CLI_SOURCE_FILES})
