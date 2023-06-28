#include "glad/glad.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils/shader.h"
#include "utils/model.h"
#include "utils/camera.h"
#include "utils/image.h"

#include "src/Context.h"
#include "src/SceneCamera.h"
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

    // Scene first, will break ImGUI callbacks otherwise
    SceneCamera sceneCamera(glm::vec3(0.0f, 0.0f, 7.0f), window, &ctx);
    Render render(window, &ctx);

    while (!glfwWindowShouldClose(window)) {
        sceneCamera.Process();
        render.Process(&sceneCamera);
    }

    glfwTerminate();
    return 0;
}