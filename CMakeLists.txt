cmake_minimum_required(VERSION 3.10)
project(Numerus C)

set(CMAKE_C_STANDARD 11)


# -----------------------------------------------------------------------------
# Source files
# -----------------------------------------------------------------------------
set(LIBNUMERUS_HEADER inc/numerus.h)
set(LIBNUMERUS_SRC
    ${LIBNUMERUS_HEADER}
    src/numerus_constants.c
    src/numerus_to_roman.c
    )
set(TEST_NUMERUS_SRC
    tst/atto.c
    tst/main.c
    tst/numerus_test.h)
set(NUMERUS_DOXYGEN_SRC
    ${LIBNUMERUS_HEADER}
    LICENSE.md
    README.md
    CHANGELOG.md
    )
include_directories(AFTER SYSTEM inc/ tst/)


# -----------------------------------------------------------------------------
# Build targets
# ----------------------------------------------------------------------------
# Copy the library API header into the build directory.
# With this command, if the header changes, it's copied, otherwise not.
configure_file(${LIBNUMERUS_HEADER} ${CMAKE_BINARY_DIR} COPYONLY)

# Static library
add_library(numerus_static ${LIBNUMERUS_SRC})
target_include_directories(numerus_static PUBLIC inc/ src/)

# Shared library (.dylib / .dll)
# Does not reuse the static library object files, as they are
# recompiled in order to have position-independent code
add_library(numerus SHARED ${LIBNUMERUS_SRC})
set_property(TARGET numerus PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(numerus PUBLIC inc/ src/)

# Test runner executable
add_executable(test_numerus ${LIBNUMERUS_SRC} ${TEST_NUMERUS_SRC})
target_include_directories(test_numerus PUBLIC inc/ tst/)

# Doxygen documentation builder
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # Cmake's wrapper of Doxygen, constructing a doxyfile from the
    # DOXYGEN_* variables, which are mapped to the Doxygen variables.
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_MAN YES)
    set(DOXYGEN_JAVADOC_AUTOBRIEF YES)
    set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
    set(DOXYGEN_SORT_MEMBER_DOCS NO)
    set(DOXYGEN_IMAGE_PATH ${PROJECT_SOURCE_DIR}/images)
    set(DOXYGEN_ALIASES license="**License:**")
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)
    doxygen_add_docs(doxygen
                     ALL # Build doxygen on make-all
                     # List of input files for Doxygen
                     ${LIBNUMERUS_DOXYGEN_SRC}
    )
else (DOXYGEN_FOUND)
    message(WARNING "Doxygen not found. Cannot generate documentation.")
endif (DOXYGEN_FOUND)
