#include "Scene.h"
#include "utils/image.h"

void Scene::Process() {
    cam->Process();
}

Scene::Scene(SceneCamera *sc, GLFWwindow *w, Context *c) : cam(sc), window(w), ctx(c) {
    lightDir = glm::vec3(-0.376533, 0.484810, -0.789419);
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

	lightDir = glm::vec3(-0.435924, 0.876727, -0.203274);
	SetSkybox("assets/textures/cubes/anime/");

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

SceneChurch::SceneChurch(SceneCamera *sc, GLFWwindow *w, Context *c) : Scene(sc, w, c) {
	sc->SetPosition(glm::vec3(-0.825525, 11.907833, 0.286204));
    sc->SetRotation(-51.750000, -8.750000);
	lightDir = glm::vec3(0.435924, 0.876727, 0.203274);
	
	textures.push_back(LoadTexture("assets/textures/StDunstanInTheWest01_Model_5_u1_v1_baseColor.jpeg"));
	models.push_back(Model("assets/models/church.obj"));

	Object obj;
	obj.model = &models[0];
	obj.specularFactor = 0.2;
	obj.texture = textures[0];
	obj.scale = glm::vec3(3);
	objects.push_back(obj);
}

SceneFight::SceneFight(SceneCamera *sc, GLFWwindow *w, Context *c) : Scene(sc, w, c) {
	sc->SetPosition(glm::vec3(-29.035936, 85.732651, 53.747051));
    sc->SetRotation(302.250000, -25.000000);

	lightDir = glm::vec3(-0.940326, -0.254602, 0.225752);
	SetSkybox("assets/textures/cubes/fantasy/");

	textures.push_back(LoadTexture("assets/textures/Bake_Bridge_baseColor.png"));
	textures.push_back(LoadTexture("assets/textures/Bake_Blood_baseColor.png"));
	models.push_back(Model("assets/models/fight0.obj"));
	models.push_back(Model("assets/models/fight1.obj"));

	Object obj;
	obj.model = &models[0];
	obj.texture = textures[0];
	obj.position = glm::vec3(-1.14, 1.03, 2.50);
	obj.rotation = glm::vec3(0.00, 0.00, 0.00);
	obj.scale = glm::vec3(0.4);
	obj.textureRepeat = 1;
	obj.shininess = 200;
	obj.specularFactor = 0.9;
	objects.push_back(obj);
	obj.model = &models[1];
	obj.texture = textures[1];
	obj.position = glm::vec3(-1.14, 1.03, 2.50);
	obj.rotation = glm::vec3(0.00, 0.00, 0.00);
	obj.scale = glm::vec3(0.4);
	obj.textureRepeat = 1;
	objects.push_back(obj);
}

SceneWindmill::SceneWindmill(SceneCamera *sc, GLFWwindow *w, Context *c) : Scene(sc, w, c) {
	sc->SetPosition(glm::vec3(11.345096, 0.781114, -6.353123));
    sc->SetRotation(161.500000, -3.000000);
	
	textures.push_back(LoadTexture("assets/textures/standardSurface1_baseColor.png"));
	models.push_back(Model("assets/models/windmill0.obj"));
	models.push_back(Model("assets/models/windmill1.obj"));
	models.push_back(Model("assets/models/windmill2.obj"));

	Object obj;
	obj.model = &models[0];
	obj.texture = textures[0];
	obj.position = glm::vec3(-0.31, 0.53, 0.37);
	obj.rotation = glm::vec3(0.00, 0.00, 0.00);
	obj.scale = glm::vec3(2.35, 2.24, 2.35);
	obj.textureRepeat = 1;
	objects.push_back(obj);
	obj.model = &models[1];
	obj.texture = textures[0];
	obj.position = glm::vec3(-0.31, 0.53, 0.37);
	obj.rotation = glm::vec3(0.00, 0.00, 0.00);
	obj.scale = glm::vec3(2.35, 2.24, 2.35);
	obj.textureRepeat = 1;
	objects.push_back(obj);
	obj.model = &models[2];
	obj.texture = textures[0];
	obj.position = glm::vec3(-0.31, 1.11, 0.37);
	obj.rotation = glm::vec3(0.00, 0.00, 0.00);
	obj.scale = glm::vec3(2.35, 2.50, 2.35);
	obj.textureRepeat = 1;
	objects.push_back(obj);
}

void SceneWindmill::Process() {
	Scene::Process();
	objects[0].rotation.x += 10.0 * ctx->deltaTime;
}
