#pragma once

struct Context {
     // Screen size
    int width, height;

    // Timing
    float currentFrame;
    float FPS;

    // Settings
    float depthStrength = 5.0;
};