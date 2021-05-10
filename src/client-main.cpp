#include <iostream>
#include "network/Client.h"
#include "game/Game.h"
//sleep
#include <stdio.h>
#include <time.h>
#include <unistd.h>



int main(int argc, char *argv[]){
	string affichage = "";

	// vérification du nombre de paramètres
	if (argc < 2) {
		printf("Usage: %s serveur [SDL/CONSOLE]\n", argv[0]);
		perror("Nombre de paramètres incorrect");
		return -1;
	}
	if(argc >= 3)
		affichage = argv[2];

	// vérification du type d'affichage
	launch lch;
	if(!affichage.compare("SDL") || !affichage.compare("sdl"))
	{
		lch = SDL;
	}else if(!affichage.compare("CONSOLE") ||
		!affichage.compare("console") ||
		!affichage.compare("")
	){
		lch = CONSOLE;
	}else
	{
		printf("Usage: %s serveur [SDL/CONSOLE]\n", argv[0]);
		perror("Le type d'affiche est inconnu");
		return -1;
	}

	// lancement du programme
	Client cli(argv[1]);
	cli.run(lch);

	std::cout<<"programme terminé"<<std::endl;
	return 0;
}
