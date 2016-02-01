/**
 * @file numerus_cli.c
 * @brief Numerus command line interface for user-friendly conversions.
 * @copyright Copyright © 2015-2016, Matjaž Guštin <dev@matjaz.it>
 * <http://matjaz.it>. All rights reserved.
 * @license This file is part of the Numerus project which is released under
 * the BSD 3-clause license.
 *
 * This file contains a command line interface that converts any typed value
 * to a roman numeral or vice-versa. To use it, just call
 *
 * `numerus_cli(argc, args);`
 *
 * with `argc` and `args` the arguments of the main. This allows any command
 * line parameters passed to the numerus executable to be interpreted as if
 * they were written withing the command line interface.
 */

#include <stdio.h>   /* For `printf()` */
#include <stdlib.h>  /* For `malloc()`, `free()`, `strtod()` */
#include <ctype.h>   /* For `isspace()`, `tolower()` */
#include <string.h>  /* For `strcmp()` */
#include "numerus.h"

/**
 * @internal
 * Macro to indicate that the CLI should continue its execution.
 */
#define NUMERUS_PROMPT_AGAIN 1


/**
 * @internal
 * Macro ro indicate that the CLI should terminate.
 */
#define NUMERUS_STOP_REPL 0

static const char *PROMPT_TEXT = "numerus> ";
static const char *WELCOME_TEXT = ""
"+-----------------+\n"
"|  N V M E R V S  |\n"
"+-----------------+\n";
static const char *INFO_TEXT = ""
"Numerus, C library for conversion and manipulation of roman numerals.\n"
"Version 2.0.0, Command Line Interface\n"
"Copyright (c) 2015-2016 Matjaž Guštin <dev@matjaz.it> http://matjaz.it\n"
"This software is subject to the terms of the BSD 3-Clause license.\n"
"Project page and source code: https://github.com/TheMatjaz/Numerus\n";
static const char *MOO_TEXT = "This is not an easter egg. Try `ascii`.\n";
static const char *PING_TEXT = "Pong.\n";
static const char *AVE_TEXT = "Ave tibi!\n";
static const char *HELP_TEXT = ""
"For ANY information about the library or the syntax of roman numeral, \n"
"check the documentation available on https://thematjaz.github.io/Numerus/\n\n"
""
"To convert an (arabic) integer to a roman numeral or vice-versa,\n"
"just type it in the shell and press enter.\n"
"Other Numerus commands are:\n\n"
""
"pretty        switches on/off the pretty printing of long roman numerals\n"
"              (with overlined notation instead of underscore notation)\n"
"              and the pretty printing of values as integer and fractional part\n"
"?, help       shows this help text\n"
"info, about   shows version, credits, licence, repository of Numerus\n"
"exit, quit    ends this shell\n\n"
""
"We also have: moo, ping, ave.\n";
static const char *QUIT_TEXT = "Vale!\n";
static const char *ASCII_TEXT = ""
" ____  _____   ____   ____   ____    ____   _________   _______    ____   ____    _______ \n"
"|_   \\|_   _| |_  _| |_  _| |_   \\  /   _| |_   ___  | |_   __ \\  |_  _| |_  _|  /  ___  |\n"
"  |   \\ | |     \\ \\   / /     |   \\/   |     | |_  \\_|   | |__) |   \\ \\   / /   |  (__ \\_|\n"
"  | |\\ \\| |      \\ \\ / /      | |\\  /| |     |  _|  _    |  __ /     \\ \\ / /     '.___`-. \n"
" _| |_\\   |_      \\ ' /      _| |_\\/_| |_   _| |___/ |  _| |  \\ \\_    \\ ' /     |`\\____) |\n"
"|_____|\\____|      \\_/      |_____||_____| |_________| |____| |___|    \\_/      |_______.'\n";
static const char *UNKNOWN_COMMAND_TEXT = "Unknown command or wrong roman numeral syntax:\n";
static const char *PRETTY_ON_TEXT = "Pretty printing is enabled.\n";
static const char *PRETTY_OFF_TEXT = "Pretty printing is disabled.\n";
static int pretty_printing = 0;


