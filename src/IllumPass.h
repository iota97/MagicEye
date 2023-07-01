#pragma once

#include "glad/glad.h"
#include "utils/shader.h"
#include "Context.h"
#include "Scene.h"

class IllumPass {
public:
    IllumPass(Context *c);
    ~IllumPass();

    void execute(Scene *scene);
    
private:
    Shader shader;
    Context *ctx;
};