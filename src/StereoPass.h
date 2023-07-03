#pragma once

#include "Context.h"
#include "utils/shader.h"
#include <GLFW/glfw3.h>
#include <vector>

class StereoPass {
public:
    StereoPass(Context *c);
    ~StereoPass();

    void execute(GLuint colorTex, GLuint depthTex, GLuint normalTex);

private:
    Shader shader;
    GLuint VAO, VBO, EBO;
    GLuint colorSSBO, uvSSBO, edgeSSBO;
    std::vector<GLuint> pattern;
    int oldW = -1, oldH = -1;
    Context *ctx;
};