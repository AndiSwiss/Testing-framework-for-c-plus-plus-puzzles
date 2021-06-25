# Testing Framework for C++ Puzzles

I have developed this testing framework for fast offline development of code puzzles to be solved in C++.
This is compatible with all different coding puzzle platforms which work with string input and string output, such as
https://www.codewars.com, https://www.codingame.com, https://onlinejudge.org and many more.

There is an example / template in `src/codewars/easy_puzzles/pXXX_YYY.cpp`. All the TODO's there should indicate the
places where you should do changes if you copy this template file for your own puzzles.


---
## Building / Running / Debugging in CLion
If you use [CLion by JetBrains](https://www.jetbrains.com/clion/), then you can simply do the following to get up and running:

- Simply choose the sub-application in [src/main.cpp](src/main.cpp) and run or debug.
- If you want to create a new problem, just select:
  - New > C/C++ Source File
  - Name the file
  - Make sure that the following two options are ticked:
    - [x] Create an associated header
    - [x] Add to targets (add's the )
  - With that, the associated header file is created and the targets are correctly added to [CMakeLists.txt](CMakeLists.txt)
- Make sure, that the entry-method is exposed in its header file
- Include this entry-method in [src/main.cpp](src/main.cpp) with `#include...` and call the method


---
## General Tips for CLion
The following commands are for macOS:
- 'cmd' + 'o': search and navigate to another class
- 'cmd' + 'b': Navigate to a definition
- 'alt' + '⇧': Extend selection


---
## Debugging in CLion
See overview, tips and video on https://www.jetbrains.com/clion/features/run-and-debug.html


---
## Fast Refactoring in CLion
- Place cursor at something (e.g., a function), then 'ctrl' + 't' (on macOS) -> brings up the "Refactor This" menu, e.g. for:
  - Renaming (in all usages)
  - for signature changes (incl. default-values) for a function (and changing order of arguments)
    - 'alt' 'F7': shows all the usages of a function
  - Extract variables/parameters/typedefs/methods/superclass
  - Pull/push members up/down (to base-class/super-class)
  - Move stuff to other classes / files (including their header-definitions and includes and everything!)
  - Copy stuff
  - Safe delete
- Inside a class or struct, press 'cmd' + 'n' for the "Generate Menu" (for constructors, getters/setters, equality and relational operators and so on)
- Use "Live Templates with 'cmd' + 'j'
- Surround blocks of code with "Surround With": 'cmd' + 'alt' + 't'
- See more Tips at https://www.jetbrains.com/clion/features/code-generation-and-refactorings.html




---
## Seldom Problem on macOS (and Solution)
Occasionally when updating macOS, suddenly, nothing was compiling anymore. Even a few of the imports were marked red 
(but by far not all) such as `#include <stdio.h>`.
When making changes to CMakeList.txt and hitting the button "Reload changes", I noticed that it complained:
```
Ignoring CMAKE_OSX_SYSROOT value:
/Application/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk

because the directory does not exist.
```

-> It did not show anymore my current macOS version

The solution was to remove the folder "cmake-build-debug", then make a small change in "CMakeList.txt" (like excluding one file saving, and click "Reload changes"). That rebuilt the folder "cmake-build-debug" and all problems were gone.

---
## Older README.md for Use with Docker Images
-> See file [README_docker_deprecated.md](README_docker_deprecated.md)