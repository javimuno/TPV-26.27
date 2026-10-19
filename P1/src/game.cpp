#include "game.h"

#include <string>

#include <SDL3_image/SDL_image.h>

#include "texture.h"

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
	{"balloons.png",7,6},
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

	Balloon* balloon1 = new Balloon(Rectangle({ 270.0f,200.0f }, 64.0f, 64.0f), Vector2D<>(0.0f, -65.0f), getTexture(BALLOON), 1);
	Balloon* balloon2 = new Balloon(Rectangle({ 400.0f,300.0f }, 64.0f, 64.0f), Vector2D<>(0.0f, -75.0f), getTexture(BALLOON), 0);
	Balloon* balloon3 = new Balloon(Rectangle({ 520.0f,120.0f }, 64.0f, 64.0f), Vector2D<>(0.0f, -50.0f), getTexture(BALLOON), 3);

	//globos
	balloons.push_back(balloon1);
	balloons.push_back(balloon2);
	balloons.push_back(balloon3);

	//arco
	mainBow = new Bow(Rectangle({ 100.0f,300.0f }, 100.0f, 100.0f), Vector2D<>{ 0.0f, 0.0f }, getTexture(BOW), 0, 10, false,this);

	//flechas
	//addArrow(Rectangle({ 200.0f,300.0f }, 80.0f, 20.0f), Vector2D<>(250, 0));

	/*Arrow* arrow1 = new Arrow(Rectangle({ 200.0f,300.0f }, 80.0f, 20.0f), Vector2D<>(250, 0), getTexture(ARROW));

	arrows.push_back(arrow1);*/

}

//texturas, renderizador y ventana y cierre
Game::~Game()
{

	for (Balloon* balloon : balloons) {
		delete balloon;
	}
	for (Arrow* arrow : arrows) {
		delete arrow;
	}
	
	delete mainBow;

	for (Texture* texture : textures) {
		delete texture;
	}



	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void
Game::render() const
{
	SDL_RenderClear(renderer);

	// TODO
	for (Balloon* balloon : balloons) {
		balloon->render();
	}
	for (Arrow* arrow : arrows) {
		arrow->render();
	}
	mainBow->render();

	SDL_RenderPresent(renderer);

	
}

void
Game::update(float delta)
{
	// TODO

	int i = 0;

	while (i<balloons.size())
	{
		if (!balloons[i]->update(delta)) {
			delete balloons[i];
			balloons.erase(balloons.begin() + i);
		}
		else
		{
			i++;
		}
	}
	i = 0;
	while (i<arrows.size())
	{
		if (!arrows[i]->update(delta)) {
			delete arrows[i];
			arrows.erase(arrows.begin() + i);
		}
		else
		{
			i++;
		}
	}
	mainBow->update(delta);
	
}
 // pulsaciones, actualizaciones, dibujo y espera ms antes de nueva vuelta
void
Game::run()
{
	Uint64 startTime, frameTime;

	while (!exit) {

		startTime = SDL_GetTicks();
		handleEvents();
		update(FRAME_PERIOD/1000.0f);
		render();

		frameTime = SDL_GetTicks() - startTime;

		if(frameTime<FRAME_PERIOD)
		SDL_Delay(FRAME_PERIOD-frameTime);
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

		mainBow->handleEvent(event);
	}
}

void
Game::addArrow(const Rectangle& rect, const Vector2D<>& vel) {

	arrows.push_back(new Arrow(rect, vel, getTexture(ARROW)));
}

// bool
// Game::checkCollision(const Rectangle& rect)
// {
//	// TODO
//	return false;
// }
