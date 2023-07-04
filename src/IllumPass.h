#pragma once

#include "glad/glad.h"
#include "utils/shader.h"
#include "Context.h"
#include "Scene.h"
#include "utils/animator.h"
#include "utils/shader.h"
#include "utils/model.h"
#include "utils/model_animation.h"
#include "utils/image.h"

class IllumPass {
public:
    IllumPass(Context *c);
    ~IllumPass();

    void execute(Scene *scene);
    
private:
    void SetupScene(Shader &shader, Scene *scene);
    void SetupObject(Shader &shader, Scene *scene, Object &obj);

    Shader illumShader;
    Shader skinningShader;
    Context *ctx;
};