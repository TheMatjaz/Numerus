# -----------------------------------------------------------------------------
# Numerus CMake main configuration file.
# Copyright © 2015-2018, Matjaž Guštin <dev@matjaz.it> https://matjaz.it
# All rights reserved.
# Released under the BSD 3-clause license.
# -----------------------------------------------------------------------------
include_directories(".")

message(STATUS
        "Configuring libraries builds")
set(LIBRARY_SOURCE_FILES
    allocation.c
    analysis.c
    constants.c
    formatting.c
    numeral_to_value.c
    twelfths.c
    value_to_numeral.c)
string(TOLOWER
       "lib${CMAKE_PROJECT_NAME}.a"
       STATIC_LIBRARY_FILE_NAME)
string(TOLOWER
       "lib${CMAKE_PROJECT_NAME}.dll"
       SHARED_LIBRARY_FILE_NAME)
add_library(${SHARED_LIBRARY_FILE_NAME}
            SHARED
            ${LIBRARY_SOURCE_FILES})
set_property(TARGET
             ${SHARED_LIBRARY_FILE_NAME}
             PROPERTY
             OUTPUT_NAME sharedlibnumerus
             PREFIX prefissoshared
             DEBUG_OUTPUT_NAME DEBUG_debugsharedlibnumerus)
add_library(${STATIC_LIBRARY_FILE_NAME}
            STATIC
            ${LIBRARY_SOURCE_FILES})

message(STATUS "Configuring command line interface builds")
set(CLI_SOURCE_FILES
    cli.c
    ${LIBRARY_SOURCE_FILES})
string(TOLOWER
       "${CMAKE_PROJECT_NAME}_cli"
       CLI_EXECUTABLE_FILE_NAME)
add_executable(
        ${CLI_EXECUTABLE_FILE_NAME}
        ${CLI_SOURCE_FILES})
