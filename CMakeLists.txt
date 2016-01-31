# -----------------------------------------------------------------------------
# Numerus cmake to use it as CLI.
# Copyright © 2015-2016, Matjaž Guštin <dev@matjaz.it> http://matjaz.it
# All rights reserved.
# This file is part of the Numerus project which is released under
# the BSD 3-clause license.
# -----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.0.0)
project(Numerus)
set(CMAKE_C_FLAGS "-Wall --std=c99")
add_executable(numerus src/numerus_core.c
                       src/numerus_utils.c
                       src/numerus_cli.c
                       src/main.c)

