# -----------------------------------------------------------------------------
# Numerus cmake to use it as CLI.
# Copyright © 2015-2017, Matjaž Guštin <dev@matjaz.it> https://matjaz.it
# All rights reserved.
# This file is part of the Numerus project which is released under
# the BSD 3-clause license.
# -----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.3)
project(Numerus)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra --std=c99")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${CMAKE_C_FLAGS}")

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message("Cmake Debug Pre-build")
    set(SOURCE_FILES
        test/main.c
        src/numerus_cli.c
        src/numerus_core.c
        src/numerus_utils.c
        test/numerus_test.c)
    add_executable(numerus_debug ${SOURCE_FILES})
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)
IF(CMAKE_BUILD_TYPE MATCHES Release)
    message("Cmake Release Pre-build")
    set(SOURCE_FILES
        src/main.c
        src/numerus_cli.c
        src/numerus_core.c
        src/numerus_utils.c)
    add_executable(numerus ${SOURCE_FILES})
ENDIF(CMAKE_BUILD_TYPE MATCHES Release)

