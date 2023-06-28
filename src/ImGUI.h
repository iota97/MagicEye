#pragma once
#include <GLFW/glfw3.h>
#include "Context.h"

class ImGUI {
public:
    ImGUI(GLFWwindow *w, Context *c);
    ~ImGUI();

    void RenderMenu();
private:
    Context *ctx;
};