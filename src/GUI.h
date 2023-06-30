#pragma once
#include <GLFW/glfw3.h>
#include "Context.h"

class GUI {
public:
    GUI(GLFWwindow *w, Context *c);
    ~GUI();

    void RenderMenu();
private:
    Context *ctx;
};