# -----------------------------------------------------------------------------
# Numerus test executable CMake configuration file.
# Copyright © 2015-2019, Matjaž Guštin <dev@matjaz.it> https://matjaz.it
# All rights reserved.
# Released under the BSD 3-clause license.
# -----------------------------------------------------------------------------

include_directories("." "../include")

add_library(test OBJECT
            atto
            test
            test_main
            test_constants
            test_int_to_basic
            )
