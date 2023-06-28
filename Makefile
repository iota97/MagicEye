CXX = c++
CXXFLAGS  = -g -O2 -x c++ -Wall -Wno-invalid-offsetof -std=c++11 -I./include
LDFLAGS = -L/usr/lib -lassimp -lglfw -lz -lminizip
SOURCES = main.cpp
TARGET = magiceye

magiceye: glad.o main.o
	$(CXX) -o $(TARGET) glad.o main.o $(LDFLAGS)

.PHONY : clean
clean :
	-rm glad.o $(TARGET)

main.o: main.cpp ./include/stb_image/stb_image.h
	$(CXX) $(CXXFLAGS) -c main.cpp

glad.o: ./include/glad/glad.c ./include/glad/glad.c ./include/glad/glad.c ./include/glad/khrplatform.h
	$(CXX) $(CXXFLAGS) -c ./include/glad/glad.c