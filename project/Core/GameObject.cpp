#include "GameObject.h"
#include <iostream>
#include <algorithm>

// Обновление объекта
void GameObject::Update(float deltaTime) {
    auto transform = GetComponent<TransformComponent>();
    auto physics = GetComponent<PhysicsComponent>();

    if (!transform || !physics) return;

    // Обновляем скорость с учётом сил и трения
    physics->Velocity = MathUtils::Add(physics->Velocity, physics->Acceleration);
    physics->Velocity = MathUtils::Multiply(physics->Velocity, physics->Drag);

    // Обновляем позицию с учитывая время
    transform->Position = MathUtils::Add(transform->Position,
        MathUtils::Multiply(physics->Velocity, deltaTime));

    // Обнуляем ускорение для следующего кадра
    physics->Acceleration = { 0, 0 };

    // Обновляем коллайдер
    auto collider = GetComponent<ColliderComponent>();
    if (collider) collider->UpdatePosition(transform->Position);
}

// Решение коллизии
void GameObject::ResolveCollision(GameObject* other) {
    auto transformA = GetComponent<TransformComponent>();
    auto colliderA = GetComponent<ColliderComponent>();
    auto physicsA = GetComponent<PhysicsComponent>();
    auto stateA = GetComponent<StateComponent>();
    auto transformB = other->GetComponent<TransformComponent>();
    auto colliderB = other->GetComponent<ColliderComponent>();
    auto physicsB = other->GetComponent<PhysicsComponent>();
    auto stateB = other->GetComponent<StateComponent>();

    if (!stateA->IsCollidable || !stateB->IsCollidable || !colliderA || !colliderB)
        return;

    const SDL_Rect& rectA = colliderA->Collider;
    const SDL_Rect& rectB = colliderB->Collider;

    // Вычисляем перекрытие
    int overlapX = std::min(rectA.x + rectA.w - rectB.x, rectB.x + rectB.w - rectA.x);
    int overlapY = std::min(rectA.y + rectA.h - rectB.y, rectB.y + rectB.h - rectA.y);

    // Определяем направление нормали
    float nx = (rectA.x + rectA.w / 2) - (rectB.x + rectB.w / 2);
    float ny = (rectA.y + rectA.h / 2) - (rectB.y + rectB.h / 2);
    SDL_FPoint normal = MathUtils::Normalize({ nx, ny });

    // Вычисляем глубину проникновения
    float penetration = std::min(overlapX, overlapY);

    // Применяем силы только к нестатическим объектам
    if (!stateA->IsStatic && !stateB->IsStatic) {
        float combinedMass = physicsA->Mass + physicsB->Mass;
        float force = physicsA->PushForce * (physicsB->Mass / combinedMass);

        if (!stateA->IsStatic) {
            physicsA->Acceleration = MathUtils::Add(
                physicsA->Acceleration,
                MathUtils::Multiply(normal, force / physicsA->Mass)
            );
        }

        if (!stateB->IsStatic) {
            physicsB->Acceleration = MathUtils::Add(
                physicsB->Acceleration,
                MathUtils::Multiply(MathUtils::Negate(normal), force / physicsB->Mass)
            );
        }
    }
    else
    {
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

        if (moveX) {
            SDL_FPoint adjustment = MathUtils::Multiply(normal, penetration);
            adjustment.y = 0; // Запрещаем смещение по оси X


            transformA->Position = MathUtils::Add(transformA->Position, adjustment);
        }

        if (moveY) {
            SDL_FPoint adjustment = MathUtils::Multiply(normal, penetration);
            adjustment.x = 0; // Запрещаем смещение по оси X

            transformA->Position = MathUtils::Add(transformA->Position, adjustment);
        }
    }


    

    /*
    // Плавное смещение на глубину проникновения
    if (stateA->IsStatic && stateB->IsStatic) {
        // Оба статические, не двигаем
    }
    else if (stateA->IsStatic) {
        std::cout << "1" << std::endl;

        transformB->Position = MathUtils::Add(
            transformB->Position,
            MathUtils::Multiply(MathUtils::Negate(normal), penetration)
        );
    }
    else if (stateB->IsStatic) {
        // std::cout << "2" << std::endl;

        // Сдвигаем объект A
        SDL_FPoint adjustment = MathUtils::Multiply(normal, penetration);
        adjustment.x = 0; // Запрещаем смещение по оси X

        // std::cout << normal.x << std::endl;
        // std::cout << normal.y << std::endl;

        transformA->Position = MathUtils::Add(transformA->Position, adjustment);

    }
    else {
        std::cout << "3" << std::endl;

        transformA->Position = MathUtils::Add(
            transformA->Position,
            MathUtils::Multiply(normal, penetration * 0.5f)
        );
        transformB->Position = MathUtils::Add(
            transformB->Position,
            MathUtils::Multiply(MathUtils::Negate(normal), penetration * 0.5f)
        );
    }
    */
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