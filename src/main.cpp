#include <iostream>
#include <unistd.h>
#include <string.h>

using namespace std;

#include "game/Game.h"
#include "game/Renderer.h"


void DoNothing(std::string instruction){std::cout<<instruction<<std::endl;};
int main(int argc, char *argv[])
{
	// init renderer Type
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
	game.init(1, 4, 0); // garder le troisième paramètre à 0
	game.initRenderer(renderer);

	direction dir_next;

	bool quit = false; // Condition d'arrêt
	while (!quit) // Boucle principale
	{
		game.startChrono();

		game.getInput(game.getPac(), quit, dir_next);
		game.getPac()->_dirNext=dir_next;
		renderer->render(0);

		game.turn();
		game.walk(); // On déplace pacman suivant sa direction
		game.actuPacgum();

		//cout<<"Position = "<<game.getPac()->getX()<<" "<<game.getPac()->getY()<<endl;

		game.stopChrono();
	}
	game.Quit();
	delete renderer;

	return EXIT_SUCCESS;
}
