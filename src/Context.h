#pragma once

struct Context {
     // Screen size
    int width, height;

    // Timing
    float currentFrame;
    float FPS;
    float deltaTime;

    // Settings
    bool showMenu = false;
    bool flyingCamera = true;
    float depthStrength = 5.0;
};