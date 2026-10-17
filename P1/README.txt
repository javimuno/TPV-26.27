# Material para la práctica 1

Instrucciones
-------------

* Copia el material sobre la plantilla para proyectos SDL que se puede descargar del campus. Elimina antes los assets para los ejercicios del tema 3, ya que no se usarán en la práctica. 

* La carpeta assets de este archivo incluye texturas en la carpeta images y archivos de nivel en el directorio levels.

* La carpeta src (donde ha de quedar incluido todo el código de las prácticas) contiene las implementaciones completas de las clases Texture y Rectangle (aunque está depende de Vector2D) y esqueletos para la función main y las clases Game y Vector2D.

* Agrega los archivos texture.cpp, rectangle.cpp y game.cpp al proyecto de Visual Studio haciendo clic con el botón derecho sobre «Archivos de código fuente» y el menú sobre «Agregar» y «Elemento existente...». Si usas CMake, añade los mismos archivos a la lista de «add_executable(ProyectoSDL ...)» al final del archivo.

* El proyecto no compilará a falta de añadir un par de métodos sencillos a la clase Vector2D. Cuando los añadas, saldrá una pantalla negra que no responde. Sin embargo, cuando completes el bucle del juego con su llamada handleEvents, la ventana responderá y se cerrará cuando se pulse el botón correspondiente.
