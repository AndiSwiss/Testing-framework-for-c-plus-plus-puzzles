//
// Created by AndiSwiss on 11.02.21.
//

#include "helpers.h"
#include <iostream>


/**
 * Helper method for writing colored text to the terminal.
 * Color: possible values: see enum in the beginning of this file
 * Default color is red (as defined in the signature of this method in the beginning of this file)
 *
 * See other colors/options:
 *    https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
 * And/Or:
 *    https://en.wikipedia.org/wiki/ANSI_escape_code#Colors

 */
void write_colored_text(const string &message, Color color) {
    // NOTE: Color is an enum I defined in the beginning of this file

    /*
     * Note: I initially declared the parameter to pass like:
     *            write_colored_text(string message) {...}
     * That worked fine,but CLion complained:
     *            "Clang-Tidy: The parameter 'message' is copied for each invocation but only used as a const reference;
     *            consider making it a const reference."
     * With auto-refactoring by CLion, it was changed to a const reference:
     *            write_colored_text(const string &message) {...}
     */

    // Setting terminal color/options:
    cout << "\033[1;" << int(color) << "m";
    cout << message;
    // Reset terminal color/options:
    cout << "\033[0m";
}