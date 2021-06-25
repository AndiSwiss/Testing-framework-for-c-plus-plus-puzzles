//
// Created by AndiSwiss on 11.02.21.
//

#ifndef TESTING_FRAMEWORK_CPLUSPLUS
#define TESTING_FRAMEWORK_CPLUSPLUS

#endif //TESTING_FRAMEWORK_CPLUSPLUS

#include "../helpers/helpers.h"
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <chrono>
#include <list>
#include <dirent.h>
#include <cmath>
#include <regex>
#include <cstdio>

using namespace std;

void string_in_out_testing_v2(const string &base_path,
                              const string &problem_no,
                              list<int> &files_to_run,
                              void (*code_to_execute)());