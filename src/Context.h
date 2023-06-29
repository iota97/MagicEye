#pragma once

struct Context {
     // Screen size
    int width, height;
    bool shouldClose = false;

    // Timing
    float currentFrame;
    float deltaTime;

    // Settings
    bool showMenu = false;
    bool flyingCamera = true;
    bool vSync = true;
    float depthStrength = 5.0;
    float eyeSep = 0.5;
    float obsDistance = 1.8;
    float sceneColorStr = 0.7;
    float edgeStr = 0.0;
    bool stereoRendering = true;
    bool staticPattern = false;
    int pattern = 0;
};