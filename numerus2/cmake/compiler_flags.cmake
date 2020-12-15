# This file contains settings for the ISO C standard, compilation warnings
# and errors, for each build type. Compilers FLAGs that are not supported by
# the used compiler are simply ignored.

message(STATUS
        "Configuring compiler flags, checking if compiler supports them")

include(CheckCCompilerFlag)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

list(APPEND
     COMPILER_FLAGS_WARNINGS
     "-Wall"
     "-Wextra"
     "-Wpedantic"
     "-Wconversion"
     "-Wswitch-default"
     "-Wswitch-enum"
     "-Wuninitialized"
     "-Wshadow"
     "-Wpacked"
     "-Wdouble-promotion"
     "-PIC")
list(APPEND
     COMPILER_FLAGS_DEBUG
     "-O0"
     "-g3"
     "-gdwarf-2")
list(APPEND
     COMPILER_FLAGS_RELEASE
     "-O3"
     "-DNDEBUG"
     "-Werror")

foreach (FLAG IN LISTS COMPILER_FLAGS_WARNINGS)
    string(REPLACE "-" "" FLAG_NO_HYPHEN ${FLAG})
    check_c_compiler_flag(${FLAG} COMPILER_SUPPORTS_${FLAG_NO_HYPHEN})
    if (COMPILER_SUPPORTS_${FLAG_NO_HYPHEN})
        string(APPEND CMAKE_C_FLAGS " ${FLAG}")
    else ()
        string(APPEND UNSUPPORTED_COMPILER_FLAGS " ${FLAG}")
    endif ()
endforeach (FLAG)

foreach (FLAG IN LISTS COMPILER_FLAGS_DEBUG)
    string(REPLACE "-" "" FLAG_NO_HYPHEN ${FLAG})
    check_c_compiler_flag(${FLAG} COMPILER_SUPPORTS_${FLAG_NO_HYPHEN})
    if (COMPILER_SUPPORTS_${FLAG_NO_HYPHEN})
        string(APPEND CMAKE_C_FLAGS_DEBUG " ${FLAG}")
    else ()
        string(APPEND UNSUPPORTED_COMPILER_FLAGS " ${FLAG}")
    endif ()
endforeach (FLAG)

foreach (FLAG IN LISTS COMPILER_FLAGS_RELEASE)
    string(REPLACE "-" "" FLAG_NO_HYPHEN ${FLAG})
    check_c_compiler_flag(${FLAG} COMPILER_SUPPORTS_${FLAG_NO_HYPHEN})
    if (COMPILER_SUPPORTS_${FLAG_NO_HYPHEN})
        string(APPEND CMAKE_C_FLAGS_RELEASE " ${FLAG}")
    else ()
        string(APPEND UNSUPPORTED_COMPILER_FLAGS " ${FLAG}")
    endif ()
endforeach (FLAG)

if (UNSUPPORTED_COMPILER_FLAGS)
    message(WARNING
            "The following compiler flags are not supported: "
            "${UNSUPPORTED_COMPILER_FLAGS}")
endif ()


#Debug mode
#
#-c -fmessage-length=0 -lm -Wdouble-promotion -Wswitch-default -Wswitch-enum -Wuninitialized -Wshadow -Wpacked -Wno-unused-variable -Wformat-security -Wjump-misses-init -Wlogical-not-parentheses -pedantic -Wall -Wextra -Wconversion -std=c11 -Wpadded -Waggregate-return -Wmissing-declarations -fPIC
#
#Release mode
#
#-c -fmessage-length=0 -lm -Wdouble-promotion -Wswitch-default -Wswitch-enum -Wuninitialized -Wshadow -Wpacked -Wno-unused-variable -Wformat-security -Wjump-misses-init -Wlogical-not-parentheses -pedantic -Wall -Wextra -Wconversion -std=c11 -Wpadded -Waggregate-return -Wmissing-declarations -fPIC -pedantic-errors -Werror -flto