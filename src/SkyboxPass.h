#pragma once

#include "Scene.h"
#include "glad/glad.h"
#include "utils/shader.h"
#include "Context.h"

class SkyboxPass {
public:
    SkyboxPass();
    ~SkyboxPass();

    void execute(Scene *scene, Context *ctx);

private:
    Shader shader;
    Model model;
};