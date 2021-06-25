//
// Created by AndiSwiss on 11.02.21.
//

#ifndef TESTING_FRAMEWORK_CPLUSPLUS
#define TESTING_FRAMEWORK_CPLUSPLUS

#endif //TESTING_FRAMEWORK_CPLUSPLUS

#include <iostream>
using namespace std;

/**
 * Enum for colored terminal output.
 * See https://en.cppreference.com/w/cpp/language/enum
 */
enum Color {
    red = 31, green = 32, yellow = 33, blue = 34, magenta = 35, cyan = 36
};


void write_colored_text(const string &message, Color color = red);