#include "SkyboxPass.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

SkyboxPass::SkyboxPass() : shader("assets/shaders/skybox.vert", "assets/shaders/skybox.frag"), model("assets/models/skybox.obj") {}

SkyboxPass::~SkyboxPass() {
    shader.Delete();
}

void SkyboxPass::execute(Scene *scene, Context *ctx) {
    shader.Use();
    glDepthFunc(GL_LEQUAL);

    // Cubemap
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, scene->skybox);
    glUniform1i(glGetUniformLocation(shader.Program, "cubemap"), 0);
    glUniform1f(glGetUniformLocation(shader.Program, "colorResolution"), ctx->colorResolution);

    // Matrix
    glm::mat4 projection = scene->cam->GetProjectionMatrix();
    glm::mat4 view = scene->cam->GetViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
    view = glm::mat4(glm::mat3(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    
    // Draw
    model.Draw();
    glDepthFunc(GL_LESS);
}