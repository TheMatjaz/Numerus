# -----------------------------------------------------------------------------
# Numerus test executable CMake configuration file.
# Copyright © 2015-2019, Matjaž Guštin <dev@matjaz.it> https://matjaz.it
# All rights reserved.
# Released under the BSD 3-clause license.
# -----------------------------------------------------------------------------

include_directories(".")

message(STATUS "Configuring test builds")
find_library(
        LIBCHECK_LIBRARY
        NAMES check check_dll libcheck libcheck_dll
        HINTS ./lib/)
if (NOT LIBCHECK_LIBRARY)
    message(WARNING
            "libcheck library not found. Cannot compile test executable.")
else ()
    set(TEST_SOURCE_FILES
        test_dictionary.c
        ${LIBRARY_SOURCE_FILES})
    string(TOLOWER
           "test_${CMAKE_PROJECT_NAME}"
           TEST_EXECUTABLE_FILE_NAME)
    add_executable(${TEST_EXECUTABLE_FILE_NAME}
                   ${TEST_SOURCE_FILES})
    target_link_libraries(${TEST_EXECUTABLE_FILE_NAME}
                          ${LIBCHECK_LIBRARY})
endif ()
