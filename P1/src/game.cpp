#include "game.h"

#include <string>
#include <SDL3_image/SDL_image.h>
#include <fstream>

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

// Lee las flechas iniciales y el intervalo entre globos.
// Ignora el resto de opciones y lanza una excepcion si un valor no es valido.
void Game::loadLevel()
{
	std::ifstream file(std::string (LEVEL_ROOT) + "1.txt");

	if (!file.is_open()) {
		throw std::string("No se puede abrir el archivo de nivel");
	}

	std::string option;
	
	file >> option;

	while (!file.fail())
	{
		
		

		if (option == "arrows") {
			file >> iniArrows_file;

			if (file.fail()||iniArrows_file<0) {
				throw std::string("Valor de arrows no valido");
			}
		}

		else if (option == "baldelay") {
			int baldelayAux;
			file >> baldelayAux;			

			if (file.fail() || baldelayAux<=0) {
				throw std::string("Valor de baldelay no valido");
			}
			balloonInterval_file = baldelayAux / 1000.0f;
		}

		// Descarta el resto de la linea antes de leer la siguiente opcion.
		file.ignore(1000, '\n');
		file >> option;
	}
}

Game::Game()
	: exit(false)
{
	//carga del nivel
	loadLevel();

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
		for (Texture* texture : textures) {
			delete texture;
		}

		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		throw; // relanza la excepción
	}

	// Configura que se pueden utilizar capas translúcidas
	// SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// TODO: crear los objetos del juego

	//arco
	mainBow = new Bow(Rectangle({ 100.0f,300.0f }, 100.0f, 100.0f), Vector2D<>{ 0.0f, 0.0f }, getTexture(BOW),iniArrows_file, false,this);

	

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
	
	updateBalloons(delta);
	updateArrows(delta);

	balloonTimer += delta;

	if (balloonTimer >= balloonInterval_file) {
		addBalloon();
		balloonTimer = 0.0f;
	}
	
	mainBow->update(delta);

	//Fin de partida
	if (!mainBow->hasArrows() && arrows.empty()) {
		std::cout << "Fin de partida" << std::endl;
		exit = true;
	}
	
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

		
		mainBow->handleEvent(event);
	}
}

void 
Game::addBalloon()
{

	//colores

	int minC = BALLOON_MIN_COLOR;
	int maxC = BALLOON_MAX_COLOR;

	std::uniform_int_distribution<int> cDistribution(minC, maxC);
	int c = cDistribution(randomGenerator);

	//franja del rango de espacio aleatorio
	float minX = WINDOW_WIDTH * BALLOON_AREA_START;
	float maxX = WINDOW_WIDTH - BALLOON_WIDTH;

	std::uniform_real_distribution<float> xDistribution(minX, maxX);
	float x = xDistribution(randomGenerator);

	//velocidades aleatorias

	float minV = BALLOON_MIN_SPEED;
	float maxV = BALLOON_MAX_SPEED;

	std::uniform_real_distribution<float> vDistribution(minV, maxV);
	float v = vDistribution(randomGenerator);

	//creacion
	balloons.push_back(new Balloon(Rectangle({x,WINDOW_HEIGHT }, BALLOON_WIDTH, BALLOON_WIDTH), Vector2D<>(0, -v), getTexture(BALLOON), c));
}
// Actualiza los globos y elimina los que han terminado su recorrido o explosion.
void Game::updateBalloons(float delta)
{

	int i = 0;

	while (i < balloons.size()) 
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
}
// Actualiza las flechas y elimina las que han salido de la ventana.
void Game::updateArrows(float delta)
{
	int i = 0;
	while (i < arrows.size()) //flechas
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

	
}

void
Game::addArrow(const Rectangle& rect, const Vector2D<>& vel) {

	arrows.push_back(new Arrow(rect, vel, getTexture(ARROW),this));
}


// Busca el primer globo alcanzado y suma puntos una sola vez por globo.
 bool
 Game::checkCollision(const Rectangle& rect)
 {
	 for (Balloon* balloon : balloons) {
		 if (balloon->hit(rect)) {
			 score += BALLOON_POINTS;
			 std::cout << "Puntuacion: " << score << "\n";
			 return true;
		}
	}
	 return false;
 }

