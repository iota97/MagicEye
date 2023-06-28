CXX = c++
CXXFLAGS  = -g -O2 -x c++ -Wall -Wno-invalid-offsetof -std=c++11 -I./include
LDFLAGS = -L/usr/lib -lassimp -lglfw -lz -lminizip
SOURCES = main.cpp
TARGET = magiceye

build: glad.o main.o
	$(CXX) -o $(TARGET) glad.o main.o $(LDFLAGS)

.PHONY : clean
clean :
	-rm glad.o $(TARGET)

main.o:
	$(CXX) $(CXXFLAGS) -c main.cpp

glad.o:
	$(CXX) $(CXXFLAGS) -c ./include/glad/glad.c