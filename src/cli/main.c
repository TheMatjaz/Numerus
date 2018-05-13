/**
 * @file main.c
 * @brief Numerus example main that starts the CLI.
 * @copyright Copyright © 2015-2018, Matjaž Guštin <dev@matjaz.it>
 * <https://matjaz.it>. All rights reserved.
 * @license This file is part of the Numerus project which is released under
 * the BSD 3-clause license.
 *
 * This file contains an extremly simple example of how to use the Numerus
 * library to start a command line interface that allows simple and
 * user-friendly conversion of roman numerals.s
 */

#include "../include/numerus.h"


/**
 * Numerus example main that only starts the Numerus CLI.
 */
int main(int argc, char **args) {
    numerus_cli(argc, args);
    return 0;
}
