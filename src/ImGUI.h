#pragma once
#include <GLFW/glfw3.h>
#include "Context.h"

class ImGUI {
public:
    ImGUI(GLFWwindow *window, Context *c);
    ~ImGUI();

    void RenderMenu();
private:
    Context *ctx;
};