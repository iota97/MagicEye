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

    // Sterogram setting
    bool stereoRendering = true;
    float depthStrength = 5.0;
    float eyeSep = 0.5;
    float obsDistance = 1.8;
    float sceneColorStr = 0.9;
    float edgeStr = 0.4;
    float edgeThreshold = 1.0;
    bool staticPattern = true;
    int pattern = 5;
};