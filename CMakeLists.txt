# Compilation configuration of the Numerus project
# Copyright © 2015-2021, Matjaž Guštin <dev@matjaz.it> <https://matjaz.it>.
# All rights reserved.
# Release under the BSD 3-clause license.

cmake_minimum_required(VERSION 3.6)
project(Numerus
        VERSION 3.0.0
        LANGUAGES C
        DESCRIPTION
        "Roman numeral conversion library")

# Unless specified, by default create Release builds
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# -----------------------------------------------------------------------------
# Compiler flags
# -----------------------------------------------------------------------------
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Activate a million warnings to have the cleanest possible code
set(FLAGS_WARNINGS -Wall -Wextra -pedantic -Wconversion -Wdouble-promotion
    -Wswitch-default -Wswitch-enum -Wuninitialized -Wno-unused-variable
    -Wpacked -Wpadded -Wshadow -Wformat-security -Wlogical-not-parentheses
    -Waggregate-return -Wmissing-declarations -Wsign-conversion
    -Wjump-misses-init)
# Debug build: compile with no optimisation, debug info and printing
set(CMAKE_C_FLAGS_DEBUG "${WARNING_FLAGS} -g -O0 -DDEBUG \
    -fprofile-instr-generate -fcoverage-mapping")
# Append sanitiser flags on non-Windows systems
if (NOT WIN32 AND NOT CYGWIN AND NOT MSYS)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} \
            -fsanitize=address,undefined -static-libsan \
            -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer")
endif ()

# Mini-sized release build: compile with optimisation for size
# convert warnings into errors and some other optimisations
set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} -DMINSIZEREL \
        ${WARNING_FLAGS} \
        -Os -Werror -fomit-frame-pointer -march=native -mtune=native -flto")

# Performance-oriented release build: compile with optimisation for speed
# convert warnings into errors and some other optimisations
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DRELEASE \
        ${WARNING_FLAGS} \
        -O3 -Werror -fomit-frame-pointer -march=native -mtune=native \
        -funroll-loops -flto")


# -----------------------------------------------------------------------------
# Source files
# -----------------------------------------------------------------------------
set(LIBNUMERUS_HEADER inc/numerus.h)
set(LIBNUMERUS_SRC_NOALLOC
    ${LIBNUMERUS_HEADER}
    src/numerus_to_roman.c
    src/numerus_strutils.c
    src/numerus_fraction.c
    src/numerus_from_roman.c
    src/numerus_fmt.c
    )
set(LIBNUMERUS_SRC_ALLOC
    ${LIBNUMERUS_SRC_NOALLOC}
    src/numerus_to_roman_alloc.c
    src/numerus_fmt_alloc.c)
set(TEST_NUMERUS_SRC
    tst/atto.c
    tst/main.c
    tst/test_constants.c
    tst/test_strutils.c
    tst/test_fraction.c
    tst/test_fmt.c
    tst/test_to_roman_basic.c
    tst/test_to_roman_extended.c
    tst/test_from_roman_basic.c
    tst/test_from_roman_extended.c
    )
set(NUMERUS_DOXYGEN_SRC
    ${LIBNUMERUS_HEADER}
    LICENSE.md
    README.md
    CHANGELOG.md
    )


# -----------------------------------------------------------------------------
# Build targets
# ----------------------------------------------------------------------------
# Copy the library API header into the build directory.
# With this command, if the header changes, it's copied, otherwise not.
configure_file(${LIBNUMERUS_HEADER} ${CMAKE_BINARY_DIR} COPYONLY)

# Static library
add_library(numerus_static_noalloc ${LIBNUMERUS_SRC_NOALLOC})
target_include_directories(numerus_static_noalloc PUBLIC inc/)
target_compile_definitions(numerus_static_noalloc PUBLIC NUMERUS_HAS_MALLOC=0)
add_library(numerus_static ${LIBNUMERUS_SRC_ALLOC})
target_include_directories(numerus_static PUBLIC inc/)
target_compile_definitions(numerus_static PUBLIC NUMERUS_HAS_MALLOC=1)

# Shared library (.dylib / .dll)
# Does not reuse the static library object files, as they are
# recompiled in order to have position-independent code
add_library(numerus_noalloc SHARED ${LIBNUMERUS_SRC_NOALLOC})
set_property(TARGET numerus_noalloc PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(numerus_noalloc PUBLIC inc/)
target_compile_definitions(numerus_noalloc PUBLIC NUMERUS_HAS_MALLOC=0)
add_library(numerus SHARED ${LIBNUMERUS_SRC_ALLOC})
set_property(TARGET numerus PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(numerus PUBLIC inc/)
target_compile_definitions(numerus PUBLIC NUMERUS_HAS_MALLOC=1)

# Test runner executable
add_executable(test_numerus ${LIBNUMERUS_SRC_ALLOC} ${TEST_NUMERUS_SRC})
target_include_directories(test_numerus PUBLIC inc/ tst/)
target_compile_definitions(test_numerus PUBLIC NUMERUS_HAS_MALLOC=1)

# Doxygen documentation builder
find_package(Doxygen OPTIONAL_COMPONENTS dot)
if (DOXYGEN_FOUND)
    # Cmake's wrapper of Doxygen, constructing a doxyfile from the
    # DOXYGEN_* variables, which are mapped to the Doxygen variables.

    # Output format
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_MAN YES)
    set(DOXYGEN_SORT_MEMBER_DOCS NO)
    set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
    set(DOXYGEN_OUTPUT_LANGUAGE English)
    set(DOXYGEN_OUTPUT_DIRECTORY docs/)

    # Input format
    set(DOXYGEN_JAVADOC_AUTOBRIEF YES)
    set(DOXYGEN_ALIASES license="**License:**")
    set(DOXYGEN_EXTRACT_ALL NO)
    set(DOXYGEN_INPUT_ENCODING UTF-8)

    # Input files
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)
    set(DOXYGEN_EXCLUDE src/ tst/ numerus2/ builds/)
    set(DOXYGEN_IMAGE_PATH ${PROJECT_SOURCE_DIR}/images)

    # Turn on all warnings, as errors
    set(DOXYGEN_WARNINGS YES)
    set(DOXYGEN_WARN_IF_UNDOCUMENTED YES)
    set(DOXYGEN_WARN_IF_DOC_ERROR YES)
    set(DOXYGEN_WARN_IF_NO_PARAMDOC YES)
    set(DOXYGEN_WARN_AS_ERROR YES)

    # Parallelise
    set(DOXYGEN_NUM_PROC_THREADS 0)  # Auto set based on CPU

    # Graphviz's dot
    set(DOXYGEN_DOT_NUM_THREADS 0)  # Auto set based on CPU
    set(DOXYGEN_DOT_PATH )  # Empty path = search in $PATH

    # Generate command
    doxygen_add_docs(doxygen
                     ALL # Build doxygen on make-all
                     ${LIBNUMERUS_DOXYGEN_SRC} # List of input files
                     )
else (DOXYGEN_FOUND)
    message(WARNING "Doxygen not found. Cannot generate documentation.")
endif (DOXYGEN_FOUND)
