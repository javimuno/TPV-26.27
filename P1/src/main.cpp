//
// Tecnología de la Programación de Videojuegos 1
// Facultad de Informática UCM
//
// Plantilla de proyecto con SDL
//

//#define _CRTDBG_MAP_ALLOC
//#include <cstdlib>
//#include <crtdbg.h>

#include "game.h"

int main(int argc, char* argv[])
{
    // Comprobar fugas de memoria al terminar el programa
    /*_CrtSetDbgFlag(
        _CRTDBG_ALLOC_MEM_DF |
        _CRTDBG_LEAK_CHECK_DF
    );*/

    try {
        Game game;
        game.run();
    }
    catch (const std::string& error) {
        SDL_Log("Error: %s", error.c_str());
        return 1;
    }

    return 0;
}
