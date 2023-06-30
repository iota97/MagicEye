#include "IllumPass.h"
#include "Object.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

IllumPass::IllumPass(Context *c) : shader("assets/shaders/illumin.vert", "assets/shaders/illumin.frag"), ctx(c) {}

IllumPass::~IllumPass() {
    shader.Delete();
}

void IllumPass::execute(Scene *scene) {
    shader.Use();

    // Camera setup
    glm::mat4 projection = scene->cam->GetProjectionMatrix();
    glm::mat4 view = scene->cam->GetViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniform3fv(glGetUniformLocation(shader.Program, "lightVector"), 1, glm::value_ptr(scene->lightDir));
    glUniform1f(glGetUniformLocation(shader.Program, "Ka"), scene->ambientFactor);

    // Object uniform location
    GLint textureLocation = glGetUniformLocation(shader.Program, "tex");
    GLint repeatLocation = glGetUniformLocation(shader.Program, "repeat");
    GLint specularColorLocation = glGetUniformLocation(shader.Program, "specularColor");
    GLint shininessLocation = glGetUniformLocation(shader.Program, "shininess");
    GLint kdLocation = glGetUniformLocation(shader.Program, "Kd");
    GLint ksLocation = glGetUniformLocation(shader.Program, "Ks");
    GLint modelMatrixLocation = glGetUniformLocation(shader.Program, "modelMatrix");
    GLint normalMatrixLocation = glGetUniformLocation(shader.Program, "normalMatrix");

    // Render all model
    for (auto &obj : scene->objects) {
        // Texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, obj.texture);
        glUniform1i(textureLocation, 0);
        glUniform1f(repeatLocation, obj.textureRepeat);

        // Transform matrix (match Unity)
        glm::mat4 modelMatrix(1.0f);
        glm::mat3 normalMatrix(1.0f);
        modelMatrix = glm::translate(modelMatrix, obj.position);
        // First result on Google for the docs still ask for degrees, was deprecated, was fun to find out...
        modelMatrix = glm::rotate(modelMatrix, glm::radians(obj.rotation.y), glm::vec3(0.0f, -1.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(obj.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(obj.rotation.z), glm::vec3(0.0f, 0.0f, -1.0f));
        modelMatrix = glm::scale(modelMatrix, obj.scale);
        normalMatrix = glm::inverseTranspose(glm::mat3(view*modelMatrix));
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix3fv(normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));

        // Material uniform
        glUniform3fv(specularColorLocation, 1, glm::value_ptr(obj.specularColor));
        glUniform1f(shininessLocation, obj.shininess);
        glUniform1f(kdLocation, obj.diffuseFactor);
        glUniform1f(ksLocation, obj.specularFactor);
        obj.model->Draw();
    }
}