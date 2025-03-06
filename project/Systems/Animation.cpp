#include "Animation.h"
#include <iostream>

Animation::Animation(int frameWidth, int frameHeight, int totalFrames, float timePerFrame)
    : FrameWidth(frameWidth),
    FrameHeight(frameHeight),
    TotalFrames(totalFrames),
    TimePerFrame(timePerFrame),
    CurrentFrame(0),
    ElapsedTime(0.0f),
    IsPlaying(false) {
}

void Animation::Update(bool isMoving, Uint32 deltaTime) {
    if (isMoving) {
        IsPlaying = true;
        ElapsedTime += static_cast<float>(deltaTime) / 1000.0f;

        if (ElapsedTime >= TimePerFrame) {
            ElapsedTime -= TimePerFrame;
            CurrentFrame = (CurrentFrame + 1) % TotalFrames;
        }
    }
    else {
        IsPlaying = false;
    }
}

SDL_Rect Animation::GetCurrentFrameRectangle(int spriteRow) const {
    return SDL_Rect{
        CurrentFrame * FrameWidth,
        spriteRow * FrameHeight,
        FrameWidth,
        FrameHeight
    };
}