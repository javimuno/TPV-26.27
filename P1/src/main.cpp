//
// Tecnología de la Programación de Videojuegos 1
// Facultad de Informática UCM
//
// Plantilla de proyecto con SDL
//
#include <iostream>
#include <string>

#include "game.h"

int main(int argc, char* argv[])
{
	try
	{
		Game().run();
	}
	catch (const std::string& error)
	{
		std::cout << "Error: " << error << std::endl;
		return 1;
	}
	

	// TODO: manejar excepciones

	return 0;
}
