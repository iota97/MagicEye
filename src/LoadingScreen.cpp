#include "LoadingScreen.h"

LoadingScreen::LoadingScreen() : shader("assets/shaders/loading.vert", "assets/shaders/loading.frag") {
    GLfloat vertices[] = {
    1.0f,  1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f
    };
    GLuint indices[] = {
        0, 3, 1,
        1, 3, 2
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
        
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    texture = LoadTexture("assets/textures/loading.png");
    glClearColor(0.114f, 0.114f, 0.114f, 1.0f);
}

LoadingScreen::~LoadingScreen() {
    shader.Delete();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    glDeleteTextures(1, &texture);
}

void LoadingScreen::Draw(GLFWwindow *window, Context *ctx) {
    shader.Use();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    GLint textureLocation = glGetUniformLocation(shader.Program, "tex");
    glUniform1i(textureLocation, 0);
    glUniform1f(glGetUniformLocation(shader.Program, "rateo"), (float)ctx->height/ctx->width);

    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);

    glfwSwapBuffers(window);
}