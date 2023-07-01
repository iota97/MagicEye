#include "SceneCamera.h"
#include <cstdio>

static bool keys[1024];
static float lastX, lastY;
static bool firstMouse = true;
static Context *context;
static Camera *cam;

void apply_camera_movements() {
    GLboolean diagonal_movement = (keys[GLFW_KEY_W] ^ keys[GLFW_KEY_S]) && (keys[GLFW_KEY_A] ^ keys[GLFW_KEY_D]); 
    cam->SetMovementCompensation(diagonal_movement);
    
    if (keys[GLFW_KEY_W])
        cam->ProcessKeyboard(FORWARD, context->deltaTime);
    if (keys[GLFW_KEY_S])
        cam->ProcessKeyboard(BACKWARD, context->deltaTime);
    if (keys[GLFW_KEY_A])
        cam->ProcessKeyboard(LEFT, context->deltaTime);
    if (keys[GLFW_KEY_D])
        cam->ProcessKeyboard(RIGHT, context->deltaTime);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        context->showMenu = !context->showMenu;
        glfwSetInputMode(window, GLFW_CURSOR, context->showMenu ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }

    if (action == GLFW_PRESS) {
        keys[key] = true;
    } else if(action == GLFW_RELEASE) {
        keys[key] = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    if (!context->showMenu) {
        cam->ProcessMouseMovement(xoffset, yoffset);
    }
}

SceneCamera::SceneCamera(const glm::vec3 &position, GLFWwindow *w, Context *c) : camera(position, GL_FALSE), window(w), ctx(c) {
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    context = ctx;
    cam = &camera;
}

glm::mat4 SceneCamera::GetViewMatrix() {
    return camera.GetViewMatrix();
}

glm::mat4 SceneCamera::GetProjectionMatrix() {
    return glm::perspective(glm::radians(60.0f), (float)ctx->width/ctx->height, 0.5f, 500.0f);
}

void SceneCamera::SetPosition(const glm::vec3 &pos) {
    camera.Position = pos;
}

void SceneCamera::SetRotation(GLfloat yaw, GLfloat pitch) {
    camera.Yaw = yaw;
    camera.Pitch = pitch;
    camera.UpdateCameraVectors();
}

void SceneCamera::Process() {
    #if 0
    printf("Camera pos: (%f, %f, %f), rot: (%f, %f), front: (%f, %f, %f)\n",
        camera.Position.x, camera.Position.y, camera.Position.z, camera.Yaw, camera.Pitch,
        camera.Front.x, camera.Front.y, camera.Front.z);
    #endif
    if (context->shouldClose) {
        glfwSetWindowShouldClose(window, true);
        return;
    }
    glfwPollEvents();
    if (!context->showMenu) {
        apply_camera_movements();
    }
}