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
		printf("Usage: %s serveur (message)\n", argv[0]);
		perror("nombre de paramètres incorrect");
		return -1;
	}
	if(argc >= 3)
		affichage = argv[2];
		

	Client cli(argv[1]);

	cli.run(affichage);

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
