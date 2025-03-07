#include "GameObject.h"
#include <iostream>
#include <algorithm>
#include "../Components/MovementComponent.h"

#include "../Core/EntityHeaders.h"

// Обновление объекта
void GameObject::Update(float deltaTime) {
    auto transform = GetComponent<TransformComponent>();
    if (!transform) return;

    // Обновляем коллайдер
    auto collider = GetComponent<ColliderComponent>();
    if (collider) collider->UpdatePosition(transform->Position);
}

// Object rendering
void GameObject::Draw(SDL_Renderer* renderer, const Camera& camera) {
    // Получаем компоненты
    auto transform = GetComponent<TransformComponent>();
    auto collider = GetComponent<ColliderComponent>();
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
    if (cameraScale <= 0) cameraScale = 1.0f; // Защита от деления на ноль

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
        transform->Origin.x * transform->Scale * cameraScale,
        transform->Origin.y * transform->Scale * cameraScale
    };

    // Drawing the texture
    SDL_SetTextureColorMod(render->Texture, render->TintColor.r, render->TintColor.g, render->TintColor.b);
    SDL_SetTextureAlphaMod(render->Texture, render->TintColor.a);

    if (animationComponent) {
        // Convert sourceRect to SDL_FRect
        SDL_Rect frameRect = animationComponent->animation->GetCurrentFrameRectangle(animationComponent->SpriteRow);      

        SDL_RenderCopyExF(renderer, render->Texture, &frameRect, &destRect, transform->Rotation, &rotationCenter, flip);
    }
    else {
        SDL_RenderCopyExF(renderer, render->Texture, nullptr, &destRect, transform->Rotation, &rotationCenter, flip);
    }

    /*
    // Отрисовка Коллайдера
    if (state->IsCollidable && collider) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Зеленый цвет для коллайдера

        if (collider->Type == ColliderComponent::ColliderType::RECTANGLE) {
            // Отрисовка прямоугольного коллайдера
            SDL_Rect colliderRect = {
                ((collider->Collider.x - camera.GetPosition().x) * cameraScale),
                ((collider->Collider.y - camera.GetPosition().y) * cameraScale),
                (collider->Collider.w * cameraScale),
                (collider->Collider.h * cameraScale)
            };
            SDL_RenderDrawRect(renderer, &colliderRect);
        }
        else if (collider->Type == ColliderComponent::ColliderType::CIRCLE) {
            // Отрисовка кругового коллайдера
            int centerX = collider->CircleCollider.x;
            int centerY = collider->CircleCollider.y;
            int radius = collider->CircleRadius;

            RenderCircle(renderer, centerX, centerY, radius, cameraScale, camera);
        }
    }

    // Отрисовка сетки с учетом камеры
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Зеленый цвет для сетки

    int cellSize = 32; // Размер одной ячейки сетки в мире (до масштабирования)
    int scaledCellSize = static_cast<int>(cellSize * cameraScale); // Масштабируем размер ячейки

    // Получаем видимую область камеры в мире
    int visibleWidth = static_cast<int>(800 / cameraScale); // Ширина области в пикселях мира
    int visibleHeight = static_cast<int>(600 / cameraScale); // Высота области в пикселях мира

    // Начальные координаты сетки в мире
    int startX = static_cast<int>(camera.GetPosition().x / cellSize) * cellSize; // Ближайшая левая граница сетки
    int startY = static_cast<int>(camera.GetPosition().y / cellSize) * cellSize; // Ближайшая верхняя граница сетки

    // Отрисовка вертикальных линий
    for (int x = startX; x < startX + visibleWidth; x += cellSize) {
        int screenX = static_cast<int>((x - camera.GetPosition().x) * cameraScale); // Преобразуем мировые координаты в экранные
        SDL_RenderDrawLine(renderer, screenX, 0, screenX, 600); // Рисуем линию по всей высоте экрана
    }

    // Отрисовка горизонтальных линий
    for (int y = startY; y < startY + visibleHeight; y += cellSize) {
        int screenY = static_cast<int>((y - camera.GetPosition().y) * cameraScale); // Преобразуем мировые координаты в экранные
        SDL_RenderDrawLine(renderer, 0, screenY, 800, screenY); // Рисуем линию по всей ширине экрана
    }
    */
}