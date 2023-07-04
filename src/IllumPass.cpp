#include "IllumPass.h"
#include "Object.h"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

IllumPass::IllumPass(Context *c) : illumShader("assets/shaders/illumin.vert", "assets/shaders/illumin.frag"), 
    skinningShader("assets/shaders/skinning.vert", "assets/shaders/illumin.frag"), ctx(c) {}

IllumPass::~IllumPass() {
    illumShader.Delete();
    skinningShader.Delete();
}

void IllumPass::SetupScene(Shader &shader, Scene *scene) {
    // Select illum model
    GLsizei n;
    glGetProgramStageiv(shader.Program, GL_FRAGMENT_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS, &n);
    std::vector<GLuint> index(n);
    GLint computePassLocation = glGetSubroutineUniformLocation(shader.Program, GL_FRAGMENT_SHADER, "illumModel");
    index[computePassLocation] = glGetSubroutineIndex(shader.Program, GL_FRAGMENT_SHADER, ctx->toon ? "toon" : "phong");
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, n, &index[0]);

    // Camera setup
    glm::mat4 projection = scene->cam->GetProjectionMatrix();
    glm::mat4 view = scene->cam->GetViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
    glUniform3fv(glGetUniformLocation(shader.Program, "lightVector"), 1, glm::value_ptr(scene->lightDir));
    glUniform1f(glGetUniformLocation(shader.Program, "Ka"), scene->ambientFactor);
    glUniform1f(glGetUniformLocation(shader.Program, "colorResolution"), ctx->colorResolution);
}

void IllumPass::SetupObject(Shader &shader, Scene *scene, Object &obj) {
    // Object uniform location
    GLint textureLocation = glGetUniformLocation(shader.Program, "tex");
    GLint repeatLocation = glGetUniformLocation(shader.Program, "repeat");
    GLint specularColorLocation = glGetUniformLocation(shader.Program, "specularColor");
    GLint shininessLocation = glGetUniformLocation(shader.Program, "shininess");
    GLint kdLocation = glGetUniformLocation(shader.Program, "Kd");
    GLint ksLocation = glGetUniformLocation(shader.Program, "Ks");
    GLint modelMatrixLocation = glGetUniformLocation(shader.Program, "modelMatrix");
    GLint normalMatrixLocation = glGetUniformLocation(shader.Program, "normalMatrix");

    // Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, obj.texture);
    glUniform1i(textureLocation, 0);
    glUniform1f(repeatLocation, obj.textureRepeat);

    // Transform matrix (match Unity)
    glm::mat4 modelMatrix(1.0f);
    glm::mat3 normalMatrix(1.0f);
    glm::mat4 view = scene->cam->GetViewMatrix();
    modelMatrix = glm::translate(modelMatrix, obj.position);
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
}

void IllumPass::execute(Scene *scene) {
    illumShader.Use();
    SetupScene(illumShader, scene);
    
    // Render all model
    for (auto &obj : scene->objects) {
        SetupObject(illumShader, scene, obj);
        obj.model->Draw();
    }

    skinningShader.Use();
    SetupScene(skinningShader, scene);

    for (auto &obj : scene->skinned_objects) {
        obj.anim->UpdateAnimation(ctx->deltaTime);
        SetupObject(skinningShader, scene, obj);
        auto transforms = obj.anim->GetFinalBoneMatrices();
        for (size_t i = 0; i < transforms.size(); ++i) {
            glUniformMatrix4fv(glGetUniformLocation(skinningShader.Program,
                ("finalBonesMatrices[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(transforms[i]));
        }
        obj.s_model->Draw();
    }
}