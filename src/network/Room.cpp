#include "Room.h"
#include <iostream>
#include <string>
#include <functional>
#include <algorithm>

// sleep
#include <stdio.h>
#include <time.h>
#include <unistd.h>


Room::Room(int nbHumain, int nbF) : m_game(nullptr), isGameLaunched(false), limite_joueur(nbHumain), nbFantome(nbF)
{
	std::cout<<"une room a été crée!"<<std::endl;
}

Room::~Room()
{
	std::cout<< "ROOM> Fermeture" << std::endl;
	sendAll(create_message(CLOSE_CONNECTION, "Fermeture de la room"));
	delete m_game;
}

void Room::addConnection(connection* co) //TODO ajouter un utilisateur (dérivé de connection)
{
	// s'assure que l'on ne dépasse pas la limite de joueur authorisé

	inscription.lock();

		if(isGameLaunched)
		{
			co->sendMessage(create_message(CLOSE_CONNECTION, "La partie a commencée ! Attendez la prochaine :)"));
			inscription.unlock();
			return;
		}


		// attache la connexion entrante à la liste
		Session s;
		s.co = co;
		s.id = -1;

		mtxList.lock();
		m_list.push_back(s);
		mtxList.unlock();

		co->setCallback(std::bind(&Room::receiveMessage, this, std::placeholders::_1, co));
		co->startReadAsync();



		sendAll(create_message(TEST, "Un nouvel utilisateur est arrive"));
		std::cout << "ROOM> Nombre de connexions actives: " << m_list.size() << std::endl;

		//démare la partie quand le nombre de joueur est atteint
		if(m_list.size()==limite_joueur)
		isGameLaunched = true;

	inscription.unlock();


}

void Room::sendAll(Message message)
{
	for(int i=0; i<(int)m_list.size(); i++)
	{
		m_list[i].co->sendMessage(message);
	}
}

void Room::sendInstructionTo(int idJoueur, std::string message)
{
	// for(unsigned i = 0; i < m_list.size(); i++)
	// {
	//   if(m_list[i].id == idJoueur) // recherche du bon joueur
	//   {std::cout<<"#########"<<std::endl;
	//     m_list[i].co->sendMessage(create_message(INSTRUCTION, message));
	//   }
	// }
	sendAll(create_message(INSTRUCTION, message));
}


void Room::receiveMessage(Message msg, connection* co)
{
	switch(msg.type)
	{
		case MESSAGE:
		sendAll(msg);
		break;
		case TEST:
		std::cout<<"ceci est un test I guess.."<<std::endl;
		break;
		case INSTRUCTION:
		assert(isGameLaunched); // On est pas sensé avoir d'instruction tant que la partie n'as pas commencée
		for(unsigned i = 0; i < m_list.size(); i++)
		{
			if(m_list[i].co == co)
			{
				mtxHeap.lock();
				instructionHeap.push_back(msg.corps + to_string(i));
				mtxHeap.unlock();
				break;
			}
		}
		break;
		case CLOSE_CONNECTION: //cherche la connection et la ferme
		mtxList.lock();
		for(unsigned i = 0; i < m_list.size(); i++)
		{
			if(m_list[i].co == co)
			{
				m_list.erase(m_list.begin()+i);
				std::cout << "l'utilisateur n°" << i << " c'est déconnecté" << std::endl;
				std::cout<<"Nombre de connexions actives: "<<m_list.size()<<std::endl;
				break;
			}
		}
		mtxList.unlock();
		break;
		default:
 			perror("ROOM> Message de type inconnu reçu");
		break;
	}
	//if(msg.type!=CLOSE_CONNECTION)
	//  print_message(msg);
}



void Room::run()
{
	while(!isGameLaunched) // on attend qu'il y aie assez de joueurs pour lancer la partie
	{
		sleep(1);
	}
	std::cout<<"ROOM> La partie va commencer!"<<std::endl;

	//création de la Game
	int tailleX = 50;
	int tailleY = 50;
	int seed = 42;//time(0); //time permet de générer une seed en fonction de l'heure
	m_game = new Game(tailleX, tailleY, seed);
	m_game->init(limite_joueur, nbFantome, -1);

	mtxList.lock();
	for(char i = 0; (unsigned)i < m_list.size(); i++)
	{
		std::string msgNewGame;
		msgNewGame.push_back(tailleX-128); //taille_terrain_x
		msgNewGame.push_back(tailleY-128); //taille_terrain_y
		msgNewGame.push_back(i-128);  //Numéro joueur
		msgNewGame.push_back(limite_joueur-128);  //Nombre joueur
		msgNewGame.push_back(nbFantome-128);  //Nombre fantômes

		m_list[i].id = i;
		m_list[i].co->sendMessage(create_message(NEW_GAME, msgNewGame + to_string(seed)));
	}
	mtxList.unlock();

	mainloop();
}

void Room::mainloop()
{
	Renderer* renderer = new ConsoleRenderer;
	m_game->initRenderer(renderer);

	m_game->setCallback(std::bind(&Room::sendInstructionTo, this, std::placeholders::_1, std::placeholders::_2));
	std::vector<Pacman*>* pacList = m_game->getPacList();

	direction dir_next;
	bool quit = false; // Condition d'arrêt
	while (!quit || m_list.size()==0) // Boucle principale
	{
		m_game->startChrono();
		renderer->render(-1, FPS);
		m_game->getInput(nullptr, quit, dir_next);
		while(instructionHeap.size()>0)
		{
			string str= instructionHeap.back();

			int info[2] = {str.at(0) - 48, str.back() - 48};
			if(info[0] < 4 && info[1] < pacList->size())
			{
				pacList->at(info[1])->_dirNext = (direction)(info[0]);
			}
			instructionHeap.pop_back();
		}
		m_game->turn();
		m_game->walk(); // On déplace pacman suivant sa direction
		m_game->actuPacgum(true);

		mtxList.lock();
		for(int i = 0; i<m_list.size(); i++)
		{
			if(pacList->at(m_list[i].id)->_state != 0 && pacList->at(m_list[i].id)->_timer == FPS*20)
			{
				m_list[i].co->sendMessage(create_message(CLOSE_CONNECTION, "Fin de partie"));
			}
		}
		mtxList.unlock();

		m_game->stopChrono();
	}

	delete renderer;
}
//
// void Game::mainloopServer()
// {
//   bool quit  = false;
//   ConsoleRenderer aff;// = new ConsoleRenderer;
//   aff.init(&_t, &pacmanList);
//
//
//   while (!quit) // Boucle d'initialisation
//   {
//
//     // // Calcule le temps pris par la frame précedente
//     // delta = chrono::duration_cast<chrono::milliseconds>(end - start);
//     //
//     // // On redémarre le chrono immédiatement pour être aussi fiable que possible
//     // start = chrono::steady_clock::now();
//
//
//     // traitement des instructions
//     aff.render(0);
//     UserInput input = aff.getInput();
//
//     mtxHeap.lock();
// 	if(instructionHeap.size()>0)
// 	{
// 		const char* str= instructionHeap.back().c_str();
//
// 		std::cout<<std::endl;
// 		pacmanList[str[1] - '0']->_dirNext = (direction)(str[0] - '0');
// 		if((direction)(str[0] - '0')==UP)
// 			std::cout<<"UP"<<std::endl;
// 		//_instructionCallback(0, instructionHeap[0]);
// 		instructionHeap.pop_back();
// 	}
//     mtxHeap.unlock();
//
//     turn();
//     walk();
//     actuPacgum();
//     napms(20); // Attend 50 ms pour la forme
//     flushinp();
//
//   }
// }
