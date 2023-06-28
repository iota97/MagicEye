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

#include "src/StereoPass.h"
#include "src/IllumPass.h"
#include "src/ImGUI.h"
#include "src/Context.h"
#include "src/SceneCamera.h"

// dimensions of application's window
GLuint screenWidth = 1280, screenHeight = 720;

// the rendering steps used in the application
enum render_passes{ SHADOWMAP, RENDER };

void framebuffer_size_callback(GLFWwindow* window, int w, int h);

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
    //glfwSwapInterval(0);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    glfwGetFramebufferSize(window, &ctx.width, &ctx.height);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.26f, 0.46f, 0.98f, 1.0f);

    GLuint FBO, depthMap, colorMap;
    glGenFramebuffers(1, &FBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ctx.width, ctx.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

    glGenTextures(1, &colorMap);
    glBindTexture(GL_TEXTURE_2D, colorMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ctx.width, ctx.height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorMap, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto deltas = std::vector<float>(256);
    int deltaIndex = 0;

    float oldW = ctx.width;
    float oldH = ctx.height;

    IllumPass illumPass(&ctx);
    StereoPass stereoPass(&ctx);
    ImGUI imGUI(window, &ctx);
    SceneCamera sceneCamera(glm::vec3(0.0f, 0.0f, 7.0f), window, &ctx);

    GLfloat lastFrame = 0;
    // Rendering loop: this code is executed at each frame
    while (!glfwWindowShouldClose(window)) {
        
        ctx.currentFrame = glfwGetTime();
        ctx.deltaTime = ctx.currentFrame - lastFrame;
        lastFrame = ctx.currentFrame;

        deltas[deltaIndex = (deltaIndex+1) % 256] = ctx.deltaTime;
        float acc = 0.0;
        for (int i = 0; i < 256; i++) {
            acc += deltas[i];
        }
        ctx.FPS = 256/acc;

        if (oldH != ctx.height || oldW != ctx.width) {
            glBindTexture(GL_TEXTURE_2D, depthMap);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ctx.width, ctx.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glBindTexture(GL_TEXTURE_2D, colorMap);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ctx.width, ctx.height, 0, GL_RGBA, GL_FLOAT, NULL);
            glBindTexture(GL_TEXTURE_2D, 0);
            oldH = ctx.height;
            oldW = ctx.width;
        }

        sceneCamera.Process();
        glm::mat4 view = sceneCamera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(45.0f, (float)ctx.width/ctx.height, 0.1f, 50.0f);
        glm::vec3 lightDir0 = glm::vec3(1.0f, 1.0f, 1.0f);

        glViewport(0, 0, ctx.width, ctx.height);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        illumPass.execute(projection, view, lightDir0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        stereoPass.execute(colorMap, depthMap);

        if (ctx.showMenu) {
            imGUI.RenderMenu();
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
    ctx.width = w;
    ctx.height = h;
}