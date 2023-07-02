CXX = c++
CXXFLAGS  = -g -O2 -Wall -Wno-invalid-offsetof -std=c++17 -I./ext
LDFLAGS = -L/usr/lib -lassimp -lglfw -lz -lminizip -lGL
TARGET = magiceye
BUILDIR = build

IMGUI_HEADER = ./ext/imgui/imconfig.h ./ext/imgui/imgui_internal.h ./ext/imgui/imgui.h \
	./ext/imgui/imstb_rectpack.h ./ext/imgui/imstb_textedit.h ./ext/imgui/imstb_truetype.h \
	./ext/imgui/imgui_impl_opengl3.h ./ext/imgui/imgui_impl_glfw.h ./ext/imgui/imgui_impl_opengl3_loader.h

OBJ = $(BUILDIR)/main.o $(BUILDIR)/image.o $(BUILDIR)/glad.o $(BUILDIR)/imgui.o $(BUILDIR)/imgui_draw.o \
	$(BUILDIR)/imgui_tables.o $(BUILDIR)/imgui_widgets.o $(BUILDIR)/imgui_impl_opengl3.o $(BUILDIR)/imgui_impl_glfw.o \
	$(BUILDIR)/StereoPass.o $(BUILDIR)/GUI.o $(BUILDIR)/SceneCamera.o $(BUILDIR)/IllumPass.o $(BUILDIR)/Render.o\
	$(BUILDIR)/Scene.o $(BUILDIR)/SceneManager.o $(BUILDIR)/SkyboxPass.o $(BUILDIR)/LoadingScreen.o

magiceye: $(OBJ) 
	$(CXX) -o $(TARGET) $(OBJ) $(LDFLAGS)

.PHONY : clean
clean :
	-rm  -rf $(BUILDIR) $(TARGET)

$(BUILDIR)/main.o: ./src/main.cpp ./src/Context.h ./src/Scene.h ./src/Render.h
	$(CXX) $(CXXFLAGS) -c ./src/main.cpp -o $(BUILDIR)/main.o

$(BUILDIR)/SkyboxPass.o: ./src/SkyboxPass.cpp ./src/SkyboxPass.h
	$(CXX) $(CXXFLAGS) -c ./src/SkyboxPass.cpp -o $(BUILDIR)/SkyboxPass.o

$(BUILDIR)/LoadingScreen.o: ./src/LoadingScreen.cpp ./src/LoadingScreen.h
	$(CXX) $(CXXFLAGS) -c ./src/LoadingScreen.cpp -o $(BUILDIR)/LoadingScreen.o

$(BUILDIR)/SceneManager.o: ./src/SceneManager.cpp ./src/SceneManager.h ./src/Scene.h
	$(CXX) $(CXXFLAGS) -c ./src/SceneManager.cpp -o $(BUILDIR)/SceneManager.o

$(BUILDIR)/Scene.o: ./src/Scene.cpp ./src/Scene.h ./src/Object.h
	$(CXX) $(CXXFLAGS) -c ./src/Scene.cpp -o $(BUILDIR)/Scene.o

$(BUILDIR)/Render.o: ./src/Render.cpp ./src/Render.h ./src/Context.h
	$(CXX) $(CXXFLAGS) -c ./src/Render.cpp -o $(BUILDIR)/Render.o
	
$(BUILDIR)/IllumPass.o: ./src/IllumPass.cpp ./src/IllumPass.h ./src/Context.h ./src/Object.h
	$(CXX) $(CXXFLAGS) -c ./src/IllumPass.cpp -o $(BUILDIR)/IllumPass.o

$(BUILDIR)/SceneCamera.o: ./src/SceneCamera.cpp ./src/SceneCamera.h ./src/Context.h
	$(CXX) $(CXXFLAGS) -c ./src/SceneCamera.cpp -o $(BUILDIR)/SceneCamera.o

$(BUILDIR)/StereoPass.o: ./src/StereoPass.cpp ./src/StereoPass.h ./src/Context.h
	$(CXX) $(CXXFLAGS) -c ./src/StereoPass.cpp -o $(BUILDIR)/StereoPass.o

$(BUILDIR)/GUI.o: ./src/GUI.cpp ./src/GUI.h ./src/Context.h $(IMGUI_HEADER)
	$(CXX) $(CXXFLAGS) -c ./src/GUI.cpp -o $(BUILDIR)/GUI.o

$(BUILDIR)/image.o: ./ext/utils/image.h ./ext/utils/image.cpp ./ext/stb_image/stb_image.h ./ext/utils/image.h
	$(CXX) $(CXXFLAGS) -c ./ext/utils/image.cpp -o $(BUILDIR)/image.o

$(BUILDIR)/glad.o: ./ext/glad/glad.c ./ext/glad/glad.c ./ext/glad/glad.c ./ext/glad/khrplatform.h
	$(CXX) $(CXXFLAGS) -c ./ext/glad/glad.c -o $(BUILDIR)/glad.o

$(BUILDIR)/imgui.o: $(IMGUI_HEADER) ./ext/imgui/imgui.cpp
	$(CXX) $(CXXFLAGS) -c ./ext/imgui/imgui.cpp -o $(BUILDIR)/imgui.o

$(BUILDIR)/imgui_draw.o: $(IMGUI_HEADER) ./ext/imgui/imgui_draw.cpp 
	$(CXX) $(CXXFLAGS) -c ./ext/imgui/imgui_draw.cpp -o $(BUILDIR)/imgui_draw.o

$(BUILDIR)/imgui_tables.o: $(IMGUI_HEADER) ./ext/imgui/imgui_tables.cpp 
	$(CXX) $(CXXFLAGS) -c ./ext/imgui/imgui_tables.cpp -o $(BUILDIR)/imgui_tables.o

$(BUILDIR)/imgui_widgets.o: $(IMGUI_HEADER) ./ext/imgui/imgui_widgets.cpp
	$(CXX) $(CXXFLAGS) -c ./ext/imgui/imgui_widgets.cpp -o $(BUILDIR)/imgui_widgets.o

$(BUILDIR)/imgui_impl_glfw.o: $(IMGUI_HEADER) ./ext/imgui/imgui_impl_glfw.cpp
	$(CXX) $(CXXFLAGS) -c ./ext/imgui/imgui_impl_glfw.cpp -o $(BUILDIR)/imgui_impl_glfw.o

$(BUILDIR)/imgui_impl_opengl3.o: $(IMGUI_HEADER) ./ext/imgui/imgui_impl_opengl3.cpp
	$(CXX) $(CXXFLAGS) -c ./ext/imgui/imgui_impl_opengl3.cpp -o $(BUILDIR)/imgui_impl_opengl3.o

$(shell mkdir -p $(BUILDIR))