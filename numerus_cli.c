#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "numerus_internal.h"

const char *PROMPT_TEXT = "numerus> ";
const char *WELCOME_TEXT = ""
"+-----------------+\n"
"|  N V M E R V S  |\n"
"+-----------------+\n";
const char *INFO_TEXT = ""
"Numerus, C library for conversion and manipulation of roman numerals.\n"
"Version 2.0.0, Command Line Interface\n"
"Copyright (c) 2015-2016 Matjaž Guštin <dev@matjaz.it> http://matjaz.it\n"
"This software is subject to the terms of the BSD 3-Clause license.\n"
"Project page and source code: https://github.com/TheMatjaz/Numerus\n";
const char *MOO_TEXT = "This is not an easter egg. Try `ascii`.\n";
const char *PING_TEXT = "Pong.\n";
const char *AVE_TEXT = "Ave tibi!\n";
const char *HELP_TEXT = ""
"For ANY information about the library or the syntax of roman numeral, "
"check the documentation available on https://thematjaz.github.io/Numerus/\n\n"
""
"To convert an (arabic) integer to a roman numeral or vice-versa,\n"
"just type it in the shell and press enter.\n"
"Other Numerus commands are:\n\n"
""
"pretty        switches on/off the pretty printing of long roman numerals\n"
"              (with overlined notation instead of underscore notation)\n"
"              and the pretty printing of values as integer and fractional part"
"?, help       shows this help text\n"
"info, about   shows version, credits, licence, repository of Numerus\n"
"exit, quit    ends this shell\n\n"
""
"We also have: moo, ping, ave.\n";
const char *QUIT_TEXT = "Vale!\n";
const char *ASCII_TEXT = ""
" ____  _____   ____   ____   ____    ____   _________   _______    ____   ____    _______ \n"
"|_   \\|_   _| |_  _| |_  _| |_   \\  /   _| |_   ___  | |_   __ \\  |_  _| |_  _|  /  ___  |\n"
"  |   \\ | |     \\ \\   / /     |   \\/   |     | |_  \\_|   | |__) |   \\ \\   / /   |  (__ \\_|\n"
"  | |\\ \\| |      \\ \\ / /      | |\\  /| |     |  _|  _    |  __ /     \\ \\ / /     '.___`-. \n"
" _| |_\\   |_      \\ ' /      _| |_\\/_| |_   _| |___/ |  _| |  \\ \\_    \\ ' /     |`\\____) |\n"
"|_____|\\____|      \\_/      |_____||_____| |_________| |____| |___|    \\_/      |_______.'\n";
const char *UNKNOWN_COMMAND_TEXT = "Unknown command or wrong roman numeral syntax.\n";
const char *PRETTY_ON_TEXT = "Pretty printing is enabled.\n";
const char *PRETTY_OFF_TEXT = "Pretty printing is disabled.\n";
static int pretty_printing = 0;

/**
 * Do not free a string after it has been trimmed, free the original pointer to it.
 */
static char *_num_trim_lowercase_get_first_word(char *string) {
    char *start;

    /* Trim leading space */
    while(isspace(*string)) {
        string++;
    }

    if(*string == 0) {/* All spaces */
        return string;
    }
    start = string;

    /* Lowercase any other characters */
    while (*string != '\0' && !isspace(*string)) {
        *string = (char) tolower(*string);
        string++;
    }
    *string = '\0';

    return start;
}

static int _num_string_is_zero(char *zero_as_string) {
    if (*zero_as_string == '-') {
        zero_as_string++;
    }
    while (*zero_as_string == '0') {
        zero_as_string++;
    }
    if (*zero_as_string == '.' || *zero_as_string == ',') {
        zero_as_string++;
        if (*zero_as_string == '0') {
            while(*zero_as_string == '0') {
                zero_as_string++;
            }
        } else {
            return 0;
        }
    }
    if (*zero_as_string == '\0') {
        return 1;
    } else {
        return 0;
    }
}

/**
 * Command should be already trimmed and lowercased
 */
