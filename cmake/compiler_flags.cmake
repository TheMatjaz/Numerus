# -----------------------------------------------------------------------------
# Numerus CMake main configuration file.
# Copyright © 2015-2018, Matjaž Guštin <dev@matjaz.it> https://matjaz.it
# All rights reserved.
# Released under the BSD 3-clause license.
# -----------------------------------------------------------------------------
# This file contains settings for the ISO C standard, compilation warnings
# and errors, for each build type. Compilers FLAGs that are not supported by
# the used compiler are simply ignored.

message(STATUS
        "Configuring compiler flags, checking if compiler supports them")

include(CheckCCompilerFlag)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

list(APPEND
     COMPILER_FLAGS_WARNINGS
     "-Wall"
     "-Wextra"
     "-Wpedantic"
     "-Wconversion"
     "-Wswitch-default"
     "-Wswitch-enum"
     "-Wuninitialized"
     "-Wshadow"
     "-Wpacked"
     "-Wdouble-promotion"
     "-Werror")  # Turn all warnings into errors
list(APPEND
     COMPILER_FLAGS_DEBUG
     "-O0"
     "-g3"
     "-gdwarf-2")
list(APPEND
     COMPILER_FLAGS_RELEASE
     "-O3"
     "-DNDEBUG")

foreach (FLAG IN LISTS COMPILER_FLAGS_WARNINGS)
    check_c_compiler_flag(${FLAG} COMPILER_SUPPORTS_${FLAG})
    if (NOT COMPILER_SUPPORTS_${FLAG})
        string(APPEND UNSUPPORTED_COMPILER_FLAGS " ${FLAG}")
    endif ()
    string(APPEND CMAKE_C_FLAGS " ${FLAG}")
endforeach (FLAG)

foreach (FLAG IN LISTS COMPILER_FLAGS_DEBUG)
    check_c_compiler_flag(${FLAG} COMPILER_SUPPORTS_${FLAG})
    if (NOT COMPILER_SUPPORTS_${FLAG})
        string(APPEND UNSUPPORTED_COMPILER_FLAGS " ${FLAG}")
    endif ()
    string(APPEND CMAKE_C_FLAGS_DEBUG " ${FLAG}")
endforeach (FLAG)

foreach (FLAG IN LISTS COMPILER_FLAGS_RELEASE)
    check_c_compiler_flag(${FLAG} COMPILER_SUPPORTS_${FLAG})
    if (NOT COMPILER_SUPPORTS_${FLAG})
        string(APPEND UNSUPPORTED_COMPILER_FLAGS " ${FLAG}")
    endif ()
    string(APPEND CMAKE_C_FLAGS_RELEASE " ${FLAG}")
endforeach (FLAG)
