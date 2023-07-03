#include "Render.h"

static Context *context;
void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
    context->width = w;
    context->height = h;
}

Render::Render(GLFWwindow *w, Context *c) : ctx(c), window(w), illumPass(ctx), stereoPass(ctx), gui(window, ctx) {
    context = ctx;
    // Size and resize support
    glfwGetFramebufferSize(window, &ctx->width, &ctx->height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Create FBO
    glGenFramebuffers(1, &FBO);

    glGenTextures(1, &depth);
    glBindTexture(GL_TEXTURE_2D, depth);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

    glGenTextures(1, &color);
    glBindTexture(GL_TEXTURE_2D, color);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

    glGenTextures(1, &normal);
    glBindTexture(GL_TEXTURE_2D, normal);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normal, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Misc settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);  
}

Render::~Render() {
    glDeleteTextures(1, &color);
    glDeleteTextures(1, &normal);
    glDeleteTextures(1, &depth);
    glDeleteFramebuffers(1, &FBO);
}

void Render::Process(Scene *scene) {
    // Delta time calculation
    ctx->currentFrame = glfwGetTime();
    ctx->deltaTime = ctx->currentFrame - lastFrame;
    lastFrame = ctx->currentFrame;

    // Resize if needed
    if (oldH != ctx->height || oldW != ctx->width) {
        glBindTexture(GL_TEXTURE_2D, depth);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ctx->width, ctx->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glBindTexture(GL_TEXTURE_2D, color);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ctx->width, ctx->height, 0, GL_RGBA, GL_FLOAT, NULL);
        glBindTexture(GL_TEXTURE_2D, normal);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ctx->width, ctx->height, 0, GL_RGBA, GL_FLOAT, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
        oldH = ctx->height;
        oldW = ctx->width;
    }

    // Illumination pass
    glViewport(0, 0, ctx->width, ctx->height);
    glBindFramebuffer(GL_FRAMEBUFFER, ctx->stereoRendering ? FBO : 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, buffers);
    illumPass.execute(scene);

    // SkyboxPass
    skyboxPass.execute(scene, ctx);
    
    // Stereogram pass
    glDrawBuffers(1, buffers);
    if (ctx->stereoRendering) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        stereoPass.execute(color, depth, normal);
    }

    // UI
    if (ctx->showMenu) {
        gui.RenderMenu();
    }

    // Swap buffer
    glfwSwapBuffers(window);
    glfwSwapInterval(ctx->vSync);
}