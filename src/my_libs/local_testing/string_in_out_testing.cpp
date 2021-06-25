/*
 *  Created by AndiSwiss on 10.10.20
 *  https://github.com/AndiSwiss
 */


#include "string_in_out_testing.h"
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <chrono>


string sub_path, in_name, out_name, expected_name;    // Initialized in prepare_ide()
//int old_stdin;  // Re-assigning back to the original stdin doesn't work yet somehow
int old_stdout;   // For re-assigning back to the original stdout

// Variables to save time stamps; They have to be initialized here because of the auto-type:
auto start = chrono::high_resolution_clock::now();
auto stop = chrono::high_resolution_clock::now();

/**
 * - Construct the used filenames
 * - Replacing cin and/or cout:
 *   - with freopen(...):
 *   - http://www.cplusplus.com/reference/cstdio/freopen/
 *   - https://en.cppreference.com/w/cpp/io/c/freopen
 */
void prepare_ide(const string &base_path, const string &problem_no, int file_no) {
    if (file_no == -1) {
        cout << "Which file-set? ";
        cin >> file_no;
    }


    sub_path = base_path + "/in_out_exp_files/" + problem_no + "/";
    string f_no = to_string(file_no);
    in_name = sub_path + "in" + f_no + ".txt";
    out_name = sub_path + "out" + f_no + ".txt";
    expected_name = sub_path + "exp" + f_no + ".txt";

    write_colored_text("Reading files for problem " + problem_no + ", file-nr " + f_no + "...", green);

    /*
     * Note: I initially declared the parameter to pass like:
     *            prepare_ide(string problem_no, int file_no) {...}
     * That worked fine,but CLion complained:
     *            "Clang-Tidy: The parameter 'message' is copied for each invocation but only used as a const reference;
     *            consider making it a const reference."
     * With auto-refactoring by CLion, it was changed to a const reference:
     *            prepare_ide(const string &problem_no, int file_no) {...}
     */

    /*
     * For input file:
     * NOTE: this method requires a char* (or char-array) as first parameter.
     * For conversion, use ...c_str()
     * See https://stackoverflow.com/questions/7352099/stdstring-to-char
     *
     * For replacing 'cout', just write 'stdout'
     */

    // First save the old file-descriptors:
    // -> In linux, the following streams are associated:
    //    - 0: stdin
    //    - 1: stdout
    //    - 2: stderr
//    old_stdin = dup(0);
    old_stdout = dup(1);
    if (!freopen(in_name.c_str(), "r", stdin)) {
        throw runtime_error("Problems reading from in-file " + in_name);
    }
    if (!freopen(out_name.c_str(), "w", stdout)) {
        throw runtime_error("Problems reading from out-file " + out_name);
    }

    // Start the code timer:
    start = chrono::high_resolution_clock::now();
}


/**
 * Compares p**_expected*.txt (expected) with p**_output*.txt (actual)
 */
void execute_tests() {

    // Stop the timer:
    stop = chrono::high_resolution_clock::now();

    /*
     * Recreate the standard output to the terminal:
     * https://stackoverflow.com/questions/1908687/how-to-redirect-the-output-back-to-the-screen-after-freopenout-txt-a-stdo/1910044#1910044
     * Somehow, this only works once - the second time, the output didn't work anymore 
     *    (e.g., when I tried to call this file string_in_out_testing.cpp in a loop)
     */
    fclose(stdout);
    FILE *fp_out = fdopen(old_stdout, "w");    // Open old stdout as a stream
    // Reassign the terminal to stdin/stdout:
    *stdout = *fp_out;                          // NOLINT(misc-non-copyable-objects)

    /*
     * Recreate the standard input to the terminal.
     * DOESN'T YET WORK ?!   
     * (But this is not necessary for this project, except if I would want to run multiple times in a sequence)
     */
//    fclose(stdin);
//    FILE  *fp_in = fdopen(old_stdin, "r");  // Open old stdin as a stream
//    *stdin = *fp_in;                        // NOLINT(misc-non-copyable-objects)



    // Open 'out_name' and 'expected_name' for 'r'-access:
    // see https://www.tutorialspoint.com/cplusplus/cpp_files_streams.htm
    // ifstream ... : represents input file stream (for reading from files)
    // ofstream ... : represents output file stream (for creating and writing to files)
    // For comparing, you only need reading:
    ifstream out_file(out_name);
    ifstream expected_file(expected_name);

    bool skip_tests = false;
    if (!out_file.is_open()) {
        write_colored_text("!! Problems reading from file '" + out_name + "'\n");
        skip_tests = true;
    }
    if (!expected_file.is_open()) {
        write_colored_text("!! Problems reading from file '" + expected_name + "'\n");
        skip_tests = true;
    }

    // Run actual tests: do string-comparison line by line
    if (skip_tests) {
        write_colored_text("-> Tests will be skipped !!\n");
    } else {
        write_colored_text("\nExecuting tests:", blue);
        write_colored_text("\n----------------\n", blue);
//        cout << "\n----------------" << endl;
        int line_nr = 0, error_count = 0;
        string out_line, exp_line;

        while (getline(out_file, out_line)) {
            if (getline(expected_file, exp_line)) {
                line_nr++;
                /*
                 * NOTE: I wanted to compare the two string as described in
                 * various online resources, such as https://www.geeksforgeeks.org/stdstringcompare-in-c/
                 * With:
                 *      if (out_line.compare(exp_line) == 0)
                 * But CLion showed "Clang-Tidy: Do not use 'compare' to test equality of strings; use the string
                 *      equality operator instead"
                 *
                 * It seems the above construct would be great to see if a string is shorter or longer than the other:
                 *      int difference = out_line.compare(exp_line);
                 *
                 * But here, I only want to know if they are the same.
                 * -> after automatic refactoring
                 */
                if (out_line != exp_line) {
                    write_colored_text("Error: Line " + to_string(line_nr) + ":\t");
                    write_colored_text("Expected: ", yellow);
                    cout << exp_line << "\t";
                    write_colored_text("Your output: ", cyan);
                    cout << out_line << endl;
                    error_count++;
                }
            } else {
                write_colored_text("You produced at least one line to much of output: ");
                cout << out_line << endl;
                error_count++;
            }
        }

        // Check, if 'expected' has more lines than 'out':
        while (getline(expected_file, exp_line)) {
            line_nr++;
            write_colored_text("You failed to produce expected line " + to_string(line_nr) + ":  ");
            cout << exp_line << endl;
            error_count++;
        }

        if (error_count > 0) {
            write_colored_text(
                    "You produced " + to_string(error_count) + " wrong line" + (error_count == 1 ? "" : "s") +
                    " of totally " + to_string(line_nr) + " lines of expected output!\n",
                    magenta);
        } else {
            write_colored_text("Yes, no errors, all " + to_string(line_nr) + " lines of output are correct!\n",
                               green);
        }
    }

    out_file.close();
    expected_file.close();

    // Subtract stop and start time-points and cast it to required unit. 
    // Predefined units are: nanoseconds, microseconds, milliseconds, seconds, minutes, hours.
    // Use duration_cast() function: 
    long int micro = chrono::duration_cast<chrono::microseconds>(stop - start).count();
    long int milli = micro / 1000;
    micro %= 1000;
    cout << "Execution time for actual code: ";
    if (milli) cout << milli << "'";
    cout << micro << " microseconds" << endl;
}


