/*
 *  Created by AndiSwiss on 11.02.2021
 *  https://github.com/AndiSwiss
 */

#include "string_in_out_testing_v2.h"
// -> other includes -> see header-file!
#define COMMENT "<!--"

int execute_tests(const string &out_name, const string &exp_name);

void print_time(long micro);

void search_in_and_exp_files(const string &path, list<int> &files_to_run);

void add_file_if_regex_ok(const regex &r, list<int> &file_list, const string &file_name);

bool problem_with_file_to_read(ifstream &in_file, const string &in_name);

bool problem_with_file_to_write(ofstream &out_file, const string &out_name);

string create_file_with_removed_comments(const string &file_name);

void remove_a_file(const string &file_name);

/**
 * Provides a test-framework usable by IDE's to provide full debugging
 * 
 * - Replaces the cin (stdin) cout (stdout):
 *   - cin will read from in1.txt, cout will write to out1.txt
 * - It can handle multiple in/out files and processes them in a loop 
 *   (for testing code on various file-sets)
 * - All in/exp files may have comments:
 *          - Each comment line must begin with <!--
 *          - Each such line will be (temporarily) removed before execution
 * 
 * @param base_path         - String containing the path to your folder "in_out_exp_files" folder
 * @param problem_no        - String containing the path to your folder "in_out_exp_files" folder
 * @param files_to_run      - Array of int of the file-sets which you want to run
 * @param files_count       - Number of files to be executed (int)
 * @param code_to_execute   - Pass your actual code for testing as a code_to_execute parameter
 *                            For an example how to pass a code_to_execute to another code_to_execute in C++, see
 *                            https://www.codingame.com/playgrounds/15869/c-runnable-snippets/passing-a-code_to_execute-as-parameter
 */
void string_in_out_testing_v2(const string &base_path,
                              const string &problem_no,
                              list<int> &files_to_run,
                              void (*code_to_execute)()) {

    auto start_all = chrono::high_resolution_clock::now();

    string path = base_path + "/in_out_exp_files/" + problem_no + "/";

    // If no files were provided, then search the directory for in1.txt, in2.txt  and exp1.txt, exp2.txt and so on:
    if (files_to_run.empty()) search_in_and_exp_files(path, files_to_run);


    int file_count_failed = 0;
    int total_errors = 0;
    long int total_runtime = 0;

    for (int file_no : files_to_run) {
        // Note: Simple string concatenation would work:
//        path = path + problem_no + "/";
        // But since this concatenation is now inside a loop, CLion suggests:
        //     Clang-Tidy: String concatenation results in_file allocation of unnecessary temporary strings; consider 
        //     using 'operator+=' or 'string::append()' instead
        // See also https://www.w3schools.com/cpp/cpp_strings_concat.asp for a short explanation
        // NOTE: Strings are automatically initialized to "".
        string f_no, ending, in_name, out_name, exp_name;
        f_no = to_string(file_no);
        ending.append(f_no).append(".txt");
        in_name.append(path).append("in").append(ending);
        out_name.append(path).append("out").append(ending);
        exp_name.append(path).append("exp").append(ending);

        write_colored_text("\n" + problem_no + ", file-nr " + to_string(file_no) + "...  ", blue);

        // Creating file with removed comments in in_file:
        string in_no_comment_name = create_file_with_removed_comments(in_name);
        if (in_no_comment_name.empty()) return;

        // Open the files:
        ifstream in_no_comment_file(in_no_comment_name);
        if (problem_with_file_to_read(in_no_comment_file, in_no_comment_name)) return;
        ofstream out_file(out_name);
        if (problem_with_file_to_write(out_file, out_name)) return;


        // Switching cin (stdin) to read from file in_name and switching cout (stdout) to write to file out_name:
        streambuf *cin_buf = cin.rdbuf(in_no_comment_file.rdbuf()); // Save old buf and redirect cin to in_file.txt
        streambuf *cout_buf = cout.rdbuf(out_file.rdbuf()); // Save old buf and redirect cout to out_file.txt

        // Start the code timer:
        auto start = chrono::high_resolution_clock::now();

        // Execute passed in_file code (now using the changed cin and cout):
        code_to_execute();

        // Stop the timer:
        auto stop = chrono::high_resolution_clock::now();

        // Recreate the standard output to the terminal:
        cin.rdbuf(cin_buf);   // Reset to standard input again
        cout.rdbuf(cout_buf); // Reset to standard output again

        // Subtract stop and start time-points and cast it to required unit with duration_cast() function: 
        // Predefined units are: nanoseconds, microseconds, milliseconds, seconds, minutes, hours.
        long int micro = chrono::duration_cast<chrono::microseconds>(stop - start).count();
        cout << "code: ";
        print_time(micro);
        cout << "...  ";
        total_runtime += micro;
        // Close out_file (was open for write access - but in the tests, its needed for read-access)
        out_file.close();

        // Remove the file without comments
        in_no_comment_file.close();
        remove_a_file(in_no_comment_name);


        // Execute the tests:
        int error_count = execute_tests(out_name, exp_name);
        if (error_count == -9999) return; // abort if error in execute_tests(..) occurred
        else if (error_count > 0) {
            file_count_failed++;
            total_errors += error_count;
        }
    }

    string files_count = to_string(files_to_run.size());

    if (file_count_failed == 0) {
        write_colored_text("\n-> Hurray, all " + files_count + " provided tests passed!", cyan);
    } else {
        write_colored_text(
                "\n-> " + to_string(file_count_failed) + " of " + files_count +
                " provided tests failed with a total of " + to_string(total_errors) + " wrong lines.", red);
    }
    cout << "  Total code execution: ";
    print_time(total_runtime);
    cout << ", total overall time: ";
    auto stop_all = chrono::high_resolution_clock::now();
    long int micro_all = chrono::duration_cast<chrono::microseconds>(stop_all - start_all).count();
    print_time(micro_all);
    cout << "." << endl;

}


