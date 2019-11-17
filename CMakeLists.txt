# -----------------------------------------------------------------------------
# Numerus CMake main configuration file.
# Copyright © 2015-2019, Matjaž Guštin <dev@matjaz.it> https://matjaz.it
# All rights reserved.
# Released under the BSD 3-clause license.
# -----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(Numerus C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
#include(doxygen)
include(build_type)
include(compiler_flags)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(test)
