#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "Context.h"
#include "StereoPass.h"
#include "IllumPass.h"
#include "SkyboxPass.h"
#include "GUI.h"
#include "Scene.h"

class Render {
public:
    Render(GLFWwindow *w, Context *c);
    ~Render();

    void Process(Scene *scene);

private:
    Context *ctx;
    GLFWwindow *window;
    GLfloat lastFrame = 0;

    int oldH = -1, oldW = -1;
    GLuint FBO, depthMap, colorMap;

    IllumPass illumPass;
    SkyboxPass skyboxPass;
    StereoPass stereoPass;
    GUI gui;
};