/**
 * Returns a pointer the first word of string that is terminated after that.
 *
 * Modifies the original string, because of the null-termination after the
 * first word.
 *
 * The extracted word is also lowercased. Do not free() the pointer returned
 * by this function, as it may not point to the start of the original string.
 * free() the original pointer to it.
 *
 * @param char* string to find the first word in.
 * @returns char* pointer to the start of the first word in the passed string.
 */
static char *_num_get_first_word_trimmed_lowercased(char *string) {
    while(isspace(*string)) {
        string++;
    }
    if(*string == '\0') {
        /* The string was full of whitespaces */
        return string;
    }
    char *first_word_start = string;
    while (*string != '\0' && !isspace(*string)) {
        *string = (char) tolower(*string);
        string++;
    }
    *string = '\0';
    return first_word_start;
}


/**
 * Verifies if a string is just any strange form of a double with value zero.
 *
 * This includes: (-)0, (-)000.000, (-)000,000 (with decimal comma).
 *
 * @param char* containing a zero-valued double.
 * @returns short 1 if the string contains a zero-valued double, 0 otherwise.
 */
static short _num_string_is_double_zero(char *zero_as_string) {
    if (*zero_as_string == '-') {
        zero_as_string++;
    }
    while (*zero_as_string == '0') {
        zero_as_string++;
    }
    if (*zero_as_string == '.' || *zero_as_string == ',') {
        zero_as_string++;
        if (*zero_as_string == '0') {
            while (*zero_as_string == '0') {
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
 * Tries to convert the string from a double to roman numeral then vice versa
 * and prints the result to stdout.
 *
 * If no conversion can be done (for instance because of wrong roman syntax or
 * the string is not a value not a roman), an error is printed. If the pretty
 * printing options is activated, performs it.
 *
 * @param char* string containing a value or a roman numeral.
 * @returns void since prints the result to stdout.
 */
void _num_convert_to_other_form_and_print(char *string) {
    double value;
    char *roman;
    int errcode;
    /* This check is necessary because strtod() returns 0 in case of errors
     * AND in case finds an actual zero. Duh! */
    if (_num_string_is_double_zero(string)) {
        printf("%s\n", roman = numerus_double_to_roman(0, NULL));
        free(roman);
        return;
    }
    value = strtod(string, NULL);
    if (value != 0) {
        /* The string is a double */
        roman = numerus_double_to_roman(value, &errcode);
        if (errcode != NUMERUS_OK) {
            printf("%s\n", numerus_explain_error(errcode));
            if (errcode != NUMERUS_ERROR_MALLOC_FAIL) {
                free(roman);
            }
            return;
        }
        /* Successful conversion */
        if (pretty_printing == 1) {
            /* Enabled pretty printing */
            char *roman_pretty = numerus_overline_long_numerals(roman, &errcode);
            if (errcode != NUMERUS_OK) {
                printf("%s\n", numerus_explain_error(errcode));
            } else {
                /* Successful transformed into pretty format */
                printf("%s\n", roman_pretty);
                free(roman_pretty);
            }
        } else {
            /* Disabled pretty printing, just print the roman numeral */
            printf("%s\n", roman);
        }
        free(roman);
        return;
    }
    /* The string is not a double, trying as a roman numeral */
    value = numerus_roman_to_double(string, &errcode);
    if (errcode == NUMERUS_OK) {
        /* The string is a roman numeral */
        if (pretty_printing == 1) {
            /* Pretty printing enabled */
            char *pretty_value = numerus_create_pretty_value_as_double(value);
            if (pretty_value == NULL) {
                printf("%s\n", numerus_explain_error(NUMERUS_ERROR_MALLOC_FAIL));
            } else {
                /* Successful transformed into pretty format */
                printf("%s\n", pretty_value);
                free(pretty_value);
            }
        } else {
            /* Pretty printing disabled, just print the value */
            printf("%f\n", value);
        }
    } else {
        /* The command is not a value/a roman numeral or it has wrong syntax */
        printf("%s -> %s\n", UNKNOWN_COMMAND_TEXT, numerus_explain_error(errcode));
    }
}


/**
 * Parses the already cleaned command and reacts accordingly.
 *
 * Command should be already trimmed and lowercased.
 */
static int _num_parse_command(char *command) {
    if (strcmp(command, "?") == 0 || strcmp(command, "help") == 0) {
        printf("%s", HELP_TEXT);
        return NUMERUS_PROMPT_AGAIN;
    } else if (strcmp(command, "moo") == 0) {
        printf("%s", MOO_TEXT);
        return NUMERUS_PROMPT_AGAIN;
    } else if (strcmp(command, "ascii") == 0) {
        printf("%s", ASCII_TEXT);
        return NUMERUS_PROMPT_AGAIN;
    } else if (strcmp(command, "info") == 0 || strcmp(command, "about") == 0) {
        printf("%s", INFO_TEXT);
        return NUMERUS_PROMPT_AGAIN;
    } else if (strcmp(command, "ave") == 0) {
        printf("%s", AVE_TEXT);
        return NUMERUS_PROMPT_AGAIN;
    } else if (strcmp(command, "pretty") == 0) {
        /* Toggles pretty printing */
        if (pretty_printing == 1) {
            pretty_printing = 0;
            printf("%s", PRETTY_OFF_TEXT);
            return NUMERUS_PROMPT_AGAIN;
        } else {
            pretty_printing = 1;
            printf("%s", PRETTY_ON_TEXT);
            return NUMERUS_PROMPT_AGAIN;
        }
    } else if (strcmp(command, "ping") == 0) {
        printf("%s", PING_TEXT);
        return NUMERUS_PROMPT_AGAIN;
    } else if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0) {
        printf("%s", QUIT_TEXT);
        return NUMERUS_STOP_REPL;
    } else if (*command == '\0') {
        /* No inserted command, just an <enter> */
        return NUMERUS_PROMPT_AGAIN;
    } else {
        _num_convert_to_other_form_and_print(command);
        return NUMERUS_PROMPT_AGAIN;
    }
}


/**
 * Starts a command line interface that converts any typed value to a roman
 * numeral or vice-versa.
 *
 * The `argc` and `args` arguments of the main may be passed to it. This allows
 * any command line parameters passed to the numerus executable to be
 * interpreted as if they were written withing the command line interface.
 * To avoid this option, set `argc` to 0 and `args` to anything, e.g. NULL.
 *
 * @param argc int number of main arguments. Set to 0 to disable parsing of
 * main arguments.
 * @param args array of main arguments to be parsed as commands.
 * @returns int status code: 0 if everything went ok or a NUMERUS_ERROR_*
 * otherwise.
 */
int numerus_cli(int argc, char **args) {
    char *command;
    /* line_buffer_size = 50 enough for every command,
     * gets reallocated by getline() if not enough */
    size_t line_buffer_size = 50;
    char *line = malloc(line_buffer_size);
    if (line == NULL) {
        numerus_error_code = NUMERUS_ERROR_MALLOC_FAIL;
        return NUMERUS_ERROR_MALLOC_FAIL;
    }
    if (argc > 1) {
        /* Parse main arguments and exit */
        args++;
        pretty_printing = 0;
        while (argc > 1) {
            command = _num_get_first_word_trimmed_lowercased(*args);
            _num_parse_command(command);
            args++;
            argc--;
        }
    } else {
        /* Enter command line interface */
        pretty_printing = 1;
        printf("%s", WELCOME_TEXT);
        int command_result = NUMERUS_PROMPT_AGAIN;
        while (command_result == NUMERUS_PROMPT_AGAIN) {
            printf("%s", PROMPT_TEXT);
            if (getline(&line, &line_buffer_size, stdin) == -1) {
                break;
            } else {
                command = _num_get_first_word_trimmed_lowercased(line);
                command_result = _num_parse_command(command);
            }
        }
    }
    free(line);
    return 0;
}
