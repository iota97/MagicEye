#include "glad/glad.h"
#include "StereoPass.h"
#include "utils/shader.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

StereoPass::StereoPass() : shader(Shader("assets/shaders/stereogram.vert", "assets/shaders/stereogram.frag")) {
    // Create fullscreen quad
    GLfloat vertices[] = {
    1.0f,  1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f
    };
    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    GLuint VBO, EBO;
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

    // Creates SSBO
    glGenBuffers(1, &colorSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, colorSSBO); 
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, colorSSBO);

    glGenBuffers(1, &uvSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, uvSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, uvSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

StereoPass::~StereoPass() {
    shader.Delete();
}

void StereoPass::execute(GLuint colorMap, GLuint depthMap, int width, int height, float time, float depthStrength) {
    shader.Use();

    // Keep the SSBO matching the screen size
    if (oldH != height || oldW != width) {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, colorSSBO); 
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::ivec4) * width * height, nullptr, GL_DYNAMIC_COPY);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, uvSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec2) * width * height, nullptr, GL_DYNAMIC_COPY);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        oldH = height;
        oldW = width;
    }

    // Pass uniform
    glUniform1f(glGetUniformLocation(shader.Program, "time"), time);
    glUniform1f(glGetUniformLocation(shader.Program, "depthStrength"), depthStrength);
    glUniform1i(glGetUniformLocation(shader.Program, "bufferWidth"), width);
    glUniform1i(glGetUniformLocation(shader.Program, "bufferHeight"), height);

    // Clear color SSBO
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, colorSSBO); 
    GLint zero = 0;
    glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R32I, GL_RED_INTEGER, GL_INT, &zero);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // Textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    GLint textureLocation = glGetUniformLocation(shader.Program, "depthMap");
    glUniform1i(textureLocation, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, colorMap);
    textureLocation = glGetUniformLocation(shader.Program, "colorMap");
    glUniform1i(textureLocation, 1);

    // Bind quad
    glBindVertexArray(VAO);

    // First pass
    glDisable(GL_DEPTH_TEST);
    GLuint index = glGetSubroutineIndex(shader.Program, GL_FRAGMENT_SHADER, "firstPass");
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &index);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Second pass, barrier needed
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    index = glGetSubroutineIndex(shader.Program, GL_FRAGMENT_SHADER, "secondPass");
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &index);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Clean up
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
}