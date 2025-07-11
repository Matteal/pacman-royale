#include <iostream>
#include <unistd.h>
#include <string.h>


#include "game/Game.h"
#include "game/Renderer.h"


int main(int argc, char *argv[])
{
	// init type d'affichage
	Renderer* renderer = nullptr;

	if(argc == 1 || strcmp(argv[1], "console") == 0)
	{
		cout<<"Affichage console enclanché"<<endl;
		renderer = new ConsoleRenderer;
	}
	else if(strcmp(argv[1], "SDL") == 0)
	{
		cout<<"Affichage SDL Enclanché"<<endl;
		renderer = new SDLRenderer;
	}
	else
	{
		cout<<"Argument invalide, <<console>> pour le terminal et <<SDL>> pour la SDL, ARRET"<<endl;
		return EXIT_FAILURE;
	}

	//init Game object
	Game game(50, 50, 131489);
	game.init(2, 15, 0); // le troisième paramètre est à 0
	game.initRenderer(renderer);

	direction dir_next;

	bool quit = false; // Condition d'arrêt
	while (!quit) // Boucle principale
	{
		game.startChrono();

		game.getInput(game.getPac(), quit, dir_next);
		game.getPac()->_dirNext=dir_next;
		renderer->render(0, FPS);

		// maj position
		game.turn();
		game.walk();
		game.actuPacgum(true, true);

		game.stopChrono();
	}

	delete renderer;

	return EXIT_SUCCESS;
}
