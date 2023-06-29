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
    bool flyingCamera = true;
    bool vSync = true;

    // Sterogram setting
    bool stereoRendering = false;
    float depthStrength = 5.0;
    float eyeSep = 0.5;
    float obsDistance = 1.8;
    float sceneColorStr = 0.7;
    float edgeStr = 0.4;
    float edgeThreshold = 0.5;
    bool staticPattern = true;
    int pattern = 5;
};