#include "glad/glad.h"
#include "StereoPass.h"
#include "utils/shader.h"
#include "utils/image.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

StereoPass::StereoPass(Context *c) : shader("assets/shaders/stereogram.vert", "assets/shaders/stereogram.frag"), ctx(c) {
    // Create fullscreen quad
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

    // Creates SSBO
    glGenBuffers(1, &colorSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, colorSSBO); 
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, colorSSBO);

    glGenBuffers(1, &uvSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, uvSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, uvSSBO);

    glGenBuffers(1, &edgeSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, edgeSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, edgeSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // Load pattern texture
    pattern.push_back(LoadTexture("assets/textures/patterns/pat0.png"));
    pattern.push_back(LoadTexture("assets/textures/patterns/pat1.png"));
    pattern.push_back(LoadTexture("assets/textures/patterns/pat2.png"));
    pattern.push_back(LoadTexture("assets/textures/patterns/pat3.png"));
    pattern.push_back(LoadTexture("assets/textures/patterns/pat4.png"));
    pattern.push_back(LoadTexture("assets/textures/patterns/pat5.png"));
}

StereoPass::~StereoPass() {
    // Shader
    shader.Delete();

    // Quad
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // SSBO
    glDeleteBuffers(1, &colorSSBO);
    glDeleteBuffers(1, &uvSSBO);
    glDeleteBuffers(1, &edgeSSBO);

    // Patterns
    for (auto &t : pattern) {
        glDeleteTextures(1, &t);
    }
}

void StereoPass::execute(GLuint colorMap, GLuint depthMap) {
    shader.Use();

    // Keep the SSBO matching the screen size
    if (oldH != ctx->height || oldW != ctx->width) {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, colorSSBO); 
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::ivec4) * ctx->width * ctx->height, nullptr, GL_DYNAMIC_COPY);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, uvSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec2) * ctx->width * ctx->height, nullptr, GL_DYNAMIC_COPY);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, edgeSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLint) * ctx->width * ctx->height, nullptr, GL_DYNAMIC_COPY);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        oldH = ctx->height;
        oldW = ctx->width;
    }

    // Pass uniform
    glUniform1i(glGetUniformLocation(shader.Program, "bufferWidth"), ctx->width);
    glUniform1i(glGetUniformLocation(shader.Program, "bufferHeight"), ctx->height);
    glUniform1f(glGetUniformLocation(shader.Program, "time"), ctx->staticPattern ? 1.0 : ctx->currentFrame);
    glUniform1f(glGetUniformLocation(shader.Program, "depthStrength"), ctx->depthStrength);
    glUniform1f(glGetUniformLocation(shader.Program, "eyeSep"), ctx->eyeSep);
    glUniform1f(glGetUniformLocation(shader.Program, "obsDistance"), ctx->obsDistance);
    glUniform1f(glGetUniformLocation(shader.Program, "sceneColorStr"), ctx->sceneColorStr);
    glUniform1f(glGetUniformLocation(shader.Program, "edgeStr"), ctx->edgeStr);
    glUniform1f(glGetUniformLocation(shader.Program, "edgeThreshold"), ctx->edgeThreshold);

    // Clear color and edge SSBO
    GLint zero = 0;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, colorSSBO); 
    glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R32I, GL_RED_INTEGER, GL_INT, &zero);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, edgeSSBO); 
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

    // Select random subroutine
    GLsizei n;
    glGetProgramStageiv(shader.Program, GL_FRAGMENT_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS, &n);
    GLint computePassLocation = glGetSubroutineUniformLocation(shader.Program, GL_FRAGMENT_SHADER, "computePass");
    GLint patternLocation = glGetSubroutineUniformLocation(shader.Program, GL_FRAGMENT_SHADER, "pattern");
    GLuint index[n];
    switch (ctx->pattern) {
        case 0:
            index[patternLocation] = glGetSubroutineIndex(shader.Program, GL_FRAGMENT_SHADER, "perlinNoise");
            break;
        case 1:
            index[patternLocation] = glGetSubroutineIndex(shader.Program, GL_FRAGMENT_SHADER, "perlinNoiseRGB");
            break;
        default:
            index[patternLocation] = glGetSubroutineIndex(shader.Program, GL_FRAGMENT_SHADER, "texturePattern");
            break;
    }
    
    // Bind pattern texture
    if (ctx->pattern > 1) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, pattern[ctx->pattern-2]);
        textureLocation = glGetUniformLocation(shader.Program, "randomTexture");
        glUniform1i(textureLocation, 2);
    }

    // First pass
    glDisable(GL_DEPTH_TEST);
    index[computePassLocation] = glGetSubroutineIndex(shader.Program, GL_FRAGMENT_SHADER, "firstPass");
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, n, index);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Second pass, barrier needed
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    index[computePassLocation] = glGetSubroutineIndex(shader.Program, GL_FRAGMENT_SHADER, "secondPass");
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, n, index);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Clean up
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
}