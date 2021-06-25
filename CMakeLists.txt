cmake_minimum_required(VERSION 3.17)
project(Testing_framework_cplusplus)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(Testing_framework_cplusplus
        # C-Apps (for code-completion only - If you want to run C-Apps with C-Lion, change running environment accordingly):
        src/_old_c_applications/hello_world_c.c

        # C++ main app:
        src/main.cpp

        # My libraries:
        src/my_libs/c_modules/my_c_module.c
        src/my_libs/c_modules/my_c_module.h
        src/my_libs/local_testing/string_in_out_testing.cpp
        src/my_libs/local_testing/string_in_out_testing.h
        src/my_libs/local_testing/string_in_out_testing_v2.cpp
        src/my_libs/local_testing/string_in_out_testing_v2.h
        src/my_libs/helpers/helpers.cpp
        src/my_libs/helpers/helpers.h

        # C++ all sub-applications:
        src/codewars/easy_puzzles/pXXX_YYY.cpp
        src/codewars/easy_puzzles/pXXX_YYY.h)