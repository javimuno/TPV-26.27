#include "game.h"

#include <sstream>
#include <fstream>
#include <string>
#include <stdexcept>

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
	textures.fill(nullptr);
	window = nullptr;
	renderer = nullptr;
	playerBow = nullptr;

	try {
		// Carga SDL
		if (!SDL_Init(SDL_INIT_VIDEO))
			throw "Error al inicializar SDL: "s + SDL_GetError();

		// Crea la ventana y el renderizador
		if (!SDL_CreateWindowAndRenderer(
			WINDOW_TITLE,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			0,
			&window,
			&renderer))
		{
			throw "Error al crear la ventana o el renderer: "s
				+ SDL_GetError();
		}

		// Carga las texturas al inicio
		string imgRoot = IMAGE_ROOT;

		for (size_t i = 0; i < textures.size(); i++) {
			auto [name, nrows, ncols] = textureList[i];

			try {
				textures[i] = new Texture(
					renderer,
					(imgRoot + name).c_str(),
					nrows,
					ncols
				);
			}
			catch (const string& error) {
				throw "Error al cargar la imagen "s + name + ": " + error;
			}
		}

		// Configura que se pueden utilizar capas translúcidas
		// SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		// Carga la configuración del nivel
		loadLevel(1);

		// Crea el arco del jugador
		playerBow = new Bow(
			getTexture(BOW),
			{ WINDOW_WIDTH / 10, WINDOW_HEIGHT / 2 },
			{ 0.0f, 0.0f },
			10,
			this
		);
	}
	catch (...) {
		// Libera las texturas que se hayan creado
		for (Texture*& texture : textures) {
			delete texture;
			texture = nullptr;
		}

		// Libera los objetos creados
		delete playerBow;
		playerBow = nullptr;

		// Libera los recursos de SDL
		if (renderer != nullptr) {
			SDL_DestroyRenderer(renderer);
			renderer = nullptr;
		}

		if (window != nullptr) {
			SDL_DestroyWindow(window);
			window = nullptr;
		}

		SDL_Quit();

		// Propaga la excepción original hasta main()
		throw;
	}
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
	SDL_SetRenderDrawColor(renderer, bgR, bgG, bgB, 255);
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

	if (timerBallons >= (balloonDelay/1000)) {
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
	std::uniform_int_distribution<int> colorDist(0, NUM_FRAMES_BALLONS);
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

 void Game::loadLevel(const int numLevel)
 {
	 std::string levelPath =
		 std::string(LEVEL_ROOT) + std::to_string(numLevel) + ".txt";

	 std::ifstream file(levelPath);

	 // Comprobar que se ha podido abrir el fichero
	 if (!file.is_open()) {
		 throw "No se pudo abrir el mapa: "s + levelPath;
	 }

	 std::string line;
	 size_t lineno = 0;

	 while (std::getline(file, line)) {
		 lineno++;

		 std::istringstream iss(line);
		 std::string keyword;

		 // Leer la palabra clave
		 iss >> keyword;

		 // Ignorar líneas vacías y comentarios
		 if (keyword.empty() || keyword[0] == '#') {
			 continue;
		 }

		 if (keyword == "arrows") {
			 if (!(iss >> numArrows)) {
				 throw "Error de formato en "s + levelPath
					 + ", linea " + std::to_string(lineno)
					 + ": arrows necesita un numero entero.";
			 }

			 if (numArrows < 0) {
				 throw "Error de formato en "s + levelPath
					 + ", linea " + std::to_string(lineno)
					 + ": arrows no puede ser negativo.";
			 }

			 std::string extra;
			 if (iss >> extra) {
				 throw "Error de formato en "s + levelPath
					 + ", linea " + std::to_string(lineno)
					 + ": sobran datos despues de arrows.";
			 }
		 }
		 else if (keyword == "baldelay") {
			 if (!(iss >> balloonDelay)) {
				 throw "Error de formato en "s + levelPath
					 + ", linea " + std::to_string(lineno)
					 + ": baldelay necesita un numero.";
			 }

			 if (balloonDelay <= 0) {
				 throw "Error de formato en "s + levelPath
					 + ", linea " + std::to_string(lineno)
					 + ": baldelay debe ser positivo.";
			 }

			 std::string extra;
			 if (iss >> extra) {
				 throw "Error de formato en "s + levelPath
					 + ", linea " + std::to_string(lineno)
					 + ": sobran datos despues de baldelay.";
			 }
		 }
		 else if (keyword == "bgcolor") {
			 int r, g, b;

			 // Comprobar que se han leido los tres componentes
			 if (!(iss >> r >> g >> b)) {
				 throw "Error de formato en "s + levelPath
					 + ", linea " + std::to_string(lineno)
					 + ": bgcolor necesita tres valores RGB.";
			 }

			 // Comprobar que cada componente esta entre 0 y 255
			 if (r < 0 || r > 255 ||
				 g < 0 || g > 255 ||
				 b < 0 || b > 255) {
				 throw "Error de formato en "s + levelPath
					 + ", linea " + std::to_string(lineno)
					 + ": los valores RGB deben estar entre 0 y 255.";
			 }

			 // Comprobar que no hay datos adicionales
			 std::string extra;
			 if (iss >> extra) {
				 throw "Error de formato en "s + levelPath
					 + ", linea " + std::to_string(lineno)
					 + ": sobran datos despues de bgcolor.";
			 }

			 // Guardar el color de fondo
			 bgR = static_cast<Uint8>(r);
			 bgG = static_cast<Uint8>(g);
			 bgB = static_cast<Uint8>(b);
		 }
		 else {
			 // Ignorar el resto de la linea desconocida
			 iss.ignore(
				 std::numeric_limits<std::streamsize>::max(),
				 '\n'
			 );
		 }
	 }

	 // Comprobar si ha ocurrido un error de lectura
	 if (file.bad()) {
		 throw "Error al leer el fichero de configuracion: "s + levelPath;
	 }
 }
