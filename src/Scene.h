#pragma once

#include <vector>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "SceneCamera.h"
#include "Object.h"
#include "utils/model.h"
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

protected:
    void SetSkybox(const char* path);
    GLFWwindow *window;
    Context *ctx;
    std::vector<Model> models;
    std::vector<GLuint> textures;
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