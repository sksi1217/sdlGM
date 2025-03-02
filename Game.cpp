// Game.cpp
#include "Game.h"
#include <iostream>
#include "project/Components/TransformComponent.h"
#include "project/Core/GameObject.h"
#include "project/Entities/Player.h"
#include "project/Utils/TextureLoader.h"
#include "project/Systems/CollisionSystem.h"
#include "project/Entities/Skelet.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

std::shared_ptr<GameObject> player;
std::shared_ptr<GameObject> enemy;
std::shared_ptr<GameObject> enemy1;
std::shared_ptr<GameObject> enemy2;
std::shared_ptr<GameObject> enemy3;

Camera camera = { {800, 600}, 5 };
CollisionSystem сollisionSystem;

// Designer
Game::Game(const char* title, int width, int height) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
		isRunning = false;
		return;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		std::cerr << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
		SDL_Quit();
		isRunning = false;
		return;
	}


	window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_SHOWN
	);

	if (!window) {
		std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
		isRunning = false;
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
		isRunning = false;
		return;
	}
}

// Destructor
Game::~Game() {
	std::cout << "Destroying renderer and window..." << std::endl;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	std::cout << "Quitting SDL_image and SDL..." << std::endl;
	IMG_Quit();
	SDL_Quit();
}


// Initialization
void Game::Initialize() {
	std::cout << "Game initialized!" << std::endl;
}

// Resource Loading
void Game::LoadContent() {
	std::cout << "Loading content..." << std::endl;

	// Создание текстуры через TextureLoader
	TextureLoader& loader = TextureLoader::GetInstance();

	SDL_Texture* playerTexture = loader.LoadTexture("D:/sdl/sdl2/project/Resources/Textures/player.png", renderer);
	SDL_Texture* boxTexture = loader.LoadTexture("D:/sdl/sdl2/project/Resources/Textures/box.png", renderer);
	SDL_Texture* enemyTexture = loader.LoadTexture("D:/sdl/sdl2/project/Resources/Textures/enemy.png", renderer);


	// Создание игрока
	player = std::make_shared<Player>(SDL_FPoint{ 0, 40 }, playerTexture);
	ManagerGame::objects.push_back(player);

	enemy = std::make_shared<Skelet>(SDL_FPoint{ 0, 0 }, enemyTexture);
	enemy1 = std::make_shared<Skelet>(SDL_FPoint{ 16, 16 }, enemyTexture);
	enemy2 = std::make_shared<Skelet>(SDL_FPoint{ 60, 80 }, enemyTexture);
	enemy3 = std::make_shared<Skelet>(SDL_FPoint{ 0, 16 }, enemyTexture);



	// enemy1 = std::make_shared<Skelet>(SDL_FPoint{ 0, 0 }, enemyTexture);
	// enemy2 = std::make_shared<Skelet>(SDL_FPoint{ 17, 0 }, enemyTexture);
	// enemy3 = std::make_shared<Skelet>(SDL_FPoint{ 35, 0 }, enemyTexture);


	ManagerGame::objects.push_back(enemy);
	ManagerGame::objects.push_back(enemy1);
	ManagerGame::objects.push_back(enemy2);
	ManagerGame::objects.push_back(enemy3);
}

// Logic Update
void Game::Update(float deltaTime) 
{
	for (auto& obj : ManagerGame::objects) {
		obj->Update(deltaTime);
	}

	сollisionSystem.Update();

	camera.UpdateCamera(player->GetComponent<TransformComponent>()->Position, deltaTime);
}

// Frame rendering
void Game::Draw() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Drawing of all objects
	for (auto& obj : ManagerGame::objects) {
		obj->Draw(renderer, camera);
	}

	SDL_RenderPresent(renderer);
}

// Выгрузка ресурсов
void Game::UnloadContent() {
	std::cout << "Unloading content..." << std::endl;
}

// Game.cpp
void Game::HandleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			isRunning = false;
			return; // Выходим из метода сразу после получения события SDL_QUIT
		}
	}
}

void Game::Run() {
	Initialize();
	LoadContent();

	const int TARGET_FPS = 60;
	const Uint32 TARGET_FRAME_TIME = 1000 / TARGET_FPS; // Time of one frame in milliseconds

	Uint32 lastTime = SDL_GetTicks(); // Time of previous frame

	while (isRunning) {
		Uint32 currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastTime) / 1000.0f; // Time difference between frames
		lastTime = currentTime;

		// Logic Update
		Update(deltaTime);

		Draw();

		HandleEvents();

		// Restriction FPS
		Uint32 frameTime = SDL_GetTicks() - currentTime;
		if (frameTime < TARGET_FRAME_TIME) {
			SDL_Delay(TARGET_FRAME_TIME - frameTime);
		}
	}

	UnloadContent();
}