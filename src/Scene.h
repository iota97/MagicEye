#pragma once

#include <vector>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "SceneCamera.h"
#include "Object.h"
#include "utils/model.h"
#include "utils/model_animation.h"
#include "utils/animator.h"
#include "utils/animation.h"

#include "Context.h"

class Scene {
public:
    Scene(SceneCamera *sc, GLFWwindow *w, Context *c);
    virtual ~Scene();
    virtual void Process();

    SceneCamera *cam;
    glm::vec3 lightDir = glm::vec3(1.0);
    GLfloat ambientFactor = 0.1;
    GLuint skybox;

    std::vector<Object> objects;
    std::vector<SkinnedObject> skinned_objects;

protected:
    void SetSkybox(const char* path);
    GLFWwindow *window;
    Context *ctx;
    std::vector<Model> models;
    std::vector<ModelAnimated> skinned_models;
    std::vector<GLuint> textures;
    std::vector<Animator> animators;
    std::vector<Animation> animations;
};

class SceneStandard : public Scene {
public:
    SceneStandard(SceneCamera *sc, GLFWwindow *w, Context *c);
};

class SceneFairyHouse : public Scene {
public:
    SceneFairyHouse(SceneCamera *sc, GLFWwindow *w, Context *c);
};

class SceneChurch : public Scene {
public:
    SceneChurch(SceneCamera *sc, GLFWwindow *w, Context *c);
};

class SceneFight : public Scene {
public:
    SceneFight(SceneCamera *sc, GLFWwindow *w, Context *c);
};

class SceneWindmill : public Scene {
public:
    SceneWindmill(SceneCamera *sc, GLFWwindow *w, Context *c);
    void Process() override;
};

class SceneSpace : public Scene {
public:
    SceneSpace(SceneCamera *sc, GLFWwindow *w, Context *c);
};

class SceneDance : public Scene {
public:
    SceneDance(SceneCamera *sc, GLFWwindow *w, Context *c);
};

class SceneAnimal : public Scene {
public:
    SceneAnimal(SceneCamera *sc, GLFWwindow *w, Context *c);
};