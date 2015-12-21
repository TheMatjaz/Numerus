#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "numerus.h"

const char *PROMPT_TEXT = "numerus> ";
const char *WELCOME_TEXT = ""
        "+---------------+\n"
        "| N V M E R V S |\n"
        "+---------------+\n";
const char *INFO_TEXT = ""
"Numerus is an open-source C library that allows conversion and manipulation of roman numerals.\n"
"Version 2.0.0, Command Line Interface\n"
"Copyright (c) 2015 Matjaž Guštin <dev@matjaz.it> http://matjaz.it\n"
"Project page: http://matjaz.it/numerus/\n"
"Source code: https://github.com/TheMatjaz/Numerus\n"
"This software is subject to the terms of the BSD 3-Clause license.\n";
const char *MOO_TEXT = "This is not an easter egg. Try 'ascii'.\n";
const char *PING_TEXT = "Pong.\n";
const char *HELLO_TEXT = "Can you hear me?\n";
const char *AVE_TEXT = "Ave tibi!\n";
const char *HELP_TEXT = ""
"To convert an arabic integer to a roman numeral or vice-versa,\n"
"just type it in the shell and press enter. Other Numerus commands are:\n\n"
""
"syntax        prints the rules of the starndard roman syntax\n"
"?, help       shows this help text\n"
"info, about   shows version, credits, licence and source code location\n"
"exit, quit    ends this shell\n\n"
""
"This command line interface has super cow powers.\n";
const char *QUIT_TEXT = "Vale!\n";
const char *ASCII_TEXT = ""
" ____  _____   ____   ____   ____    ____   _________   _______    ____   ____    _______ \n"
"|_   \\|_   _| |_  _| |_  _| |_   \\  /   _| |_   ___  | |_   __ \\  |_  _| |_  _|  /  ___  |\n"
"  |   \\ | |     \\ \\   / /     |   \\/   |     | |_  \\_|   | |__) |   \\ \\   / /   |  (__ \\_|\n"
"  | |\\ \\| |      \\ \\ / /      | |\\  /| |     |  _|  _    |  __ /     \\ \\ / /     '.___`-. \n"
" _| |_\\   |_      \\ ' /      _| |_\\/_| |_   _| |___/ |  _| |  \\ \\_    \\ ' /     |`\\____) |\n"
"|_____|\\____|      \\_/      |_____||_____| |_________| |____| |___|    \\_/      |_______.'\n";
const char *SYNTAX_TEXT = ""
"The values of the roman characters are:\n\n"
""
" rom int\n"
" -------\n"
"  M 1000\n"
" CM  900\n"
"  D  500\n"
" CD  400\n"
"  C  100\n"
" XC   90\n"
"  L   50\n"
" XL   40\n"
"  X   10\n"
" IX    9\n"
"  V    5\n"
" IV    4\n"
"  I    1\n\n"
""
"The rules to read and write them are:\n"
" * I, X, C, M can be written up to 3 times.\n"
" * V, L, D can be written up to once.\n"
" * When I, X, C, M stand before V, L, D, M, subtract them.\n"
"   Example: IX = X - I = 10 - 1 = 9, CD = D - C = 500 - 100 = 400\n"
" * For everything else, sum from the left to the right.\n"
"   Example: CDLXXXVI = CD        + L  + XXX      + V + I\n"
"                     = 500 - 100 + 50 + 10+10+10 + 5 + 1\n"
"                     = 486\n\n"
""
"The standard structure of a syntactically roman numeral is composed of the \n"
"following characters in this order:\n * 0-3 M\n"
" * 0-1 CM or 0-1 CD or ( 0-1 D and 0-3 C )\n"
" * 0-1 XC or 0-1 XL or ( 0-1 L and 0-3 X )\n"
" * 0-1 IX or 0-1 IV or ( 0-1 V and 0-3 I )\n"
" * or \"NULLA\" instead of any other symbol.\n";
const char *UNKNOWN_COMMAND_TEXT = "Unknown command or wrong roman numeral syntax.\n";
const char *PRETTY_ON_TEXT = "Pretty printing is enabled.\n";
const char *PRETTY_OFF_TEXT = "Pretty printing is disabled.\n";
static int pretty_printing = 1;

/**
 * Do not free a string after it has been trimmed, free the original pointer to it.
 */
char *trim_and_lowercase(char *string) {
    char *start;

    /* Trim leading space */
    while(isspace(*string)) {
        string++;
    }

    if(*string == 0) { /* All spaces */
        return string;
    }
    start = string;

    /* Lowercase any other characters */
    while (*string != '\0') {
        *string = (char) tolower(*string);
        string++;
    }

    /* Reached end of string, go bacwards and trim trailing space */
    string--;
    while (isspace(*string)) {
        string--;
    }
    *(string+1) = '\0';

    return start;
}

char *trim_lowercase_get_first_word(char *string) {
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

int string_is_zero(char *zero_as_string) {
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
int _num_parse_command(char *command) {
    if (strcmp(command, "?") == 0 || strcmp(command, "help") == 0) {
        printf("%s", HELP_TEXT);
        return 1;
    } else if (strcmp(command, "moo") == 0) {
        printf("%s", MOO_TEXT);
        return 1;
    } else if (strcmp(command, "ascii") == 0) {
        printf("%s", ASCII_TEXT);
        return 1;
    } else if (strcmp(command, "info") == 0 || strcmp(command, "about") == 0) {
        printf("%s", INFO_TEXT);
        return 1;
    } else if (strcmp(command, "syntax") == 0) {
        printf("%s", SYNTAX_TEXT);
        return 1;
    } else if (strcmp(command, "hello") == 0) {
        printf("%s", HELLO_TEXT);
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
    } else if (strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0) {
        printf("%s", QUIT_TEXT);
        return 0; /* 0 to stop the REPL */
    } else if (strcmp(command, "") == 0) {
        /* No inserted command, just <enter> */
        return 1;
    } else {
        long value;
        if (string_is_zero(command)) {
            printf("%s\n", numerus_long_to_roman(0));
            return 1;
        } else if ((value = strtol(command, NULL, 10)) != 0){
            if (pretty_printing == 1) {
                printf("%s\n", numerus_pretty_print_long_numerals(
                        numerus_long_to_roman(value)));
                return 1;
            } else {
                printf("%s\n", numerus_long_to_roman(value));
                return 1;
            }
        } else if ((value = numerus_roman_to_long(command)) != NUMERUS_MAX_LONG_VALUE + 1) {
            printf("%li\n", value);
            return 1;
        } else {
            printf("%s", UNKNOWN_COMMAND_TEXT);
            return 2;
        }
    }
}

void numerus_repl(int argc, char **args) {
    int cycle_repl = 1;
    char *command;
    size_t line_buffer_size = 100;
    char *line = malloc(line_buffer_size);
    printf("%s", WELCOME_TEXT);
    while (cycle_repl) {
        printf("%s", PROMPT_TEXT);
        if (getline(&line, &line_buffer_size, stdin) == -1) {
            break;
        } else {
            command = trim_lowercase_get_first_word(line);
            cycle_repl = _num_parse_command(command);
        }
    }
    free(line);
}

int main(int argc, char **args) {
    numerus_repl(argc, args);
    return 0;
}