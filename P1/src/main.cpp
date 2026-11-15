//
// Tecnología de la Programación de Videojuegos 1
// Facultad de Informática UCM
//
// Plantilla de proyecto con SDL
//
#include <iostream>
#include <string>
#include "GameError.h"
#include <SDL3/SDL_messagebox.h>

#include "game.h"

int main(int argc, char* argv[])
{
	try
	{
		Game().run();
	}
	catch (const GameError& error)
	{
		
		//este bloque es con el simplemessage
		std::cerr << "Error: " << error.what() << '\n';

		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"Error en el Juego",
			error.what(),
			nullptr
		);
		return 1;
	}
	

	

	return 0;
}
