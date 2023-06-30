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

SceneFairyHouse::SceneFairyHouse(SceneCamera *sc, GLFWwindow *w, Context *c) : Scene(sc, w, c) {
    sc->SetPosition(glm::vec3(14.605305, -2.974416, -29.658712));
    sc->SetRotation(471.5, 2.25);

	textures.push_back(LoadTexture("assets/textures/ground_baseColor.jpeg"));
	textures.push_back(LoadTexture("assets/textures/piedras_baseColor.jpeg"));
	textures.push_back(LoadTexture("assets/textures/material_baseColor.jpeg"));
	textures.push_back(LoadTexture("assets/textures/roof_baseColor.jpeg"));
	textures.push_back(LoadTexture("assets/textures/madera_baseColor.jpeg"));
	textures.push_back(LoadTexture("assets/textures/tree_leaves_baseColor.jpeg"));
	models.push_back(Model("assets/models/house0.obj"));
	models.push_back(Model("assets/models/house1.obj"));
	models.push_back(Model("assets/models/house2.obj"));
	models.push_back(Model("assets/models/house3.obj"));
	models.push_back(Model("assets/models/house4.obj"));
	models.push_back(Model("assets/models/house5.obj"));
	models.push_back(Model("assets/models/house6.obj"));
	models.push_back(Model("assets/models/house7.obj"));
	models.push_back(Model("assets/models/house8.obj"));
	models.push_back(Model("assets/models/house9.obj"));

	Object obj;
	obj.model = &models[0];
	obj.texture = textures[0];
	obj.position = glm::vec3(-0.31, 0.53, 0.37);
	obj.rotation = glm::vec3(0.00, 0.00, 0.00);
	obj.scale = glm::vec3(3.73, 3.73, 3.73);
	obj.textureRepeat = 1;
	objects.push_back(obj);
	obj.model = &models[1];
	obj.texture = textures[1];
	obj.position = glm::vec3(-0.31, 0.53, 0.37);
	obj.rotation = glm::vec3(0.00, 0.00, 0.00);
	obj.scale = glm::vec3(3.73, 3.73, 3.73);
	obj.textureRepeat = 1;
	objects.push_back(obj);
	obj.model = &models[2];
	obj.texture = textures[2];
	obj.position = glm::vec3(-0.31, 0.53, 0.37);
	obj.rotation = glm::vec3(0.00, 0.00, 0.00);
	obj.scale = glm::vec3(3.73, 3.73, 3.73);
	obj.textureRepeat = 1;
	objects.push_back(obj);
	obj.model = &models[3];
	obj.texture = textures[1];
	obj.position = glm::vec3(-0.31, 0.53, 0.37);
	obj.rotation = glm::vec3(0.00, 0.00, 0.00);
	obj.scale = glm::vec3(3.73, 3.73, 3.73);
	obj.textureRepeat = 1;
	objects.push_back(obj);
	obj.model = &models[4];
	obj.texture = textures[1];
	obj.position = glm::vec3(-0.31, 0.53, 0.37);
	obj.rotation = glm::vec3(0.00, 0.00, 0.00);
	obj.scale = glm::vec3(3.73, 3.73, 3.73);
	obj.textureRepeat = 1;
	objects.push_back(obj);
	obj.model = &models[5];
	obj.texture = textures[3];
	obj.position = glm::vec3(-0.31, 0.53, 0.37);
	obj.rotation = glm::vec3(0.00, 0.00, 0.00);
	obj.scale = glm::vec3(3.73, 3.73, 3.73);
	obj.textureRepeat = 1;
	objects.push_back(obj);
	obj.model = &models[6];
	obj.texture = textures[4];
	obj.position = glm::vec3(-0.31, 0.53, 0.37);
	obj.rotation = glm::vec3(0.00, 0.00, 0.00);
	obj.scale = glm::vec3(3.73, 3.73, 3.73);
	obj.textureRepeat = 1;
	objects.push_back(obj);
	obj.model = &models[7];
	obj.texture = textures[5];
	obj.position = glm::vec3(-0.31, 0.53, 0.37);
	obj.rotation = glm::vec3(0.00, 0.00, 0.00);
	obj.scale = glm::vec3(3.73, 3.73, 3.73);
	obj.textureRepeat = 1;
	objects.push_back(obj);
	obj.model = &models[8];
	obj.texture = textures[1];
	obj.position = glm::vec3(-0.31, 0.53, 0.37);
	obj.rotation = glm::vec3(0.00, 0.00, 0.00);
	obj.scale = glm::vec3(3.73, 3.73, 3.73);
	obj.textureRepeat = 1;
	objects.push_back(obj);
	obj.model = &models[9];
	obj.texture = textures[3];
	obj.position = glm::vec3(-0.31, 0.53, 0.37);
	obj.rotation = glm::vec3(0.00, 0.00, 0.00);
	obj.scale = glm::vec3(3.73, 3.73, 3.73);
	obj.textureRepeat = 1;
	objects.push_back(obj);
}