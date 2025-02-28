#pragma once
#include <SDL.h>
#include <cstdint>

class Animation {
public:
    // Size of one sprite
    int FrameWidth = 16;
    int FrameHeight = 16;

    int CurrentFrame = 0;
    int TotalFrames = 8;
    float TimePerFrame = 0.2f; // Time between frames (in seconds)
    float ElapsedTime = 0;

    // Flag to determine if the object is moving
    bool IsPlaying;

    // Designer
    Animation(int frameWidth, int frameHeight, int totalFrames, float timePerFrame);

    // Animation update
    void Update(bool isMoving, Uint32 deltaTime);

    // Getting the rectangle of the current frame
    SDL_Rect GetCurrentFrameRectangle(int spriteRow) const;
};