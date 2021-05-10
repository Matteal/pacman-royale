#include "Client.h"

#include <unistd.h> //close
#include <iostream>
// ***************
// * CLIENT SIDE *
// ***************


Client::Client(const char* serverName) : m_co(nullptr), m_isActive(true), m_isGameLaunched(false)
{
	//initialise WinSocks sous windows
	#ifdef _WIN32
		WSADATA WSAData;
		WSAStartup(MAKEWORD(2,2), &WSAData);
	#endif // _WIN32

	struct sockaddr_in addr_serveur;
	struct hostent *serveur;

	m_socket = socket(AF_INET, SOCK_STREAM, 0);       /* création socket */
	if (m_socket < 0)
	{
		perror("socket");
		exit(-1);
	}

	serveur = gethostbyname(serverName); /* recherche adresse serveur */
	if (serveur == NULL)
	{
		perror("Erreur lors de la résolution du nom de l'hôte");
		exit(-1);
	}

	addr_serveur.sin_family = AF_INET;
	addr_serveur.sin_port = htons(PORT);
	addr_serveur.sin_addr = *(struct in_addr *) serveur->h_addr;

	if (connect(m_socket, /* connexion au serveur */
		(struct sockaddr *) &addr_serveur, sizeof addr_serveur) < 0)
	{
		perror("connect");
		exit(-1);
	}

	// création de la connection
	m_co = new connection(m_socket);
	m_co->setCallback(std::bind(&Client::printMessage, this, std::placeholders::_1));
}


void Client::run(launch aff)
{
	m_co->startReadAsync();
	std::string input;
	std::cout<<"CLIENT> Vous êtes bien connectés, veillez attendre que la partie démare"<<std::endl;

	//attend que l'ordre de début de la partie soit lancé
	while(isConnectionActive() && !m_isGameLaunched)
	{
		sleep(1);
	}

	if(m_isGameLaunched)
	{
		std::cout<<"CLIENT> La Game est lancée" <<std::endl;

		m_game->setCallback(std::bind(&Client::setInstructionTo, this, std::placeholders::_2));

		mainloop(aff);

		m_isGameLaunched = false;
		m_isActive = false;

		std::cout<<"CLIENT> partie terminée"<<std::endl;
	}
}

void Client::mainloop(launch aff)
{std::cout<<"main" << aff <<std::endl;
	// initialisation des variables
	std::vector<Pacman*>* pacList = m_game->getPacList();

	direction dir_next;
	bool quit = false;

	// Choisit le renderer à utiliser
	Renderer *renderer;

	if (aff == CONSOLE)
		renderer = new ConsoleRenderer;
	else if (aff == SDL)
		renderer = new SDLRenderer;

	m_game->initRenderer(renderer);

	while (!quit) // Boucle principale
	{
		m_game->startChrono();

		// récupère l'entrée utilisateur
		dir_next = m_game->getPac()->_dirNext;
		m_game->getInput(m_game->getPac(), quit, dir_next);
		if(m_game->getPac()->_dirNext != dir_next)
		{
			m_co->sendMessage(create_message(INSTRUCTION, std::to_string(dir_next)));
		}

		// Traite le tas d'instruction
		mtxHeap.lock();
		while(instructionHeap.size()>0) // 0 - dir 1 - INDEX 2 - STATE 3 - ISSUPER 4 - X 5 - Y 6 - TIMER
		{
			string str = instructionHeap.back();
			vector<string> inf = explode(str, '_');

			if(inf[0].back() == 'M') // M pour mouvement
			{
				pacList->at(stoi(inf[2]))->_dirNext = (direction)(stoi(inf[1]));
				pacList->at(stoi(inf[2]))->setDir((direction)(stoi(inf[1])));
				pacList->at(stoi(inf[2]))->_state = stoi(inf[3]);
				pacList->at(stoi(inf[2]))->setPos(Point(stof(inf[5]), stof(inf[6])));
				pacList->at(stoi(inf[2]))->_isSuper = stoi(inf[4]);
				pacList->at(stoi(inf[2]))->_timer = stoi(inf[7]);
			}
			else if(inf[0].back() == 'S') // S pour superPacgum
			{
				m_game->pacgumList.at(stoi(inf[1])).setSuper(true);
				m_game->_t.setTile(m_game->pacgumList.at(stoi(inf[1])).getIndexX(), m_game->pacgumList.at(stoi(inf[1])).getIndexY(), 'S');
			}

			instructionHeap.pop_back();
		}
		mtxHeap.unlock();

		m_game->walk(); // On déplace pacman suivant sa direction
		m_game->actuPacgum(false);

		renderer->render(m_game->getPac()->getIndex(), FPS);

		// gère la fin de partie
		if(m_game->getPac()->_state !=0)
		{
			if(m_game->getPac()->_timer == FPS*20)
			{
				sleep(2);
				quit = true;
			}
		}

		m_game->stopChrono();
	}

	delete renderer;
}

Client::~Client()
{
	close(m_socket);
	m_socket = -1;

	delete(m_co);
	m_co = nullptr;

	//ferme la bibliothèque WinSock
	#ifdef _WIN32
		WSACleanup();
	#endif // _WIN32
}

bool Client::isConnectionActive()
{
	return m_isActive;
}

void Client::setInstructionTo(std::string instruction)
{
	m_co->sendMessage(create_message(INSTRUCTION, instruction));
}

void Client::printMessage(Message msg)
{
	switch(msg.type)
	{
		case CLOSE_CONNECTION:
			std::cout<<"CLIENT> Vous avez été déconnecté pour la raison suivante : "<< msg.corps << std::endl;
			m_isActive = false;
			exit(3); // met fin au programme
		break;
		case INSTRUCTION:
		if(m_isActive){ //teste si la game est lancée
			mtxHeap.lock();
			instructionHeap.push_back(msg.corps);
			mtxHeap.unlock();
		}
		break;
		case NEW_GAME:
			std::cout <<"Le signal de début de partie à été recu" << std::endl;
			m_game = new Game((int)(msg.corps[0]+128), (int)(msg.corps[1]+128) , stoi(msg.corps.substr(5)));
			m_game->init((int)(msg.corps[3]+128), (int)(msg.corps[4]+128), (int)(msg.corps[2]+128));
			m_isGameLaunched = true;
		break;
		default:
			std::cout << "CLIENT> message de type " << msg.type << " non reconnu :" << msg.corps << std::endl;
		break;
	}
}
