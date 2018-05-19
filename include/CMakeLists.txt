# -----------------------------------------------------------------------------
# Numerus public API header CMake configuration file.
# Copyright © 2015-2018, Matjaž Guštin <dev@matjaz.it> https://matjaz.it
# All rights reserved.
# Released under the BSD 3-clause license.
# -----------------------------------------------------------------------------
set(PUBLIC_API_HEADER_FILE
    "numerus.h")
include_directories(".")
install(FILES PUBLIC_API_HEADER_FILE DESTINATION CMAKE_BINARY_DIR)

