# -------------------------------------- #
# Makefile for C++ and C Applications    #
#   									 #
# Good Tutorial for creating a makefile: #
#     https://makefiletutorial.com/      #
# -------------------------------------- #


# Change the relative path of the library files depending on the location of this makefile:
# Leave empty, if you don't have any files in there -> OTHERWISE there will be an error!
#LIB_PATH_CPP =
LIB_PATH_CPP = ../my_libs/**/*.cpp
#LIB_PATH_C =
LIB_PATH_C = ../my_libs/**/*.c


# Following would run all C++ apps, but not C apps:
# all: $(patsubst %.cpp, %, $(wildcard *.cpp))
# For both C and C++:
all: $(patsubst %.cpp, %, $(wildcard *.cpp)) $(patsubst %.c, %, $(wildcard *.c))


# Rules for all C++ apps:
%: %.cpp
	- @mkdir -p out
	- g++ $< $(LIB_PATH_CPP) -o out/$@ -pthread -lncurses -lrt -ggdb


# Rules for all C apps:
%: %.c
	- @mkdir -p out
	- gcc $< $(LIB_PATH_C) -o out/$@ -pthread -lncurses -lrt -lm -ggdb


.PHONY: clean
clean:
	- rm out/*


# ---------------------------------------------------------------------------- #
# Some explanations about this makefile (and C++ compiling):
#
# Global variable:
#     $<    holds the whole filename, such as:				 my_app.cpp
#     $@    holds the filename withouth extension, such as:	 my_app
#
# Execute Linux commands:
#     Just write them. They will be prompted out when executed.
#     when using 'echo', you want to prepend '@echo', because otherwise you see the command twice!
#
# To just compile a file (not produce a linked executable file):
#     - g++ -c rectangle.cpp
#     -> Will produce  rectangle.o
#     -> This can be used in the "g++ -o" (linking) -> then just provide 'rectangle.o' instead of 'rectangle.cpp'

# Explanation of 'patsubst':
# The following line would save a list of C++ and C files in the variable 'SRC', which can be later used with $(SRC):
#	SRC = $(wildcard *.cpp) $(wildcard *.c)
# -> @echo $(SRC)          ->   main.cpp hello_world.c
#
# The following line would just save the filenames without the extensions:
#   FILENAMES = $(patsubst %.cpp, %, $(wildcard *.cpp)) $(patsubst %.c, %, $(wildcard *.c))
# -> @echo $(FILENAMES)    ->   main hello_world


# Note: the following line:
#     - g++ $< -o out/$@ -pthread -lncurses -lrt
# Would produce the following line, when called with  make main:
#     g++ main.cpp -o out/main -pthread -lncurses -lrt

# If you want to include other files, just add them here:
#     E.g. if they are called helper1.cpp, helper2.cpp:
#     - g++ $< helper1.cpp helper2.cpp -o out/$@ -pthread -lncurses -lrt
# Produces:
#     g++ main.cpp helper1.cpp helper2.cpp -o out/main -pthread -lncurses -lrt

# You may also use wildcard, which will be dynamically expanded by your system (like the usual system commands)