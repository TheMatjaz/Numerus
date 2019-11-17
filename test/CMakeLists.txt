# -----------------------------------------------------------------------------
# Numerus test executable CMake configuration file.
# Copyright © 2015-2019, Matjaž Guštin <dev@matjaz.it> https://matjaz.it
# All rights reserved.
# Released under the BSD 3-clause license.
# -----------------------------------------------------------------------------

include_directories("." "../include" "../src")

message(STATUS "Configuring test builds")
set(TEST_SOURCE_FILES
    atto.h
    test.h
    test_main.c
    test_constants.c
    test_int_to_basic.c)
string(TOLOWER
       "test_${CMAKE_PROJECT_NAME}"
       TEST_EXECUTABLE_FILE_NAME)
add_executable(${TEST_EXECUTABLE_FILE_NAME}
               ${TEST_SOURCE_FILES}
               ${PUBLIC_API_HEADER_FILE}
               ${LIBRARY_SOURCE_FILES})
