#pragma once

struct Context {
     // Screen size
    int width, height;
    bool shouldClose = false;

    // Timing
    float currentFrame;
    float deltaTime;

    // Misc
    int sceneId = 0;
    bool showMenu = false;
    bool vSync = true;
    float moveSpeed = 15.0f;
    float mouseSensitivity = 0.25f;
    bool toon = false;
    float colorResolution = 1.0;
    bool fullscreen = false;
    float animSpeed = 1.0;
    bool dqSkinning = true;

    // Sterogram setting
    bool stereoRendering = true;
    float depthStrength = 5.0;
    float eyeSep = 0.3;
    float obsDistance = 1.1;
    float sceneColorStr = 0.9;
    float skyInflunce = 0.01;
    float edgeStr = 0.4;
    float edgeThreshold = 1.0;
    bool edgeNormal = true;
    bool staticPattern = true;
    int pattern = 4;
};