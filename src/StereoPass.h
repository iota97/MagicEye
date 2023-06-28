#pragma once

#include "StereoPass.h"
#include "utils/shader.h"
#include <GLFW/glfw3.h>

class StereoPass {
public:
    StereoPass();
    ~StereoPass();

    void execute(GLuint colorMap, GLuint depthMap, int width, int height, float time, float depthStrength);

private:
    Shader shader;
    GLuint VAO;
    GLuint colorSSBO, uvSSBO;
    int oldW = -1, oldH = -1;
};