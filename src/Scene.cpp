#include "Scene.h"
#include "utils/image.h"

void Scene::Process() {
    cam->Process();
}

Scene::Scene(SceneCamera *sc, GLFWwindow *w, Context *c) : cam(sc), window(w), ctx(c) {}

Scene::~Scene() {
    for (auto &t : textures) {
        glDeleteTextures(1, &t);
    }
}

Scene1::Scene1(SceneCamera *sc, GLFWwindow *w, Context *c) : Scene(sc, w, c) {
    textures.push_back(LoadTexture("assets/textures/UV_Grid_Sm.png"));
    models.push_back(Model("assets/models/cube.obj"));

    lightDir = glm::vec3(1.0, 1.0, 1.0);
    sc->SetPosition(glm::vec3(0.0f, 0.0f, 7.0f));
    sc->SetRotation(-90, 0);

    Object obj;
    obj.model = &models[0];
    obj.texture = textures[0];
    objects.push_back(obj);
}