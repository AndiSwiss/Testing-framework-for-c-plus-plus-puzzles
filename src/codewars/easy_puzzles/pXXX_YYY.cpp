/*
 * Created by AndiSwiss on 28.09.2020
 */

/*
 * This file is intended to check my custom test Framework. It can also be used as a template for a new coding puzzle.
 * 
 *   
 * It should produce the following (colored) output:
 *
 *     pXXX_YYY, file-nr 1...  code: 77µs...  executing tests...
 *     Yes, no errors, all 3 lines of output are correct!
 *     
 *     pXXX_YYY, file-nr 2...  code: 59µs...  executing tests...
 *     Yes, no errors, all 2 lines of output are correct!
 *     
 *     -> Hurray, all 2 provided tests passed!  Total code execution: 136µs, total overall time: 14'313µs.
 *  
 * Try to change any of the in*.txt or exp*.txt files in src/codewars/easy_puzzles/in_out_exp_files/pXXX_YYY/..
 * and you should see some errors instead.
 */


// You can use this file as a template to create your own files.
// TODO: Change ALL occurrences of pXXX_YYY    (XXX is the problem-no, YYY is the problem-name)
// TODO: Change ALL occurrences of pXXX
// TODO: Put method signature to the header-file: void pXXX_YYY();
// TODO: Import header-file to ./src/main.cpp
// TODO: Call method pXXX_YYY(); from ./src/main.cpp
// TODO: For code-completion, make sure, that this file and its header file are listed in CMakeLists.txt


#include "../../my_libs/local_testing/string_in_out_testing_v2.h"


#include "pXXX_YYY.h"

void pXXX_main();

/**
 * This method is used for running the code in pXXX_main() on one or multiple sets of input and expected files: <br>
 * in1.txt, exp1.txt, in2.txt, exp2.txt in src/codewars/easy_puzzles/in_out_exp_files/pXXX_YYY/..
 */
void pXXX_YYY() {
    list<int> files_to_run = {1, 2};  // TODO: Specify which test-sets you want to run or leave empty to run all test-files found)
    const string base_path = "src/codewars/easy_puzzles";  // TODO: Change if necessary
    const string problem_no = "pXXX_YYY";
    string_in_out_testing_v2(base_path, problem_no, files_to_run, pXXX_main);
}


/**
 * ====================================================================================
 * Actual code from here on!
 * Code before is for IDE ONLY!
 * ====================================================================================
 */


// TODO: Make sure, you include here all libraries which you use, otherwise you might get compile error on the judge
//       (Since the following libraries are already imported by the test-framework-import):
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <chrono>
#include <cmath>
#include <list>
#include <dirent.h>
#include <regex>        // TODO: Needed for 'sort(..)', e.g., sorting an array; otherwise, you will have compile error in docker or on the online judge !!!


using namespace std;

// TODO: Before submitting, change method to  int main() {..}
//int main() {
void pXXX_main() {
    
    // TODO: write your code here:
    long long i, j;
    while (cin >> i >> j) {
        cout << i << " and " << j << endl;
    }
    
}


