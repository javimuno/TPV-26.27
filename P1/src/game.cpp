#include "game.h"

#include <string>

#include <SDL3_image/SDL_image.h>

#include "texture.h"
#include "Balloon.h"

using namespace std;

// Constantes
constexpr const char* const WINDOW_TITLE = "Bow & Arrow 1.0";
constexpr const char* IMAGE_ROOT = "../assets/images/";
constexpr const char* LEVEL_ROOT = "../assets/levels/";

// Estructura para especificar las texturas que hay que
// cargar y el tamaño de su matriz de frames
struct TextureSpec
{
	const char* name;
	int nrows = 1;
	int ncols = 1;
};

constexpr array<TextureSpec, Game::NUM_TEXTURES> textureList{
	TextureSpec{"bow.png", 1, 2},
	{"arrow.png"},
	TextureSpec{"balloons.png", 7, 6},
	TextureSpec{"archer.png", 1, 2},
	TextureSpec{"rewards.png", 10, 8},
	TextureSpec{"digits.png", 1, 10}
};

Game::Game()
	: exit(false)
{
	// Carga SDL y crea la ventana con su renderizador
	if (!SDL_Init(SDL_INIT_VIDEO))
		throw "SDL_Init: "s + SDL_GetError();

	if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		0, &window, &renderer))
		throw "window or renderer: "s + SDL_GetError();

	// Carga las texturas al inicio
	try {
		textures.fill(nullptr);
		string imgRoot = IMAGE_ROOT;

		for (size_t i = 0; i < textures.size(); i++) {
			auto [name, nrows, ncols] = textureList[i];
			textures[i] = new Texture(renderer, (imgRoot + name).c_str(), nrows, ncols);
		}
	}
	catch (...) {
		// TODO: liberar la memoria reservada
		throw; // relanza la excepción
	}

	// Configura que se pueden utilizar capas translúcidas
	// SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// TODO: crear los objetos del juego
	Balloon* balloon1 = new Balloon(getTexture(BALLOONS), { 400.0f,300.0f }, { 0.0f, -50.0f }, 0);

	balloons.push_back(balloon1);
}

Game::~Game()
{
	// TODO: liberar la memoria reservada por la clase
}

void
Game::render() const
{
	SDL_RenderClear(renderer);

	for (auto& ballon : balloons)
		ballon->render();

	SDL_RenderPresent(renderer);
}

void
Game::update()
{
	// TODO
}

void
Game::run()
{
	const Uint32 frameDelay = FRAME_RATE;
	while (!exit) {
		// TODO: implementar bucle del juego
		Uint32 frameStart = SDL_GetTicks();

		render();

		Uint32 frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime)
			SDL_Delay(frameDelay - frameTime);
	}
}

void
Game::handleEvents()
{
	SDL_Event event;

	// Only quit is handled directly, everything else is delegated
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT)
			exit = true;

		// TODO
	}
}

// bool
// Game::checkCollision(const Rectangle& rect)
// {
//	// TODO
//	return false;
// }
