#include "../Core/GameObject.h"
#include "../Systems/Animation.h"
#include "../Components/StateComponent.h"


// Object rendering
void GameObject::Draw(SDL_Renderer* renderer, const Camera& camera) {
    // Получаем компоненты
    auto transform = GetComponent<TransformComponent>();
    auto render = GetComponent<RenderComponent>();
    auto animationComponent = GetComponent<AnimationComponent>();
    auto state = GetComponent<StateComponent>();


    // Проверяем, что все необходимые компоненты существуют
    if (!transform || !render || !render->IsVisible || !render->Texture) return;

    // Get the dimensions of the texture
    int textureWidth, textureHeight;
    SDL_QueryTexture(render->Texture, nullptr, nullptr, &textureWidth, &textureHeight);

    // Get sprite dimensions from animation
    int frameWidth = animationComponent ? animationComponent->animation->FrameWidth : textureWidth;
    int frameHeight = animationComponent ? animationComponent->animation->FrameHeight : textureHeight;

    float cameraScale = camera.GetScale(); // Get the camera scale

    // Create a rectangle for floating point drawing
    SDL_FRect destRect = {
        (transform->Position.x - camera.GetPosition().x) * cameraScale - transform->Origin.x * transform->Scale * cameraScale,
        (transform->Position.y - camera.GetPosition().y) * cameraScale - transform->Origin.y * transform->Scale * cameraScale,
        static_cast<float>(frameWidth * transform->Scale * cameraScale),
        static_cast<float>(frameHeight * transform->Scale * cameraScale)
    };

    // Reflection flags
    SDL_RendererFlip flip = transform->FlipHorizontal ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    // Calculate the rotation point (in pixels after scaling)
    SDL_FPoint rotationCenter = {
        transform->Origin.x * transform->Scale* cameraScale,
        transform->Origin.y * transform->Scale* cameraScale
    };

    // Drawing the texture
    SDL_SetTextureColorMod(render->Texture, render->TintColor.r, render->TintColor.g, render->TintColor.b);
    SDL_SetTextureAlphaMod(render->Texture, render->TintColor.a);

    if (animationComponent->animation) {
        // Convert sourceRect to SDL_FRect
        SDL_Rect frameRect = animationComponent->animation->GetCurrentFrameRectangle(animationComponent->SpriteRow);

        SDL_RenderCopyExF(renderer, render->Texture, &frameRect, &destRect, transform->Rotation, &rotationCenter, flip);
    }
    else {
        SDL_RenderCopyExF(renderer, render->Texture, nullptr, &destRect, transform->Rotation, &rotationCenter, flip);
    }


    if (state->IsCollidable) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_Rect colliderRect = {
            ((transform->Collider.x - camera.GetPosition().x) * cameraScale),
            ((transform->Collider.y - camera.GetPosition().y) * cameraScale),
            (transform->Collider.w * cameraScale),
            (transform->Collider.h * cameraScale)
        };
        SDL_RenderDrawRect(renderer, &colliderRect);
    }
}