#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "numerus.h"

const char *PROMPT_TEXT = "numerus> ";
const char *WELCOME_TEXT = ""
"+-----------------+\n"
"|  N V M E R V S  |\n"
"+-----------------+\n";
const char *INFO_TEXT = ""
"Numerus, C library for conversion and manipulation of roman numerals.\n"
"Version 2.0.0, Command Line Interface\n"
"Copyright (c) 2015 Matjaž Guštin <dev@matjaz.it> http://matjaz.it\n"
"This software is subject to the terms of the BSD 3-Clause license.\n"
"Project page and source code: https://github.com/TheMatjaz/Numerus\n";
const char *MOO_TEXT = "This is not an easter egg. Try `ascii`.\n";
const char *PING_TEXT = "Pong.\n";
const char *HELLO_TEXT = "Can you hear me?\n";
const char *AVE_TEXT = "Ave tibi!\n";
const char *HELP_TEXT = ""
"To convert an (arabic) integer to a roman numeral or vice-versa,\n"
"just type it in the shell and press enter.\n"
"Other Numerus commands are:\n\n"
""
"syntax        prints the syntax rules to write and read roman numerals\n"
"pretty        switches on/off the pretty printing of long roman numerals\n"
"              with overlined notation instead of underscore notation\n"
"?, help       shows this help text\n"
"info, about   shows version, credits, licence, repository of Numerus\n"
"exit, quit    ends this shell\n\n"
""
"We also have: moo, hello, ping, ave.\n";
const char *QUIT_TEXT = "Vale!\n";
const char *ASCII_TEXT = ""
" ____  _____   ____   ____   ____    ____   _________   _______    ____   ____    _______ \n"
"|_   \\|_   _| |_  _| |_  _| |_   \\  /   _| |_   ___  | |_   __ \\  |_  _| |_  _|  /  ___  |\n"
"  |   \\ | |     \\ \\   / /     |   \\/   |     | |_  \\_|   | |__) |   \\ \\   / /   |  (__ \\_|\n"
"  | |\\ \\| |      \\ \\ / /      | |\\  /| |     |  _|  _    |  __ /     \\ \\ / /     '.___`-. \n"
" _| |_\\   |_      \\ ' /      _| |_\\/_| |_   _| |___/ |  _| |  \\ \\_    \\ ' /     |`\\____) |\n"
"|_____|\\____|      \\_/      |_____||_____| |_________| |____| |___|    \\_/      |_______.'\n";
const char *SYNTAX_TEXT = "\n"
"Character values\n"
"================\n\n"
"+------+-------+\n"
"| char | value |\n"
"+------+-------+\n"
"|   M  |  1000 |\n"
"|   D  |   500 |\n"
"|   C  |   100 |\n"
"|   L  |    50 |\n"
"|   X  |    10 |\n"
"|   V  |     5 |\n"
"|   I  |     1 |\n"
"+------+-------+\n\n"
"When a character with a smaller value is placed before a character with\n"
"bigger value, they have to be subracted: bigger - smaller, resulting in:\n"
"+------+-------+\n"
"| char | value |\n"
"+------+-------+\n"
"|  CM  |   900 |\n"
"|  CD  |   400 |\n"
"|  XC  |    90 |\n"
"|  XL  |    40 |\n"
"|  IX  |     9 |\n"
"|  IV  |     4 |\n"
"+------+-------+\n\n"
"The rules to read roman numerals are:\n"
" 1. When I, X, C, M stand before V, L, D, M, subtract them as shown above.\n"
" 2. For everything else, sum from the left to the right.\n"
"An example: CDLXXXVI = CD        + L  + XXX      + V + I\n"
"                     = 500 - 100 + 50 + 10+10+10 + 5 + 1\n"
"                     = 486\n\n"
"Standard roman syntax\n"
"=====================\n\n"
"The syntax of a standard roman numeral is:\n"
" * 0-3 M\n"
" * 0-1 CM or 0-1 CD or ( 0-1 D and 0-3 C )\n"
" * 0-1 XC or 0-1 XL or ( 0-1 L and 0-3 X )\n"
" * 0-1 IX or 0-1 IV or ( 0-1 V and 0-3 I )\n"
"This allows writing values from 1 to 3999.\n\n"
"Zero\n"
"====\n\n"
"Numerus uses the notation \"NULLA\" for the numeral of value 0 (zero).\n"
"The word \"nulla\" means \"nothing\" in latin.\n\n"
"Negative values\n"
"===============\n\n"
"Numerus uses the '-' (minus) sign before any numeral to indicate that it is\n"
"a negative numeral.\n\n"
"Long numerals\n"
"=============\n\n"
"Values bigger than +-3999 should be indicated with the overlining method\n"
"where overlined characters have their value multiplied by 1000. An example:\n"
"___\n"
"MIIXVI = 1002016\n\b"
"Those numerals have the following syntax:\n"
" * overlined standard roman syntax numeral\n"
" * non-overlined standar roman synax numeral without any leading Ms\n\n"
"Numerus calls these numerals \"long\" once, resembling the `long int` value \n"
"and uses underscores as linear notation for them: the numeral part between \n"
"single underscores is interpreted as overlined. An example:\n"
"The number above 1002016 is written as _MII_XVI\n";
const char *UNKNOWN_COMMAND_TEXT = "Unknown command or wrong roman numeral syntax.\n";
const char *PRETTY_ON_TEXT = "Pretty printing is enabled.\n";
const char *PRETTY_OFF_TEXT = "Pretty printing is disabled.\n";
static int pretty_printing = 0;

/**
 * Do not free a string after it has been trimmed, free the original pointer to it.
 */
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
    } else if (strcmp(command, "exit") == 0
               || strcmp(command, "quit") == 0) {
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
        } else if ((value = numerus_roman_to_long(command))
                   != NUMERUS_MAX_LONG_VALUE + 1) {
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
    size_t line_buffer_size = 50; /* Suffices for every command, gets
                                   * reallocated by getline() if not enough */
    char *line = malloc(line_buffer_size);
    if (argc > 1) { /* Command line arguments to interprete and exit */
        args++;
        pretty_printing = 0;
        while (argc > 1) {
            command = trim_lowercase_get_first_word(*args);
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
                command = trim_lowercase_get_first_word(line);
                cycle_repl = _num_parse_command(command);
            }
        }
    }
    free(line);
}

int main(int argc, char **args) {
    numerus_repl(argc, args);
    return 0;
}