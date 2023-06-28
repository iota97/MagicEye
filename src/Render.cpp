#include "Render.h"

static Context *context;
void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
    context->width = w;
    context->height = h;
}

Render::Render(GLFWwindow *w, Context *c) : ctx(c), window(w), deltas(64), illumPass(ctx), stereoPass(ctx), imGUI(window, ctx) {
    context = ctx;
    // Size and resize support
    glfwGetFramebufferSize(window, &ctx->width, &ctx->height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Misc setting
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.26f, 0.46f, 0.98f, 1.0f);

    // Create FBO
    glGenFramebuffers(1, &FBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

    glGenTextures(1, &colorMap);
    glBindTexture(GL_TEXTURE_2D, colorMap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorMap, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Render::Process(SceneCamera *scene) {
    // Delta time calculation
    ctx->currentFrame = glfwGetTime();
    ctx->deltaTime = ctx->currentFrame - lastFrame;
    lastFrame = ctx->currentFrame;

    // Avg FPS
    deltas[deltaIndex = (deltaIndex+1) % 64] = ctx->deltaTime;
    float acc = 0.0;
    for (int i = 0; i < 64; i++) {
        acc += deltas[i];
    }
    ctx->FPS = 64/acc;

    // Resize if needed
    if (oldH != ctx->height || oldW != ctx->width) {
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ctx->width, ctx->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glBindTexture(GL_TEXTURE_2D, colorMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ctx->width, ctx->height, 0, GL_RGBA, GL_FLOAT, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
        oldH = ctx->height;
        oldW = ctx->width;
    }

    // Camera and scene setup
    glm::mat4 projection = glm::perspective(45.0f, (float)ctx->width/ctx->height, 0.1f, 50.0f);
    glm::mat4 view = scene->GetViewMatrix();
    glm::vec3 lightDir0 = glm::vec3(1.0f, 1.0f, 1.0f);

    // Illumination pass
    glViewport(0, 0, ctx->width, ctx->height);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    illumPass.execute(projection, view, lightDir0);

    // Stereogram pass
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    stereoPass.execute(colorMap, depthMap);

    // UI
    if (ctx->showMenu) {
        imGUI.RenderMenu();
    }

    // Swap buffer
    glfwSwapBuffers(window);
    glfwSwapInterval(ctx->vSync);
}