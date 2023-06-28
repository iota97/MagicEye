#pragma once

#include <GLFW/glfw3.h>
#include "Context.h"
#include "utils/camera.h"

class SceneCamera {
public:
    SceneCamera(glm::vec3 position, GLFWwindow *w, Context *ctx);
    void Process();
    glm::mat4 GetViewMatrix();

private:
    Camera camera;
    GLFWwindow *window;
    Context *ctx;
};