#pragma once

#include "Context.h"
#include "utils/shader.h"
#include <GLFW/glfw3.h>
#include <vector>

class StereoPass {
public:
    StereoPass(Context *c);
    ~StereoPass();

    void execute(GLuint colorMap, GLuint depthMap);

private:
    Shader shader;
    GLuint VAO;
    GLuint colorSSBO, uvSSBO;
    std::vector<GLuint> pattern;
    int oldW = -1, oldH = -1;
    Context *ctx;
};