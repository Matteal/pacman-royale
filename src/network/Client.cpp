#include "Gateway.h"

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

	m_socket = socket(AF_INET, SOCK_STREAM, 0);       /* création prise */
	if (m_socket < 0)
	{
		perror("socket");
		exit(-1);
	}

	serveur = gethostbyname(serverName); /* recherche adresse serveur */
	if (serveur == NULL)
	{
		perror("gethostbyname");
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
		authentification();
		m_co->setCallback(std::bind(&Client::printMessage, this, std::placeholders::_1));
	}

	void Client::authentification()
	{
		// std::string input="";
		//
		//
		// std::cout<<"Entrez un nom de X caractères (todo: limiter la taille)\n>";
		// std::getline(std::cin, input);
		// m_co->sendMessage(create_message(NEW_CONNECTION, input)); //vérifier la taille du pseudo
		//
		// std::cout<<"@fin de l'authentification\n"<<std::endl;

	}

	void Client::run()
	{
		m_co->startReadAsync();
		std::string input;
		std::cout<<"entrez 'exit' pour quitter"<<std::endl;
		while(m_isActive)
		{
			input="";
			std::cout<<"> ";
			std::getline(std::cin, input); //protège des espaces

			if(isConnectionActive())
			{
				if(input[0]=='!')
				{
					input.erase(input.begin());//supprime le ! du message
					m_co->sendMessage(create_message(INSTRUCTION, input));
				}
				else if(m_isGameLaunched)
				{
					std::cout<<"CLIENT> La Game est lancée" <<std::endl;

					mainloop();
					m_game->setCallback(std::bind(&Client::setInstructionTo, this, std::placeholders::_2));
					//m_game ->Start(CONSOLE);
					mainloop();
					//m_game->run();
					m_isGameLaunched = false;
				}
				else
				{
					m_co->sendMessage(create_message(MESSAGE, input));
				}
			}
			else
			{
				std::cout<<"programme terminé"<<std::endl;
				return;
			}
		}
	}

	void Client::mainloop()
	{
		Renderer *renderer;
		launch aff = SDL;

		// Choisit le renderer à utiliser
		if (aff == CONSOLE)
		renderer = new ConsoleRenderer;
		else if (aff == SDL)
		renderer = new SDLRenderer;

		m_game->initRenderer(renderer);

		std::vector<Pacman*>* pacList = m_game->getPacList();

		direction dir_next;
		bool quit = false;
		while (!quit) // Boucle principale
		{
			m_game->startChrono();

			dir_next = m_game->getPac()->_dirNext; // prevent several requests to be sent
			m_game->getInput(m_game->getPac(), quit, dir_next);
			if(m_game->getPac()->_dirNext != dir_next)
			{
				m_co->sendMessage(create_message(INSTRUCTION, std::to_string(dir_next)));
			}

			mtxHeap.lock();
			while(instructionHeap.size()>0)
			{
			const char* str= instructionHeap.back().c_str();
			vector<int> info;
			int i = 0;
			while(str[i] != '\0')
			{
				if(str[i] != 'O' && (int)(str[i] - '0') < 20)
				{
					cout<<str[i] - '0'<<endl;
					info.push_back(str[i] - 48);
				}
					
				i++;
			}
			cout<<"index = "<<info[1]<<" dir = "<<info[0]<<" truc = "<<info[2]<<endl;
			if(info.size() > 0)
			{
				cout<<"ici "<<info.size()<<endl;
				if(info[0] > 0 && info[1] > 0 && info[1] < pacList->size())
					pacList->at(info[1])->_dirNext = (direction)(info[0]);
			}
			instructionHeap.pop_back();
			}
			mtxHeap.unlock();


			m_game->turn();
			m_game->walk(); // On déplace pacman suivant sa direction
			m_game->actuPacgum();
			//cout<<this->m_game->getPac()->_state<<endl;
			renderer->render(this->m_game->getPac()->getIndex(), FPS);

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
			case MESSAGE:
			std::cout << "Message reçu: " << msg.corps << std::endl;
			break;
			case TEST:
			std::cout << "(LOG DU SERV) " << msg.corps << std::endl;
			break;
			case INSTRUCTION:
			assert(m_isActive); //le programme n'est pas sensé recevoir d'instruction avant que la game aie commencée
			//std::cout << "Instruction>" << msg.corps << std::endl;
			mtxHeap.lock();
			instructionHeap.push_back(msg.corps);
			mtxHeap.unlock();
			//m_game->addInstruction(msg.corps);
			break;
			case NEW_GAME:
			std::cout <<"Le signal de début de partie à été recu, appuie sur entrée pour débloquer" <<std::endl;

			m_game = new Game((int)(msg.corps[1]+128), (int)(msg.corps[2]+128) , stoi(msg.corps.substr(3)));
			m_game->init(2, 0, (int)(msg.corps[0]+128));
			m_isGameLaunched = true;

			break;
			default:
			std::cout << "CLIENT> message de type " << msg.type << " non reconnu :" << msg.corps << std::endl;
			break;
		}
	}
