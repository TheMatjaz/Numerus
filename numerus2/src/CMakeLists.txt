# -----------------------------------------------------------------------------
# Copyright © 2015-2019, Matjaž Guštin <dev@matjaz.it> https://matjaz.it
# All rights reserved.
# Released under the BSD 3-clause license.
# -----------------------------------------------------------------------------

include_directories("." "../include")
add_library(library OBJECT
            #allocation
            #analysis
            constants
            #formatting
            #numeral_to_value
            #twelfths
            value_to_numeral
            )
add_library(cli OBJECT cli)
