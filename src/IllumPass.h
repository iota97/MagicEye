#pragma once

#include "glad/glad.h"
#include <glm/glm.hpp>
#include "utils/shader.h"
#include "utils/model.h"
#include "Context.h"

class IllumPass {
public:
    IllumPass(Context *c);
    ~IllumPass();

    void execute(const glm::mat4 &projection, const glm::mat4 &view, const glm::vec3 &lightDir);
    
private:
    Shader shader;
    Context *ctx;

    Model cubeModel;
    Model sphereModel;
    Model bunnyModel;
    Model planeModel;
    GLint tex1, tex2;

    glm::mat4 sphereModelMatrix = glm::mat4(1.0f);
    glm::mat3 sphereNormalMatrix = glm::mat3(1.0f);
    glm::mat4 cubeModelMatrix = glm::mat4(1.0f);
    glm::mat3 cubeNormalMatrix = glm::mat3(1.0f);
    glm::mat4 bunnyModelMatrix = glm::mat4(1.0f);
    glm::mat3 bunnyNormalMatrix = glm::mat3(1.0f);
    glm::mat4 planeModelMatrix = glm::mat4(1.0f);
    glm::mat3 planeNormalMatrix = glm::mat3(1.0f);

    void RenderObjects(const glm::mat4 &view);
};