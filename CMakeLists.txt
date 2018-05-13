# -----------------------------------------------------------------------------
# Numerus cmake to use it as CLI.
# Copyright © 2015-2017, Matjaž Guštin <dev@matjaz.it> https://matjaz.it
# All rights reserved.
# This file is part of the Numerus project which is released under
# the BSD 3-clause license.
# -----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.3)
project(Numerus)

set(CMAKE_C_FLAGS "-Wall -pedantic -Wextra --std=c99")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g3 -gdwarf-2")
set(CMAKE_C_FLAGS_CLI "${CMAKE_C_FLAGS} -O3 -DNDEBUG")
set(CMAKE_C_FLAGS_SHAREDLIB "${CMAKE_C_FLAGS} -O3 -DNDEBUG")
set(CMAKE_C_FLAGS_STATICLIB "${CMAKE_C_FLAGS} -O3 -DNDEBUG")

if(NOT CMAKE_BUILD_TYPE)
   message(STATUS "Setting build type to 'CLI' as none was specified.")
   set(CMAKE_BUILD_TYPE CLI CACHE STRING "Choose the type of build." FORCE)
   # Set the possible values of build type for cmake-gui
   set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "CLI"
     "SharedLib" "StaticLib")
endif()
message(STATUS "Chosen build type: ${CMAKE_BUILD_TYPE}")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(SOURCE_FILES
        test/main.c
        src/numerus_core.c
        src/numerus_utils.c
        test/numerus_test.c)
    set(EXECUTABLE_NAME numerus_debug)
    message(STATUS "Executable fill will be ${EXECUTABLE_NAME}")
    add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

elseif(CMAKE_BUILD_TYPE MATCHES SharedLib)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
    set(SOURCE_FILES
        src/numerus_core.c
        src/numerus_utils.c)
    add_library(numerus SHARED ${SOURCE_FILES})

elseif(CMAKE_BUILD_TYPE MATCHES StaticLib)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
    set(SOURCE_FILES
        src/numerus_core.c
        src/numerus_utils.c)
    add_library(numerus STATIC ${SOURCE_FILES})

else(CMAKE_BUILD_TYPE MATCHES Debug)
    # CLI is the default build type
    set(SOURCE_FILES
        src/cli/main.c
        src/cli/numerus_cli.c
        src/numerus_core.c
        src/numerus_utils.c)
    set(EXECUTABLE_NAME numerus)
    message(STATUS "Executable fill will be ${EXECUTABLE_NAME}")
    add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})
endif(CMAKE_BUILD_TYPE MATCHES Debug)