static int _num_parse_command(char *command) {
    if (strcmp(command, "?") == 0
        || strcmp(command, "help") == 0) {
        printf("%s", HELP_TEXT);
        return 1;
    } else if (strcmp(command, "moo") == 0) {
        printf("%s", MOO_TEXT);
        return 1;
    } else if (strcmp(command, "ascii") == 0) {
        printf("%s", ASCII_TEXT);
        return 1;
    } else if (strcmp(command, "info") == 0
               || strcmp(command, "about") == 0) {
        printf("%s", INFO_TEXT);
        return 1;
    } else if (strcmp(command, "ave") == 0) {
        printf("%s", AVE_TEXT);
        return 1;
    } else if (strcmp(command, "pretty") == 0) {
        if (pretty_printing == 1) {
            pretty_printing = 0;
            printf("%s", PRETTY_OFF_TEXT);
            return 1;
        } else {
            pretty_printing = 1;
            printf("%s", PRETTY_ON_TEXT);
            return 1;
        }
    } else if (strcmp(command, "ping") == 0) {
        printf("%s", PING_TEXT);
        return 1;
    } else if (strcmp(command, "exit") == 0
               || strcmp(command, "quit") == 0) {
        printf("%s", QUIT_TEXT);
        return 0; /* 0 to stop the REPL */
    } else if (strcmp(command, "") == 0) {
        /* No inserted command, just <enter> */
        return 1;
    } else {
        double value;
        char *roman;
        int errcode = 0;
        if (_num_string_is_zero(command)) { // a double of any form with value 0.0 is typed
            printf("%s\n", roman = numerus_double_to_roman(0, NULL));
            free(roman);
            return 1;
        }
        if ((value = strtod(command, NULL)) != 0) { // a double is typed
            if (pretty_printing == 1) {
                int errcode_pretty;
                printf("%s\n", roman = numerus_pretty_print_long_numerals(
                        numerus_double_to_roman(value, &errcode), &errcode_pretty));
                if (errcode != NUMERUS_OK) {
                    printf("%s\n", numerus_explain_error(errcode));
                }
                if (errcode_pretty != NUMERUS_OK) {
                    printf("%s\n", numerus_explain_error(errcode_pretty));
                }
                free(roman);
                return 1;
            } else {
                printf("%s\n", roman = numerus_double_to_roman(value, &errcode));
                if (errcode != NUMERUS_OK) {
                    printf("%s\n", numerus_explain_error(errcode));
                }
                free(roman);
                return 1;
            }
        }
        value = numerus_roman_to_double(command, &errcode);
        if (errcode == NUMERUS_OK) { // a roman is typed
            if (pretty_printing == 1) {
                char *pretty_value;
                printf("%s\n", pretty_value = numerus_pretty_print_value_as_double(
                        value));
                if (pretty_value != NULL) {
                    free(pretty_value);
                }
                return 1;
            } else {
                printf("%15.15f\n", value);
                return 1;
            }
        } else {
            printf("%s-> %s\n", UNKNOWN_COMMAND_TEXT, numerus_explain_error(errcode));
            return 2;
        }
    }
}

void numerus_repl(int argc, char **args) {
    int cycle_repl = 1;
    char *command;
    size_t line_buffer_size = 50; /* Suffices for every command, gets
                                   * reallocated by getline() if not enough */
    char *line = malloc(line_buffer_size);
    if (argc > 1) { /* Command line arguments to interprete and exit */
        args++;
        pretty_printing = 0;
        while (argc > 1) {
            command = _num_trim_lowercase_get_first_word(*args);
             _num_parse_command(command);
            args++;
            argc--;
        }
    } else { /* Enter shell */
        pretty_printing = 1;
        printf("%s", WELCOME_TEXT);
        while (cycle_repl) {
            printf("%s", PROMPT_TEXT);
            if (getline(&line, &line_buffer_size, stdin) == -1) {
                break;
            } else {
                command = _num_trim_lowercase_get_first_word(line);
                cycle_repl = _num_parse_command(command);
            }
        }
    }
    free(line);
}
