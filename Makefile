CXX = c++
CXXFLAGS  = -g -O2 -x c++ -Wall -Wno-invalid-offsetof -std=c++11 -I./ext
LDFLAGS = -L/usr/lib -lassimp -lglfw -lz -lminizip -lGL
SOURCES = main.cpp
TARGET = magiceye

magiceye: image.o glad.o imgui_impl_opengl3.o imgui_impl_glfw.o imgui.o imgui.o imgui_draw.o imgui_tables.o imgui_widgets.o main.o  
	$(CXX) -o $(TARGET) image.o imgui_impl_opengl3.o imgui_impl_glfw.o glad.o main.o imgui.o imgui_draw.o imgui_tables.o imgui_widgets.o $(LDFLAGS)

.PHONY : clean
clean :
	-rm main.o image.o glad.o imgui.o imgui_draw.o imgui_tables.o imgui_widgets.o imgui.ini imgui_impl_opengl3.o imgui_impl_glfw.o $(TARGET)

main.o: main.cpp ./ext/stb_image/stb_image.h ./ext/utils/image.h
	$(CXX) $(CXXFLAGS) -c main.cpp

image.o: ./ext/utils/image.h ./ext/utils/image.cpp
	$(CXX) $(CXXFLAGS) -c ./ext/utils/image.cpp

glad.o: ./ext/glad/glad.c ./ext/glad/glad.c ./ext/glad/glad.c ./ext/glad/khrplatform.h
	$(CXX) $(CXXFLAGS) -c ./ext/glad/glad.c

IMGUI_HEADER = ./ext/imgui/imconfig.h ./ext/imgui/imgui_internal.h ./ext/imgui/imgui.h \
	./ext/imgui/imstb_rectpack.h ./ext/imgui/imstb_textedit.h ./ext/imgui/imstb_truetype.h \
	./ext/imgui/imgui_impl_opengl3.h ./ext/imgui/imgui_impl_glfw.h ./ext/imgui/imgui_impl_opengl3_loader.h

imgui.o: $(IMGUI_HEADER) ./ext/imgui/imgui.cpp
	$(CXX) $(CXXFLAGS) -c ./ext/imgui/imgui.cpp

imgui_draw.o: $(IMGUI_HEADER) ./ext/imgui/imgui_draw.cpp 
	$(CXX) $(CXXFLAGS) -c ./ext/imgui/imgui_draw.cpp 

imgui_tables.o: $(IMGUI_HEADER) ./ext/imgui/imgui_tables.cpp 
	$(CXX) $(CXXFLAGS) -c ./ext/imgui/imgui_tables.cpp 

imgui_widgets.o: $(IMGUI_HEADER) ./ext/imgui/imgui_widgets.cpp
	$(CXX) $(CXXFLAGS) -c ./ext/imgui/imgui_widgets.cpp

imgui_impl_glfw.o: $(IMGUI_HEADER) ./ext/imgui/imgui_impl_glfw.cpp
	$(CXX) $(CXXFLAGS) -c ./ext/imgui/imgui_impl_glfw.cpp

imgui_impl_opengl3.o: $(IMGUI_HEADER) ./ext/imgui/imgui_impl_opengl3.cpp
	$(CXX) $(CXXFLAGS) -c ./ext/imgui/imgui_impl_opengl3.cpp