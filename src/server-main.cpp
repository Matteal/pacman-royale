#include <iostream>
#include "network/Gateway.h"

// sleep
#include <stdio.h>
#include <time.h>
#include <unistd.h>



int main(int argc, char *argv[])
{
	int nbFantome = 0;
	int nbHumain = 0;
	if (argc == 3) 
	{
		nbHumain = (int)argv[1][0] - '0';
		nbFantome = (int)argv[2][0] - '0';
	}
	else if(argc == 2)
	{
		nbHumain = (int)argv[1][0] - '0';
		nbFantome = nbHumain * 4;
		cout<<"Pas de fantome dans la map car param non mis"<<endl;
	}
	else 
	{
		cout<<"veuillez fournir le nombre de joueur en premier parametre et le nombre de fantome en second"<<endl;
		return -1;
	}
	Server srv;
	srv.startListening(nbHumain, nbFantome);//wait_for_connection();
	srv.run();
	std::cout<<"un pti thread est fermé on dirais"<<std::endl;
	return 0;
}
