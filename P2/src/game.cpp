#include "game.h"

#include <sstream>
#include <fstream>
#include <string>
#include <stdexcept>

#include <SDL3_image/SDL_image.h>

#include "texture.h"
#include "Butterfly.h"
#include "Balloon.h"
#include "Bow.h"
#include "Arrow.h"

#include "GameError.h"
#include "SDLError.h"
#include "FileNotFoundError.h"
#include "FileFormatError.h"

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
	{"butterfly.png",4,10},
	{"digits.png", 1, 10}
};

void Game::saveGame() const
{
	std::ofstream file("saved.txt");

	if (!file.is_open())
		throw std::string("Error al guardar la partida");

	// Identificador y versión del archivo
	file << "GAME 1\n";

	// Información general de la partida
	file << currentLevel << " "
		<< score << " "
		<< numArrows << " "
		<< timerBallons << "\n";

	// Número de objetos
	file << sceneObjects.size() << "\n";

	// Guardar todos los objetos
	for (SceneObject* obj : sceneObjects)
		obj->save(file);

	if (!file.good())
		throw std::string("Error al escribir el archivo de guardado");

	file.close();
}

void Game::loadGame()
{
	const std::string filename = "saved.txt";
	std::ifstream file(filename);

	if (!file.is_open())
		throw FileNotFoundError(filename);

	size_t lineno = 1;

	// Comprobar cabecera
	std::string header;
	int version;

	if (!(file >> header >> version) || header != "GAME" || version != 1)
		throw FileFormatError(filename, lineno, "Cabecera incorrecta");

	lineno++;

	// Leer información general
	int savedLevel;
	int savedScore;
	int savedArrows;
	float savedTimer;

	if (!(file >> savedLevel >> savedScore >> savedArrows >> savedTimer))
		throw FileFormatError(filename, lineno, "Error al leer los datos generales");

	if (savedLevel < 1)
		throw FileFormatError(filename, lineno, "El nivel debe ser mayor que cero");

	if (savedArrows < 0)
		throw FileFormatError(filename, lineno, "El numero de flechas no puede ser negativo");

	if (savedTimer < 0 || !std::isfinite(savedTimer))
		throw FileFormatError(filename, lineno, "El temporizador no es valido");

	lineno++;

	// Leer número de objetos
	size_t numObjects;

	if (!(file >> numObjects))
		throw FileFormatError(filename, lineno, "Error al leer el numero de objetos");

	lineno++;

	// Cargar la configuración del nivel.
	// Esto recupera los parámetros que no guardamos.
	loadLevel(savedLevel);

	// Eliminar los objetos que había en la partida actual
	for (SceneObject* obj : sceneObjects)
		delete obj;

	sceneObjects.clear();

	// Restaurar los datos generales guardados.
	currentLevel = savedLevel;
	score = savedScore;
	numArrows = savedArrows;
	timerBallons = savedTimer;

	// Reconstruir los objetos
	for (size_t i = 0; i < numObjects; i++)
	{
		std::string type;

		if (!(file >> type))
			throw FileFormatError(filename, lineno, "Falta el tipo de objeto");

		SceneObject* obj = nullptr;

		try {
			if (type == "ARROW")
				obj = new Arrow(file, this);

			else if (type == "BALLOON")
				obj = new Balloon(file, this, balloonPoints);

			else if (type == "BUTTERFLY")
				obj = new Butterfly(file, this);

			else
				throw FileFormatError(filename, lineno,
					"Tipo de objeto desconocido: " + type);
		}
		catch (const FileFormatError&) {
			throw;
		}
		catch (const std::exception& e) {
			throw FileFormatError(filename, lineno, e.what());
		}

		sceneObjects.push_back(obj);
		lineno++;
	}

	if (file.bad())
		throw FileFormatError(filename, lineno, "Error al leer el archivo");

	gameOver = false;
}

