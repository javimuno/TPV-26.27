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
	playerBow = new Bow(getTexture(BOW), { WINDOW_WIDTH/10, WINDOW_HEIGHT/2 }, { 0.0f, 0.0f }, 10, this);
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
	
	delete playerBow;

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
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	for (Balloon* balloon : balloons) {
		balloon->render();
	}
	for (Arrow* arrow : arrows) {
		arrow->render();
	}
	playerBow->render();

	SDL_RenderPresent(renderer);

	
}

void
Game::update(float delta)
{
	auto itB = balloons.begin();

	while (itB != balloons.end()) {
		if (!(*itB)->update(delta)) {
			delete* itB;
			itB = balloons.erase(itB);
		}
		else {
			++itB;
		}
	}
		
	auto itA = arrows.begin();

	while (itA != arrows.end()) {
		if (!(*itA)->update(delta)) {
			delete* itA;
			itA = arrows.erase(itA);
		}
		else {
			++itA;
		}
	}

	timerBallons += delta;

	if (timerBallons >= DELAY_BALLOON) {
		addBalloon();
		timerBallons =0;
	}
	playerBow->update(delta);
	
}
 // pulsaciones, actualizaciones, dibujo y espera ms antes de nueva vuelta
void
Game::run()
{
	const Uint32 frameDelay = FRAME_RATE;

	while (!exit) {

		Uint32 frameStart = SDL_GetTicks();
		handleEvents();
		update(FRAME_PERIOD/1000.0f);
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
		else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)
			exit = true;

		else
			playerBow->handleEvent(event);
	}
}

void 
Game::addBalloon()
{
	// Calcular una posición horizontal aleatoria
	const float minX = WINDOW_WIDTH / 3.0f;
	const float maxX = WINDOW_WIDTH -
		getTexture(BALLOON)->getFrameWidth() * GAME_SCALE;

	std::uniform_real_distribution<float> posDist(minX, maxX);
	const float posX = posDist(randomGenerator);

	// Elegir la velocidad vertical
	std::uniform_real_distribution<float> speedDist(MIN_SPEED_BALLONS, MAX_SPEED_BALLONS);
	const float speed = speedDist(randomGenerator);

	// Crear el globo en la parte inferior de la ventana
	const Point2D<float> position{ posX, static_cast<float>(WINDOW_HEIGHT) };
	const Vector2D<float> velocity{ 0.0f, -speed };

	// Elegir un color aleatorio
	std::uniform_int_distribution<int> colorDist(0, NUM_BALLONS);
	const int color = colorDist(randomGenerator);

	balloons.push_back(
		new Balloon(getTexture(BALLOON), position, velocity, color)
	);
}

void
Game::addArrow(const Vector2D<float>& pos, const Vector2D<float>& vel) {
	arrows.push_back(new Arrow(getTexture(ARROW), pos, vel, this));
}

 bool
 Game::checkCollision(const Rectangle& rect)
 {
	 for (auto* balloon : balloons)
	 {
		 if (balloon->hit(rect))
		 {
			 return true;
		 }
	 }

	 return false;
 }
