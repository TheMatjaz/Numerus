# -----------------------------------------------------------------------------
# Doxygen generation target CMake configuration file.
# Copyright © 2015-2019, Matjaž Guštin <dev@matjaz.it> https://matjaz.it
# All rights reserved.
# Released under the BSD 3-clause license.
# -----------------------------------------------------------------------------

find_package(Doxygen)
if (NOT DOXYGEN_FOUND)
    message(WARNING "Doxygen not found. Cannot generate documentation.")
    return()
endif ()

set(DOXYGEN_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/docs")
list(APPEND
     DOXYGEN_CONFIG_FILES
     "${CMAKE_SOURCE_DIR}/doxyfile_internal.doxygen"
     "${CMAKE_SOURCE_DIR}/doxyfile_public_api.doxygen")

foreach (DOXYFILE IN LISTS DOXYGEN_CONFIG_FILES)
    if (NOT EXISTS ${DOXYFILE})
        message(ERROR "Could not find Doxygen config file at ${DOXYFILE}.")
    else ()
        set_property(
                DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                APPEND
                PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${DOXYGEN_OUTPUT_DIR}")
        get_filename_component(DOXY_TARGET_NAME ${DOXYFILE} NAME_WE)
        string(REPLACE "doxyfile" "doxygen" DOXY_TARGET_NAME ${DOXY_TARGET_NAME})
        add_custom_target(
                ${DOXY_TARGET_NAME} ${DOXYGEN_EXECUTABLE} ${DOXYFILE})
        message(STATUS "Added Doxygen target: ${DOXY_TARGET_NAME}")
    endif ()
endforeach ()
