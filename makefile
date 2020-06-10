CC=g++
CFLAGS=-c -g -Wall -I./include/
OFLAGS=-g -Wall -o

all: computation user_interface
	$(CC) $(OFLAGS) compton_program *.o

computation: src/computation/ComptonEvent.cpp include/ComptonEvent.hpp
	$(CC) $(CFLAGS) src/computation/*.cpp

user_interface: src/user_interface/graphing.cpp src/user_interface/input.cpp
	$(CC) $(CFLAGS) src/user_interface/*.cpp

clean:
	rm *.o compton_program
