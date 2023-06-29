#pragma once

#include <GLFW/glfw3.h>
#include "Context.h"
#include "utils/camera.h"

class SceneCamera {
public:
    SceneCamera(const glm::vec3 &position, GLFWwindow *w, Context *ctx);
    void Process();
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    void SetPosition(const glm::vec3 &pos);
    void SetRotation(GLfloat yaw, GLfloat pitch);

private:
    Camera camera;
    GLFWwindow *window;
    Context *ctx;
};