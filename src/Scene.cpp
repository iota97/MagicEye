#include "Scene.h"
#include "utils/image.h"

void Scene::Process() {
    cam->Process();
}

Scene::Scene(SceneCamera *sc, GLFWwindow *w, Context *c) : cam(sc), window(w), ctx(c) {
    lightDir = glm::normalize(glm::vec3(-1.0, 1.0, -1.8));
    skybox = LoadTextureCube("assets/textures/cubes/skybox/");
    sc->SetPosition(glm::vec3(0.0f, 0.0f, -7.0f));
    sc->SetRotation(90, 0);
}

Scene::~Scene() {
    for (auto &t : textures) {
        glDeleteTextures(1, &t);
    }
    glDeleteTextures(1, &skybox);
}

void Scene::SetSkybox(const char* path) {
    glDeleteTextures(1, &skybox);
    skybox = LoadTextureCube(path);
}

Scene1::Scene1(SceneCamera *sc, GLFWwindow *w, Context *c) : Scene(sc, w, c) {
    textures.push_back(LoadTexture("assets/textures/UV_Grid_Sm.png"));
    models.push_back(Model("assets/models/cube.obj"));

    Object obj;
    obj.model = &models[0];
    obj.texture = textures[0];
    objects.push_back(obj);
}