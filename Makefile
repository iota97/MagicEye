CXX = c++
CXXFLAGS  = -g -O2 -x c++ -Wall -Wno-invalid-offsetof -std=c++11 -I./include
LDFLAGS = -L/usr/lib -lassimp   -lglfw -lz -lminizip
SOURCES = ./include/glad/glad.c main.cpp
TARGET = magiceye

.PHONY : all
all:
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SOURCES) -o $(TARGET)

.PHONY : clean
clean :
	-rm $(TARGET)