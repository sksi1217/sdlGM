#include "GameObject.h"
#include <iostream>
#include <algorithm>

// Обновление объекта
void GameObject::Update(float deltaTime) {
    auto transform = GetComponent<TransformComponent>();
    auto collider = GetComponent<ColliderComponent>();

    if (transform && collider) {
        collider->UpdatePosition(transform->Position); // Обновляем позицию коллайдера
    }
}

// Решение коллизии
void GameObject::ResolveCollision(GameObject* other) {
    auto transformA = GetComponent<TransformComponent>();
    auto colliderA = GetComponent<ColliderComponent>();
    auto stateA = GetComponent<StateComponent>();
    auto transformB = other->GetComponent<TransformComponent>();
    auto colliderB = other->GetComponent<ColliderComponent>();
    auto stateB = other->GetComponent<StateComponent>();

    if (!stateA->IsCollidable || !stateB->IsCollidable || !colliderA || !colliderB)
        return;

    const SDL_Rect& rectA = colliderA->Collider;
    const SDL_Rect& rectB = colliderB->Collider;

    // Вычисляем перекрытие для каждой оси
    int overlapX = std::min(rectA.x + rectA.w - rectB.x, rectB.x + rectB.w - rectA.x);
    int overlapY = std::min(rectA.y + rectA.h - rectB.y, rectB.y + rectB.h - rectA.y);

    // Определяем направление разрешения коллизии
    bool moveX = overlapX > 0;
    bool moveY = overlapY > 0;

    // Проверяем, какой оси приоритет (опционально)
    if (moveX && moveY) {
        // Если перекрытие по обеим осям, выбираем ось с меньшим перекрытием
        if (overlapX < overlapY) {
            moveY = false;
        }
        else {
            moveX = false;
        }
    }

    // Вычисляем направление вектора разрешения
    SDL_FPoint direction = MathUtils::Normalize(MathUtils::Subtract(transformB->Position, transformA->Position));

    if (moveX) {
        transformA->Position.x -= overlapX * direction.x;
        transformB->Position.x += overlapX * direction.x;
    }

    if (moveY) {
        transformA->Position.y -= overlapY * direction.y;
        transformB->Position.y += overlapY * direction.y;
    }

    // Применяем силу толчка как вектор
    ApplyRepulsion(other, direction);
}

void GameObject::ApplyRepulsion(GameObject* other, const SDL_FPoint& direction) {
    auto stateB = other->GetComponent<StateComponent>();
    if (stateB->IsStatic) return;

    auto physicsA = GetComponent<PhysicsComponent>();
    auto physicsB = other->GetComponent<PhysicsComponent>();

    float combinedMass = physicsA->Mass + physicsB->Mass;
    float force = physicsA->PushForce * (physicsA->Mass / combinedMass);

    // SDL_FPoint pushForce = Multiply(direction, force);
    SDL_FPoint pushForce = { direction.x * force, direction.y * force };

    other->GetComponent<TransformComponent>()->Position =
        MathUtils::Add(other->GetComponent<TransformComponent>()->Position, pushForce);
}

// Проверка столкновения
bool GameObject::CheckCollision(GameObject* other) {
    auto colliderA = GetComponent<ColliderComponent>();
    auto stateA = GetComponent<StateComponent>();
    auto colliderB = other->GetComponent<ColliderComponent>();
    auto stateB = other->GetComponent<StateComponent>();

    if (!colliderA || !colliderB) return false;
    if (!stateA->IsCollidable || !stateB->IsCollidable) return false;

    return colliderA->CheckCollision(*colliderB);
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