/**
 * Compares p**_expected*.txt (expected) with p**_output*.txt (actual)
 *
 * @param out_name 
 * @param exp_name 
 * @return success (-9999 on fail, 0 on success, higher number: numbers of errors) 
 */
int execute_tests(const string &out_name, const string &exp_name) {
    // Open 'out_name' and 'exp_name' for 'r'-access:
    // see https://www.tutorialspoint.com/cplusplus/cpp_files_streams.htm
    // ifstream ... : represents input file stream (for reading from files)
    // ofstream ... : represents output file stream (for creating and writing to files)
    // For comparing, you only need reading:


    // Creating file with removed comments in exp_file:
    string exp_no_comment_name = create_file_with_removed_comments(exp_name);
    if (exp_no_comment_name.empty()) return -9999;


    // Open files for read-access
    ifstream out_file(out_name);
    ifstream exp_no_comment_file(exp_no_comment_name);
    if (problem_with_file_to_read(out_file, out_name)) return -9999;
    if (problem_with_file_to_read(exp_no_comment_file, exp_no_comment_name)) return -9999;


    // Run actual tests: do string-comparison line by line
    write_colored_text("executing tests...\n", blue);
    int line_nr = 0, error_count = 0;
    string out_line, exp_line;

    while (getline(out_file, out_line)) {
        if (getline(exp_no_comment_file, exp_line)) {
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
    while (getline(exp_no_comment_file, exp_line)) {
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

    // Remove the file without comments
    exp_no_comment_file.close();
    remove_a_file(exp_no_comment_name);

    out_file.close();
    exp_no_comment_file.close();

    return error_count;
}

/**
 * Searches for in1.txt, in2.txt, ... and exp1.txt, exp2.txt, ...
 * Only adds to 'files_to_run', if BOTH were found: in1.txt AND exp1.txt
 */
void search_in_and_exp_files(const string &path, list<int> &files_to_run) {
    struct dirent *entry;

    list<int> in_files;
    list<int> exp_files;

    // Convert string to char-array
    int length = path.size();
    char char_path[length + 1];
    strcpy(char_path, path.c_str());

    // Open the directory
    DIR *dir = opendir(char_path);
    if (dir == nullptr) return;

    // Read each filename from the directory
    while ((entry = readdir(dir)) != nullptr) {
        string file_name = entry->d_name;

        /*
         * I just want to search for in1.txt, in2.txt, ..., in15.txt, in16.txt, ...
         * And I just want to extract the number 1, 2, ..., 15, 16, ..
         * See https://regex101.com for nice interactive explaining of the regex.
         * - ^      -> must start with what follows
         * - ^in    -> must start with 'in'
         * - (...)  -> first capture group -> get with $1
         * - [0-9]+ -> at least one digit
         * - \\.    -> a dot has to be present (note: in many languages, just one \ is required, also on https://regex101.com/)
         * - txt$   -> must end with txt
         */
        add_file_if_regex_ok(regex("^in([0-9]+)\\.txt$"), in_files, file_name);
        add_file_if_regex_ok(regex("^exp([0-9]+)\\.txt$"), exp_files, file_name);
    }
    closedir(dir);

    // Sort the file-lists
    in_files.sort();
    exp_files.sort();

    // Only add files to 'files_to_run', if the inXX.txt AND the expXX.txt are present:
    for (int in : in_files) {
        string i = to_string(in);
        if (exp_files.front() == in) {
            files_to_run.push_back(in);
            exp_files.pop_front();
        } else if (exp_files.empty() || exp_files.front() > in) {
            write_colored_text("in" + i += ".txt is present, but exp" + i + ".txt missing -> Skipping test!\n");
        } else {
            while (exp_files.front() < in) {
                write_colored_text("exp" + i += ".txt is present, but in" + i + ".txt missing -> Skipping test!\n");
                exp_files.pop_front();
            }
        }
    }
    // Possibly remaining exp-files (without in-files):
    for (int exp: exp_files) {
        string i = to_string(exp);
        write_colored_text("exp" + i += ".txt is present, but in" + i + ".txt missing -> Skipping test!\n");
    }
}


/**
 * Helper method for compiling a list of files based on the passed regex
 * @param r 
 * @param file_list 
 * @param file_name 
 */
void add_file_if_regex_ok(const regex &r, list<int> &file_list, const string &file_name) {
    string in_file = regex_replace(file_name, r, "$1");

    // NOTE: If the regex did not find anything, the original string is passed on to in_file
    //       -> With stoi, this should be handled with the exception catching
    try {
        int in_no = stoi(in_file);
        file_list.push_back(in_no);
    }
    catch (exception &e) {
        // Do nothing
    }
}


/**
 * Prints time, e.g. if micro would be 123423553, then it would be printed 123'423'553
 * @param micro 
 */
void print_time(long micro) {
    long n = (long) log10(micro);
    long p = pow(10, n);
    for (int i = 0; i < n; ++i) {
        cout << micro / p;
        micro %= p;
        p /= 10;
        if ((n - i) % 3 == 0) cout << "'";
    }
    cout << micro << "\u00B5s";
}

bool problem_with_file_to_read(ifstream &in_file, const string &in_name) {
    if (!in_file.is_open()) {
        write_colored_text("\n!! Problems reading from in_file '" + in_name + "'. Aborting!\n");
        return true;  // NOTE: closing of already opened files is not necessary, as they are closed when ifstream/ofstream are destroyed
    }
    return false;
}

bool problem_with_file_to_write(ofstream &out_file, const string &out_name) {
    if (!out_file.is_open()) {
        write_colored_text("\n!! Problems writing to out_file '" + out_name + "'. Aborting!\n");
        return true;  // NOTE: closing of already opened files is not necessary, as they are closed when ifstream/ofstream are destroyed
    }
    return false;
}

/**
 * Creates a file with removed comments!
 * @param file_name 
 * @return Returns the filename of the file with the removed comments. Returns an empty string on failure
 */
string create_file_with_removed_comments(const string &file_name) {
    // Open the files:
    ifstream file(file_name);
    if (problem_with_file_to_read(file, file_name)) return "";
    string no_comment_name = file_name.substr(0, file_name.length() - 4).append("_no_comment").append(".txt");
    ofstream no_comment_file_write(no_comment_name);
    if (problem_with_file_to_write(no_comment_file_write, no_comment_name)) return "";
    // Copy content, but only NON-comment lines:
    string line;
    while (getline(file, line)) {
        if (line.substr(0, 4) != COMMENT) no_comment_file_write << line << endl;
    }
    file.close();
    no_comment_file_write.close();

    return no_comment_name;
}

void remove_a_file(const string &file_name) {
    // See also https://www.geeksforgeeks.org/c-program-delete-file/
    if (remove(file_name.c_str()) != 0) {
        write_colored_text("\n!! Problems deleting file_name '" + file_name + "'. Aborting!\n");
        return;
    }
}
