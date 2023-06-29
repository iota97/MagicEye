#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "src/Context.h"
#include "src/SceneManager.h"
#include "src/Render.h"

GLuint screenWidth = 1280, screenHeight = 720;
static Context ctx;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "MagicEye", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    // Scene manager first, will break ImGUI callbacks otherwise...
    SceneManager sceneManager(window, &ctx);
    Render render(window, &ctx);

    while (!glfwWindowShouldClose(window)) {
        sceneManager.Process();
        render.Process(sceneManager.GetScene());
    }

    glfwTerminate();
    return 0;
}