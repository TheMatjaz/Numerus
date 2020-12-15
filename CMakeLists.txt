cmake_minimum_required(VERSION 3.10)
project(Numerus C)

set(CMAKE_C_STANDARD 11)

include_directories(inc/)
add_library(numerus
            src/numerus_constants.c
            src/numerus_to_roman.c
            src/numerus_dictionary.h)
