#pragma once

#include <glm/glm.hpp>

class Model;
struct Object {
    glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 rotation = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);

    Model *model;
    GLuint texture;
    GLfloat textureRepeat = 1.0;

    glm::vec3 specularColor = glm::vec3(1.0);
    float shininess = 500.0;
    float diffuseFactor = 1.0;
    float specularFactor = 0.4;
};