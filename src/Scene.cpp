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

SceneStandard::SceneStandard(SceneCamera *sc, GLFWwindow *w, Context *c) : Scene(sc, w, c) {
	sc->SetPosition(glm::vec3(-36.619953, 11.382235, -37.909637));
	sc->SetRotation(39.250000, -21.250000);

	textures.push_back(LoadTexture("assets/textures/white.png"));
	models.push_back(Model("assets/models/bunny.obj"));
	models.push_back(Model("assets/models/teapot.obj"));
	models.push_back(Model("assets/models/dragon.obj"));

	Object obj;
	obj.model = &models[0];
	obj.texture = textures[0];
	obj.position = glm::vec3(-0.31, -5.44, -11.62);
	obj.rotation = glm::vec3(0.00, 180.39, 0.00);
	obj.scale = glm::vec3(76.25, 76.25, 76.25);
	obj.textureRepeat = 1;
	objects.push_back(obj);
	obj.model = &models[1];
	obj.texture = textures[0];
	obj.position = glm::vec3(26.07, -3.94, -23.51);
	obj.rotation = glm::vec3(0.00, 251.06, 0.00);
	obj.scale = glm::vec3(2.54, 2.54, 2.54);
	obj.textureRepeat = 1;
	objects.push_back(obj);
	obj.model = &models[2];
	obj.texture = textures[0];
	obj.position = glm::vec3(-28.37, -7.70, -22.26);
	obj.rotation = glm::vec3(0.00, 107.19, 0.00);
	obj.scale = glm::vec3(0.89, 0.89, 0.89);
	obj.textureRepeat = 1;
	objects.push_back(obj);
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

SceneSpace::SceneSpace(SceneCamera *sc, GLFWwindow *w, Context *c) : Scene(sc, w, c) {
	sc->SetPosition(glm::vec3(3.900593, 2.222718, -12.913857));
    sc->SetRotation(142.750000, -11.750000);

	lightDir = glm::vec3(-0.693408, 0.402747, -0.597478);
	SetSkybox("assets/textures/cubes/space/");

	textures.push_back(LoadTexture("assets/textures/tie_aile.jpg"));
	textures.push_back(LoadTexture("assets/textures/tie_sphere.jpg"));
	textures.push_back(LoadTexture("assets/textures/tie_attach.jpg"));
	textures.push_back(LoadTexture("assets/textures/lambert1_baseColor.jpeg"));
	models.push_back(Model("assets/models/tie0.obj"));
	models.push_back(Model("assets/models/tie1.obj"));
	models.push_back(Model("assets/models/tie2.obj"));
	models.push_back(Model("assets/models/tie3.obj"));
	models.push_back(Model("assets/models/tie4.obj"));
	models.push_back(Model("assets/models/xwing.obj"));

	Object obj;
	obj.model = &models[0];
	obj.texture = textures[0];
	obj.position = glm::vec3(41.20, -4.71, -15.58);
	obj.rotation = glm::vec3(343.61, 260.89, 5.68);
	obj.scale = glm::vec3(2.29, 2.29, 2.29);
	obj.textureRepeat = 1;
	objects.push_back(obj);
	obj.model = &models[1];
	obj.texture = textures[1];
	obj.position = glm::vec3(41.20, -4.71, -15.58);
	obj.rotation = glm::vec3(343.61, 260.89, 5.68);
	obj.scale = glm::vec3(2.29, 2.29, 2.29);
	obj.shininess = 150;
	objects.push_back(obj);
	obj.model = &models[2];
	obj.texture = textures[2];
	obj.position = glm::vec3(41.20, -4.71, -15.58);
	obj.rotation = glm::vec3(343.61, 260.89, 5.68);
	obj.scale = glm::vec3(2.29, 2.29, 2.29);
	obj.shininess = 150;
	objects.push_back(obj);
	obj.model = &models[3];
	obj.texture = textures[2];
	obj.position = glm::vec3(41.20, -4.71, -15.58);
	obj.rotation = glm::vec3(343.61, 260.89, 5.68);
	obj.scale = glm::vec3(2.29, 2.29, 2.29);
	obj.shininess = 150;
	objects.push_back(obj);
	obj.model = &models[4];
	obj.texture = textures[0];
	obj.position = glm::vec3(41.20, -4.71, -15.58);
	obj.rotation = glm::vec3(343.61, 260.89, 5.68);
	obj.scale = glm::vec3(2.29, 2.29, 2.29);
	obj.shininess = 150;
	objects.push_back(obj);
	obj.model = &models[5];
	obj.texture = textures[3];
	obj.position = glm::vec3(-12.94, -8.33, -1.46);
	obj.rotation = glm::vec3(7.82, 262.24, 358.54);
	obj.scale = glm::vec3(97.67, 97.67, 97.67);
	obj.shininess = 150;
	objects.push_back(obj);
}
