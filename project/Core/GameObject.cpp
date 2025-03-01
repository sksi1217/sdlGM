#include "GameObject.h"
#include <iostream>
#include <algorithm>
#include "../Components/MovementComponent.h"

// Обновление объекта
void GameObject::Update(float deltaTime) {
    auto transform = GetComponent<TransformComponent>();
    auto physics = GetComponent<PhysicsComponent>();

    if (!transform || !physics) return;

    // Обновляем скорость с учетом силы и трения
    physics->Velocity = MathUtils::Add(physics->Velocity, physics->Acceleration);
    physics->Velocity = MathUtils::Multiply(physics->Velocity, physics->Drag);

    // Обновляем позицию с учетом времени
    transform->Position = MathUtils::Add(transform->Position,
        MathUtils::Multiply(physics->Velocity, deltaTime));

    // Обнуляем ускорение для следующего кадра
    physics->Acceleration = { 0, 0 };

    // Обновляем коллайдер
    auto collider = GetComponent<ColliderComponent>();
    if (collider) collider->UpdatePosition(transform->Position);
}

// Проверка столкновения
bool GameObject::CheckCollision(GameObject* other) {
    auto colliderA = GetComponent<ColliderComponent>();
    auto colliderB = other->GetComponent<ColliderComponent>();

    // auto stateA = GetComponent<StateComponent>();
    // auto stateB = other->GetComponent<StateComponent>();

    if (!colliderA || !colliderB) return false;
    // if (!stateA->IsCollidable || !stateB->IsCollidable) return false;

    return colliderA->CheckCollision(*colliderB);
}

// Решение коллизии
void GameObject::ResolveCollision(GameObject* other) {
    auto transformA = GetComponent<TransformComponent>();
    auto transformB = other->GetComponent<TransformComponent>();

    auto physicsA = GetComponent<PhysicsComponent>();
    auto physicsB = other->GetComponent<PhysicsComponent>();

    auto colliderA = GetComponent<ColliderComponent>();
    auto colliderB = other->GetComponent<ColliderComponent>();

    auto stateA = GetComponent<StateComponent>();
    auto stateB = other->GetComponent<StateComponent>();

    if (!transformA || !transformB || !physicsA || !physicsB || !colliderA || !colliderB) return;

    // Получаем глубину пересечения
    SDL_FPoint intersectionDepth = colliderA->GetIntersectionDepth(*colliderB);

    // Если есть пересечение, исправляем позицию объекта
    if (intersectionDepth.x != 0 || intersectionDepth.y != 0) {
        // Исправляем позицию A
        transformA->Position.x += intersectionDepth.x;
        transformA->Position.y += intersectionDepth.y;

        // Обновляем коллайдер после изменения позиции
        colliderA->UpdatePosition(transformA->Position);

        /* Останавливаем движение в направлении пересечения
        if (intersectionDepth.x != 0) {
            physicsA->Velocity.x = 0; // Остановка по оси X
        }
        if (intersectionDepth.y != 0) {
            physicsA->Velocity.y = 0; // Остановка по оси Y
        }
        */
    }
}
// Object rendering
void GameObject::Draw(SDL_Renderer* renderer, const Camera& camera) {
    // Получаем компоненты
    auto transform = GetComponent<TransformComponent>();
    auto render = GetComponent<RenderComponent>();
    auto animationComponent = GetComponent<AnimationComponent>();
    auto state = GetComponent<StateComponent>();
    auto collider = GetComponent<ColliderComponent>();


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
            ((collider->Collider.x - camera.GetPosition().x) * cameraScale),
            ((collider->Collider.y - camera.GetPosition().y) * cameraScale),
            (collider->Collider.w * cameraScale),
            (collider->Collider.h * cameraScale)
        };
        SDL_RenderDrawRect(renderer, &colliderRect);
    }
}