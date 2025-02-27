# sdl2

Примерная структкра проекта:
```
project/
├── Components/
│   ├── TransformComponent.h
│   ├── AnimationComponent.h
│   ├── InputComponent.h
│   ├── WeaponComponent.h
│   ├── ColliderComponent.h
│   └── HealthComponent.h
├── Systems/
│   ├── MovementSystem.h
│   ├── AnimationSystem.h
│   ├── WeaponSystem.h
│   ├── CollisionSystem.h
│   ├── AISystem.h
│   └── HealthSystem.h
├── Entities/
│   ├── Player.h
│   ├── Enemy.h
│   └── Bullet.h
├── Core/
│   ├── GameObject.h
│   ├── Component.h
│   ├── System.h
│   └── Utils.h
├── Main/
│   └── main.cpp
└── Resources/
    ├── Textures/
    │   ├── player.png
    │   ├── enemy.png
    │   └── bullet.png
    ├── Fonts/
    │   └── font.ttf
    └── Sounds/
        ├── shoot.wav
        └── explosion.wav
```
