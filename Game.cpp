// Game.cpp
#include "Game.h"
#include <iostream>
#include "project/Components/TransformComponent.h"
#include "project/Core/GameObject.h"
#include "project/Entities/Player.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Создание списка объектов
std::vector<std::shared_ptr<GameObject>> entities;

std::shared_ptr<GameObject> player;

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

	// Создание игрока
	player = std::make_shared<Player>();
	entities.push_back(player);
}

// Logic Update
void Game::Update(float deltaTime) 
{
	auto transform = player->GetComponent<TransformComponent>();

	std::cout << transform->Position.x << std::endl;
	std::cout << transform->Position.y << std::endl;
}

// Frame rendering
void Game::Draw() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);



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