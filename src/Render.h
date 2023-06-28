#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <vector>
#include "Context.h"
#include "SceneCamera.h"
#include "StereoPass.h"
#include "IllumPass.h"
#include "ImGUI.h"

class Render {
public:
    Render(GLFWwindow *w, Context *c);
    void Process(SceneCamera *scene);

private:
    Context *ctx;
    GLFWwindow *window;

    int oldH = -1, oldW = -1;
    GLuint FBO, depthMap, colorMap;

    GLfloat lastFrame = 0;
    std::vector<float> deltas;
    int deltaIndex = 0;

    IllumPass illumPass;
    StereoPass stereoPass;
    ImGUI imGUI;
};