Game::Game()
	: exit(false), currentLevel(1), goal(0), gameOver(false)
{
	textures.fill(nullptr);
	window = nullptr;
	renderer = nullptr;
	playerBow = nullptr;

	try {
		// Carga SDL
		if (!SDL_Init(SDL_INIT_VIDEO))
			throw SDLError("Error al inicializar SDL");

		// Crea la ventana y el renderizador
		if (!SDL_CreateWindowAndRenderer(
			WINDOW_TITLE,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			0,
			&window,
			&renderer))
		{
			throw SDLError("Error al crear la ventana o el renderer");
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
		infoBar = new InfoBar(this);
		// Crea el arco del jugador
		playerBow = new Bow(
			getTexture(BOW),
			{ WINDOW_WIDTH / 10, WINDOW_HEIGHT / 2 },
			{GAME_SCALE, GAME_SCALE},
			this
		);

		createButterflies();
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
	for (SceneObject* obj : sceneObjects)
		delete obj;
	sceneObjects.clear();
	
	delete playerBow;

	delete infoBar;
	infoBar = nullptr;

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

	for (SceneObject* obj : sceneObjects)
	{
		obj->render();
	}
	playerBow->render();

	infoBar->render();

	SDL_RenderPresent(renderer);

	
}

void
Game::update(float delta)
{
	if (gameOver) return;

	auto it = sceneObjects.begin();

	while (it != sceneObjects.end())
	{
		SceneObject* obj = *it;

		if (!obj->update(delta))
		{
			delete obj;
			it = sceneObjects.erase(it);
		}
		else
		{
			++it;
		}
	}

	timerBallons += delta;

	if (timerBallons >= (balloonDelay/1000)) {
		addBalloon();
		timerBallons =0;
	}
	playerBow->update(delta);
	
	// Comprobar si se ha alcanzado el objetivo del nivel
	if (score >= goal) {
		currentLevel++;
		loadLevel(currentLevel);
	}

	// Comprobar si se han agotado las flechas
	if (numArrows <= 0) {
		gameOver = true;
	}
}
 // pulsaciones, actualizaciones, dibujo y espera ms antes de nueva vuelta
void
Game::run()
{
	const Uint32 frameDelay = FRAME_RATE;

	while (!gameOver) {

		Uint32 frameStart = SDL_GetTicks();
		handleEvents();
		update(FRAME_PERIOD/1000.0f);
		render();

		Uint32 frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime)
			SDL_Delay(frameDelay - frameTime);
	}
}

void Game::handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {

		if (event.type == SDL_EVENT_QUIT) {
			exit = true;
		}
		else if (event.type == SDL_EVENT_KEY_DOWN &&
			event.key.key == SDLK_ESCAPE) {
			exit = true;
		}
		else if (event.type == SDL_EVENT_KEY_DOWN &&
			(event.key.mod & SDL_KMOD_CTRL)) {

			if (event.key.key == SDLK_S)
				saveGame();

			else if (event.key.key == SDLK_O)
				loadGame();
		}
		else {
			playerBow->handleEvent(event);
		}
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

	sceneObjects.push_back(
		new Balloon(getTexture(BALLOON), position, {GAME_SCALE, GAME_SCALE}, { 0.0f, -balloonSpeed }, this, color, balloonPoints)
	);
}

void Game::createButterflies()
{
	for (int i = 0; i < numButterflies; i++)
	{
		Point2D<float> position = randomPosition();
		Vector2D<float> velocity = randomVelocity();

		sceneObjects.push_back(
			new Butterfly(getTexture(BUTTERFLY), position, { BUTTERFLY_SCALE, BUTTERFLY_SCALE }, velocity, this)
		);
	}
}

Point2D<float> Game::randomPosition() const
{
	float x = static_cast<float>(
		rand() % (WINDOW_WIDTH - 50/*BUTTERFLY_WIDTH*/)
		);

	float y = static_cast<float>(
		rand() % (WINDOW_HEIGHT - 50/*BUTTERFLY_HEIGHT*/)
		);

	return Point2D<float>(x, y);
}

Vector2D<float> Game::randomVelocity() const
{
	float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159265f;

	float velocity = 50.0f + static_cast<float>(rand()) / RAND_MAX * 50.0f;

	float x = cos(angle) * velocity;
	float y = sin(angle) * velocity;

	return Vector2D<float>(x, y);
}

void
Game::addArrow(const Vector2D<float>& pos, const Vector2D<float>& vel) {
	sceneObjects.push_back(new Arrow(getTexture(ARROW), pos, {GAME_SCALE, GAME_SCALE}, vel, this));
}

 bool
 Game::checkCollision(const Rectangle& rect)
 {
	 for (SceneObject* obj : sceneObjects)
	 {
		 if (obj->canBeHit() && obj->hit(rect))
			 return true;
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
		 throw FileNotFoundError(levelPath);
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
				 throw FileFormatError(
					 levelPath,
					 lineno, "arrows necesita un numero entero"
				 );
			 }

			 if (numArrows < 0) {
					 throw FileFormatError(
						 levelPath,
						 lineno, "arrows no puede ser negativo."
					 );
			 }

			 std::string extra;
			 if (iss >> extra) {
				 throw FileFormatError(
					 levelPath,
					 lineno, "sobran datos despues de arrows."
				 );
			 }
		 }
		 else if (keyword == "baldelay") {
			 if (!(iss >> balloonDelay)) {
				 throw FileFormatError(
					 levelPath,
					 lineno, "baldelay necesita un numero."
				 );
			 }

			 if (balloonDelay <= 0) {
				 throw FileFormatError(
					 levelPath,
					 lineno, "baldelay debe ser positivo.");
			 }

			 std::string extra;
			 if (iss >> extra) {
				 throw FileFormatError(
					 levelPath,
					 lineno, "sobran datos despues de baldelay.");
			 }
		 }
		 else if (keyword == "bgcolor") {
			 int r, g, b;

			 // Comprobar que se han leido los tres componentes
			 if (!(iss >> r >> g >> b)) {
				 throw FileFormatError(
					 levelPath,
					 lineno, "bgcolor necesita tres valores RGB.");
			 }	

			 // Comprobar que cada componente esta entre 0 y 255
			 if (r < 0 || r > 255 ||
				 g < 0 || g > 255 ||
				 b < 0 || b > 255) {
				 throw FileFormatError(
					 levelPath,
					 lineno, "los valores RGB deben estar entre 0 y 255.");
			 }

			 // Comprobar que no hay datos adicionales
			 std::string extra;
			 if (iss >> extra) {
				 throw FileFormatError(
					 levelPath,
					 lineno, "sobran datos despues de bgcolor.");
			 }

			 // Guardar el color de fondo
			 bgR = static_cast<Uint8>(r);
			 bgG = static_cast<Uint8>(g);
			 bgB = static_cast<Uint8>(b);
		 }
		 else if (keyword == "balpoints") {
			 int n;

			 if (!(iss >> n)) {
				 throw FileFormatError(
					 levelPath,
					 lineno, "balpoints necesita un numero entero.");
			 }

			 if (n < 0) {
				 throw FileFormatError(
					 levelPath,
					 lineno, "balpoints no puede ser negativo.");
			 }

			 std::string extra;
			 if (iss >> extra) {
				 throw FileFormatError(
					 levelPath,
					 lineno, "sobran datos despues de balpoints.");
			 }

			 balloonPoints = n;
		 }
		 else if (keyword == "balspeed") {
			 float v;

			 if (!(iss >> v)) {
				 throw FileFormatError(
					 levelPath,
					 lineno, "balspeed necesita un numero.");
			 }

			 if (v <= 0 || !std::isfinite(v)) {
				 throw FileFormatError(
					 levelPath,
					 lineno, "balspeed debe ser positivo.");
			 }

			 std::string extra;
			 if (iss >> extra) {
				 throw FileFormatError(
					 levelPath,
					 lineno, "sobran datos despues de balspeed.");
			 }

			 balloonSpeed = v * 1000;
		 }
		 else if (keyword == "goal") {
			 int n;

			 if (!(iss >> n)) {
				 throw FileFormatError(
					 levelPath,
					 lineno, "goal necesita un numero entero.");
			 }

			 if (n < 0) {
				 throw FileFormatError(
					 levelPath,
					 lineno, "goal no puede ser negativo.");
			 }

			 std::string extra;
			 if (iss >> extra) {
				 throw FileFormatError(
					 levelPath,
					 lineno, "sobran datos despues de goal.");
			 }

			 goal = n;
		 }
		 else if (keyword == "butterflies") {
			 int n;

			 if (!(iss >> n)) {
				 throw FileFormatError(
					 levelPath,
					 lineno, "butterflies necesita un numero entero.");
			 }

			 if (n < 0) {
				 throw FileFormatError(
					 levelPath,
					 lineno, "butterflies no puede ser negativo.");
			 }

			 std::string extra;
			 if (iss >> extra) {
				 throw FileFormatError(
					 levelPath,
					 lineno, "sobran datos despues de butterflies.");
			 }

			 numButterflies = n;
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

 void Game::updateNumArrows()
 {
	 --numArrows;
 }

 void Game::addScore(int points)
 {
	 score += points;
	 if (score < 0) score = 0;
	 std::cout << "Score: " << score << std::endl;
 }

 int Game::getScore()
 {
	 return score;
 }

 int Game::getArrows() const
 {
	 return numArrows;
 }
