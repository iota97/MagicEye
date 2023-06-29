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
    glm::vec3 lightDir;
    std::vector<Object> objects;

protected:
    GLFWwindow *window;
    Context *ctx;
    std::vector<Model> models;
    std::vector<GLuint> textures;
};

class Scene1 : public Scene {
public:
    Scene1(SceneCamera *sc, GLFWwindow *w, Context *c);
};