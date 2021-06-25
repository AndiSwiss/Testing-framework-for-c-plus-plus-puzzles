# Old README.md - for Execution in non CLion-Environments with a Docker Image

Before migrating this full project to a fully runnable project inside [CLion by JetBrains](https://www.jetbrains.com/clion/), 
I had a slightly more complicated setup with docker. While it might be safer for execution,
CLion has problems with properly debugging such a project.


## Build (deprecated)

The programs are located in the `src` directory.

The basic pattern to compile a single program with [gcc](https://gcc.gnu.org/)
is as follows:

```
gcc src_file.c -o executable_file       # Compiles C-files
g++ src_file.cpp -o executable_file     # Compiles C++-files
```

This will compile `src_file.c` and generate an executable file called
`executable_file` in the same directory.  Execute it with `./executable_file`.

A `makefile` is present to compile **all** programs at once to `src/out`:

```
cd src
make
```

Single programs can be compiled using their names:

```
make time1
```
You can also do the following to compile and then immediately execute an
application:
```
make time1 && ./out/time1
```



---
### Docker
Depending on your operating system, you might miss some libraries that are
necessary for a few programs. E.g., OS X does not provide POSIX `mqueue.h`,
thus the message queue programs won't compile on OS X. Therefore, our
lecturer suggested to work on a Raspberry Pi using a linux distribution.

An alternative way is to compile and execute the programs in a docker container.
You can find a `Dockerfile` in this repository which uses an ubuntu image
and installs the following components:
- `gcc` compiler for C-applications
- `g++` compiler for C++-applications
- `make` build-tool for C/C++
- `libc` library
- `libncurses` library

Build the image using the provided shell script:
```
chmod u+x docker-build.sh
./docker-build.sh
```

Afterwards, start a container (this will directly connect you to the
container, so you can start hacking):
```
chmod u+x docker-start.sh
./docker-start.sh
```
All files/directories from your working directory will be synced with the
container, so you can edit on your computer and compile/execute in the
container. Run `exit` when connected to the container to terminate the session.

#### Multiple Docker-connections
If you want to have multiple terminals to the same docker, you can easily
do that like this: Open a new terminal window and type:
```
docker exec -it testing_cplusplus /bin/bash

# Or do once:
chmod u+x docker-second-connection.sh

# Then simply:
./docker-second-connection.sh
```


---
### Possible Errors concerning Docker
If you try to build the software with `make p027_xxx`, and you get the following error:
```
clang: error: linker command failed with exit code 1 (use -v to see invocation)
```
Then you simply forgot to change into the docker-environment!!

Same, if try to execute something with `./out/p027_xxx`, and you get the following error:
```
cannot execute binary file
```
-> Same Story!


## Code-Completion in CLion
If you want to run this project in CLion, then it is very useful for better
code-completion, if you do the following when creating or adding new
*.c - files:
- When creating inside CLion, then make sure to choose option "add to
  targets", so the file gets added to `CMakeLists.txt`.
- When importing a file, just manually add it to the list of
  "add_executable" inside `CMakeLists.txt`.


---

## Debugging with GDB
For easy debugging, `gdb` is installed in the docker image.

Additionally, the `-ggdb` has been added to the compile instructions in each `makefile` (enables line-numbering and code-reading line-by-line for debugging).

For easy entering the debugger, just compile and
boot up `gdb`:
```
make pXX && gdb /out pXX
```

### Useful Commands for gdb
Useful commands, mostly compiled from the great 8-min tutorial video [introduction to GDB a tutorial - Harvard CS50](https://www.youtube.com/watch?v=sCtY--xRUyI).

*Note*: Nearly all commands can be abbreviated, e.g., `b` for `break`.
```
# Run the program, with or without breakpoints set (run):
r

# Set breakpoint, e.g. for method 'main' (break):
b main

# Set a breakpoint in a specific line:
b 15

# In multi-file projects, the filename should also be specified
# when setting breakpoints, otherwise, gdb can get confused):
b pXX.cpp:15 

# Move to next line (next):
n

# Move to the next breakpoint (continue):
c

# Show lines of code around the current line (list):
l

# Show lines of code around a specific line number:
l 12

# Show lines of code between two line numbers:
l 12, 50

# Show value of all variables in scope (info locals):
i lo

# Show value of a variable, e.g., variable 'num' (print):
p num

# Each variable is then assigned a temporary variable name, e.g., $1.
# This can be called later with:
p $1

# Change a variable to another value, e.g., variable 'num':
p num = 10

# Repeat the previous command (e.g. 'n'): Just hit RETURN

# Recompile the file
make pXX

# Disable all breakpoints (disable):
d

# Disable a specific breakpoint: First list all breakpoints, get its number (info breakpoints), then disable it (disable):
i b
d 6

# Get help about a command (help):
h i b       # -> shows help for 'info breakpoints'
h l         # -> shows help for 'list'

# Abort the currently debugged application:
kill

# Quit the debugger (quit):
q
```

### Docs
As mentioned before, a great 8-min tutorial video can be found here: [introduction to GDB a tutorial - Harvard CS50](https://www.youtube.com/watch?v=sCtY--xRUyI).

Further documentation can be found on [GDB Documentation](https://www.gnu.org/software/gdb/documentation/).




