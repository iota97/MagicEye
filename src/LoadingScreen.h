#pragma once

#include "glad/glad.h"
#include "utils/shader.h"
#include "utils/image.h"
#include "Context.h"
#include <GLFW/glfw3.h>

class LoadingScreen {
public:
    LoadingScreen();
    ~LoadingScreen();

    void Draw(GLFWwindow *window, Context *ctx);

private:
    Shader shader;
    GLuint VAO, VBO, EBO;
    GLuint texture;
};