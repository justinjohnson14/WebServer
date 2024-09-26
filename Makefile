# Thomas Daley
# September 13, 2021

# A generic build template for C/C++ programs

# executable name
EXE = WebServer

# C compiler
CC = gcc
# C++ compiler
CXX = g++
# linker
LD = g++

# C flags
CFLAGS = -Wall -I./
# C++ flags
CXXFLAGS = 
# C/C++ flags
CPPFLAGS = -Wall
# dependency-generation flags
DEPFLAGS = -MMD -MP
#Includes
INCLUDES = $(wildcard ./*.h)

SOURCES := $(wildcard ./*.c)

.DEFAULT_GOAL = all

.PHONY: all
all: WebServer

WebServer: $(SOURCES)
	gcc $(CFLAGS) -o $@ $^

# force rebuild
.PHONY: remake
remake:	clean WebServer

# execute the program
.PHONY: run
run: WebServer
	sudo ./WebServer

# remove previous build and objects
.PHONY: clean
clean:
	rm -fv ./WebServer

-include $(DEPENDS)