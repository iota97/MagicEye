#pragma once

#include <glm/glm.hpp>

class Model;
struct Object {
    Model *model;
    GLuint texture;
    glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 rotation = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);
    GLfloat textureRepeat = 1.0;
};