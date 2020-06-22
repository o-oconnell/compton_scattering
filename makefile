CC=g++
CFLAGS=-c -g -Wall -I./include/
OFLAGS=-g -Wall -o

all: computation user_interface main
	$(CC) $(OFLAGS) compton_program *.o `pkg-config --libs gtk+-3.0` 

computation: src/computation/ComptonEvent.cpp include/ComptonEvent.hpp
	$(CC) $(CFLAGS) src/computation/*.cpp

user_interface: src/user_interface/graphing.cpp src/user_interface/ComptonEventWindow.cpp
	$(CC) $(CFLAGS) `pkg-config --cflags gtk+-3.0` src/user_interface/ComptonEventWindow.cpp
	$(CC) $(CFLAGS) src/user_interface/graphing.cpp

main: src/main/main.cpp
	$(CC) $(CFLAGS) `pkg-config --cflags gtk+-3.0` src/main/main.cpp

clean:
	rm *.o compton_program
