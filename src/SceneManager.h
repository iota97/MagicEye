#pragma once

#include "Context.h"
#include "Scene.h"
#include "LoadingScreen.h"
#include <GLFW/glfw3.h>

class SceneManager {
public:
    SceneManager(GLFWwindow *w, Context *c);
    ~SceneManager();

    void Process();
    Scene *GetScene();

private:
    void SetScene(int id);

    int sceneId;
    Scene *scene = nullptr;
    GLFWwindow *window;
    Context *ctx;
    SceneCamera cam;
    LoadingScreen loadingScreen;
};