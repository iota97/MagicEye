CXX = c++
CXXFLAGS  = -g -O2 -x c++ -Wall -Wno-invalid-offsetof -std=c++11 -I./ext
LDFLAGS = -L/usr/lib -lassimp -lglfw -lz -lminizip -lGL
TARGET = magiceye

IMGUI_HEADER = ./ext/imgui/imconfig.h ./ext/imgui/imgui_internal.h ./ext/imgui/imgui.h \
	./ext/imgui/imstb_rectpack.h ./ext/imgui/imstb_textedit.h ./ext/imgui/imstb_truetype.h \
	./ext/imgui/imgui_impl_opengl3.h ./ext/imgui/imgui_impl_glfw.h ./ext/imgui/imgui_impl_opengl3_loader.h

OBJ = main.o image.o glad.o imgui.o imgui_draw.o imgui_tables.o imgui_widgets.o imgui_impl_opengl3.o imgui_impl_glfw.o \
	StereoPass.o ImGUI.o SceneCamera.o IllumPass.o

magiceye: $(OBJ) 
	$(CXX) -o $(TARGET) $(OBJ) $(LDFLAGS)

.PHONY : clean
clean :
	-rm  $(OBJ) $(TARGET) imgui.ini 

main.o: main.cpp ./src/Context.h
	$(CXX) $(CXXFLAGS) -c main.cpp

IllumPass.o:  ./src/IllumPass.cpp ./src/IllumPass.h ./src/Context.h
	$(CXX) $(CXXFLAGS) -c ./src/IllumPass.cpp

SceneCamera.o: ./src/SceneCamera.cpp ./src/SceneCamera.h ./src/Context.h
	$(CXX) $(CXXFLAGS) -c ./src/SceneCamera.cpp

StereoPass.o: ./src/StereoPass.cpp ./src/StereoPass.h ./src/Context.h
	$(CXX) $(CXXFLAGS) -c ./src/StereoPass.cpp

ImGUI.o: ./src/ImGUI.cpp ./src/ImGUI.h ./src/Context.h $(IMGUI_HEADER)
	$(CXX) $(CXXFLAGS) -c ./src/ImGUI.cpp

image.o: ./ext/utils/image.h ./ext/utils/image.cpp ./ext/stb_image/stb_image.h ./ext/utils/image.h
	$(CXX) $(CXXFLAGS) -c ./ext/utils/image.cpp

glad.o: ./ext/glad/glad.c ./ext/glad/glad.c ./ext/glad/glad.c ./ext/glad/khrplatform.h
	$(CXX) $(CXXFLAGS) -c ./ext/glad/glad.c

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