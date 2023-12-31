#include "SceneManager.h"

SceneManager::SceneManager(GLFWwindow *w, Context *c) : window(w), ctx(c), cam(glm::vec3(0), w, c) {
    SetScene(ctx->sceneId);
}

SceneManager::~SceneManager() {
    delete scene;
}

void SceneManager::Process() {
    if (sceneId != ctx->sceneId) {
        SetScene(ctx->sceneId);
    }
    scene->Process();
}

Scene *SceneManager::GetScene() {
    return scene;
}

void SceneManager::SetScene(int id) {
    sceneId = id;
    loadingScreen.Draw(window, ctx);
    delete scene;
    switch(id) {
        case 0:
            scene = new SceneStandard(&cam, window, ctx);
            break;
        case 1:
            scene = new SceneFairyHouse(&cam, window, ctx);
            break;
        case 2:
            scene = new SceneChurch(&cam, window, ctx);
            break;
        case 3:
            scene = new SceneFight(&cam, window, ctx);
            break;
        case 4:
            scene = new SceneWindmill(&cam, window, ctx);
            break;
        case 5:
            scene = new SceneSpace(&cam, window, ctx);
            break;
        case 6:
            scene = new SceneDance(&cam, window, ctx);
            break;
        case 7:
            scene = new SceneAnimal(&cam, window, ctx);
            break;
    }
}