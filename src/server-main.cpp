#include <iostream>
#include "network/Server.h"


int main(int argc, char *argv[])
{
	// vérification du nombre de paramètre
	if (argc != 3)
	{
		printf("Usage: %s (nb joueur) (nb fantôme)\n", argv[0]);
		perror("Nombre de paramètres incorrect");
		return -1;
	}

	// traitement des paramètres
	int nbFantome = 0;
	int nbJoueur = 0;

	nbJoueur = atoi(argv[1]);
	nbFantome = atoi(argv[2]);

	// lancement du programme
	Server srv;
	srv.startListening(nbJoueur, nbFantome);//wait_for_connection();
	srv.run();

	std::cout<<"Fermeture du serveur"<<std::endl;
	return 0;
}
