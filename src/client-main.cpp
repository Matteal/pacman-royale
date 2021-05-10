#include <iostream>
#include "network/Gateway.h"
#include "game/Game.h"
//sleep
#include <stdio.h>
#include <time.h>
#include <unistd.h>



int main(int argc, char *argv[]){
	//usage
	string affichage = "";
	if (argc < 2) {
		printf("Usage: %s serveur [SDL/CONSOLE]\n", argv[0]);
		perror("le nombre de paramètres incorrect");
		return -1;
	}
	if(argc >= 3)
		affichage = argv[2];

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

std::cout<<"main"<<std::endl;
	Client cli(argv[1]);

	cli.run(lch);

	// [async chat]
	// cli.m_co->startReadAsync();
	// std::string input;
	// std::cout<<"entrez 'exit' pour quitter"<<std::endl;
	// while(input != "exit")
	// {
	//   input="";
	//   std::cout<<"> ";
	//   std::getline(std::cin, input); //protège des espaces
	//
	//   if(cli.isConnectionActive())
	//   {
	//     if(input[0]=='!')
	//     {
	//       input.erase(input.begin());//supprime le ! du message
	//       cli.m_co->sendMessage(create_message(INSTRUCTION, input));
	//     }
	//
	//     else
	//       cli.m_co->sendMessage(create_message(MESSAGE, input));
	//   }
	//   else
	//   {
	//     std::cout<<"programme terminé"<<std::endl;
	//     return 0;
	//   }
	// }
	// [!async chat]



	std::cout<<"programme terminé"<<std::endl;
	return 0;
}
