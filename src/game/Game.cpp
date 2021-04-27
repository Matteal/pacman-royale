#include "Game.h"
#include <iostream>
#include <string.h>

#include <chrono>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

const float FPS = 30;
const float UPDATEFREQ = ((float)1 / (float)FPS) * 1000.0f;

Game::Game(int t_width, int t_height, int t_seed) : _t(t_width, t_height, t_seed), Pac(nullptr)
{
	_score = 0;
	_superPacgum = 50;
}

Game::~Game()
{
	for(int i = 0; i < pacmanList.size(); i++)
	{
		delete pacmanList[i];
	}
	_t.~Terrain(); // Destruction du terrain

	for(unsigned i = 0; i < pacmanList.size(); i++)
	{
		delete pacmanList[i];
	}
}

// TODO: réparer ca
void Game::init(unsigned pj, unsigned pnj, int numParticipant)
{
	assert(numParticipant<(int)pj || numParticipant == -1);

	_t.generateTerrain(); // Génère le terrain
	generatePacgum();

	nbEntityRemain = nbGhost = 0;

	for(unsigned i = 0; i < pj + pnj; i++)
	{
		bool ghost = false;
		bool player = false;
		if(i >= pj)
		{
			ghost = true;
			player = false;
			nbGhost++;
		}
		else
		{
			ghost = false;
			player = true;
			nbEntityRemain++;
		}
		addPacman(player, ghost);
	}

	if(numParticipant != -1)
	{
		Pac = pacmanList[numParticipant];
		initJoueur();
		Pac->_state = 0;
	}
}
Pacman* Game::getPac()
{
	return Pac;
};

void Game::initRenderer(Renderer* rend)
{
	m_renderer = rend;
	rend->init(&_t, &pacmanList);
}

void Game::getInput(Pacman* Pac, bool& quit, direction& dirNext)
{
	const UserInput input = m_renderer->getInput();

	switch (input)
	{
		case QUIT:
		quit = true;
		break;
		case IDLE:
		break;
		case Z:
		dirNext = UP;
		break;
		case Q:
		dirNext = LEFT;
		break;
		case S:
		dirNext = DOWN;
		break;
		case D:
		dirNext = RIGHT;
		break;
		case PAUSE:
		Pac->_state = 0;
		break;
	}
}

void Game::startChrono()
{
	// Calcule le temps pris par la frame précedente
	deltaT = chrono::duration_cast<chrono::milliseconds>(endT - startT);

	// On redémarre le chrono immédiatement pour être aussi fiable que possible
	startT = chrono::steady_clock::now();

	// Si la mise à jour a été trop rapide, on attend pour garder le rythme
	if (deltaT.count() < UPDATEFREQ)
	{
		napms(UPDATEFREQ - deltaT.count());
	}
}
void Game::stopChrono()
{
	endT = chrono::steady_clock::now();
}

void Game::addInstruction(const string msg)
{
	mtxHeap.lock();
	instructionHeap.push_back(msg);
	mtxHeap.unlock();
}

#pragma region pacman

void Game::initJoueur()
{
	Pac->setDir(UP); // Le Pacman va monter dès l'exécution du programme
	Pac->_dirNext = UP;
	Pac->_state = 0;
	Pac->_timer = 0;
	Pac->_isSuper = false;

	_score = 0;
	Pac->setPlayer(true);
}

void Game::addPacman(bool player, bool ghost)
{
	Pacman *pac = new Pacman;
	pac->setPos(_t.randomPointEmpty());
	pac->setGhost(ghost);
	pac->setPlayer(player);
	int r = rand()%4;
	pac->_state = 0;
	pac->setColor(r);
	pac->setDir(UP);
	pac->_dirNext = UP;
	pacmanList.push_back(pac);

}

// /!\ les instructions ne sont pas envoyées pour les ghosts
void Game::turn()
{
	for (int i = 0; i < (int)pacmanList.size(); i++)
	{
		if (pacmanList[i]->getGhost())
		{
			actuDirGhost(pacmanList[i]);
		}
		if (pacmanList[i]->_dirNext != pacmanList[i]->getDir())
		{

			if (canTurn(pacmanList[i], pacmanList[i]->_dirNext))
			{
				if (pacmanList[i]->getDir() == UP || pacmanList[i]->getDir() == DOWN)
				pacmanList[i]->setY(pacmanList[i]->getIndexY());
				else
				pacmanList[i]->setX(pacmanList[i]->getIndexX());

				pacmanList[i]->setDir(pacmanList[i]->_dirNext);

				if(Pac == nullptr)  // le pacMan n'est pas initialisé: Serveur l'appelle
				{
					const Point point = pacmanList[i]->getPos();

					// construction de l'instruction
					std::string  chaine;
					chaine.push_back(pacmanList[i]->getDir()+'0');
					chaine.push_back(i+'0');
					chaine.push_back((int)point.x-128);
					chaine.push_back((point.x - (int)point.x)*100 -128);
					chaine.push_back((int)point.y-128);
					chaine.push_back((point.y - (int)point.y)*100 -128);

					//envoi de l'instruction
					_instructionCallback(0, chaine);
				}
			}
			//
			// if(pacmanList[i]->getDir() == UP || pacmanList[i]->getDir() == DOWN)
			//     pacmanList[i]->setY(pacmanList[i]->getIndexY());
			// else pacmanList[i]->setX(pacmanList[i]->getIndexX());
			//     pacmanList[i]->setDir(pacmanList[i]->_dirNext);


		}
	}
}

void Game::walk()
{
	float vitesse = 0.4;
	for (int i = 0; i < (int)pacmanList.size(); i++)
	{
		// on gère ici les sorties de tableau pour que le Pacman apparaisse de l'autre côté
		if (pacmanList[i]->getIndexX() < 0)
		pacmanList[i]->setX(_t.getWidth() - 1); // Si sort du tableau a gauche on tp a droite
		else if (pacmanList[i]->getIndexX() >= _t.getWidth())
		pacmanList[i]->setX(0); // Si sort a droite on tp gauche
		if (pacmanList[i]->getY() < 0)
		pacmanList[i]->setY(_t.getHeight() - 1); // si sort en bas on tp haut
		else if (pacmanList[i]->getIndexY() >= _t.getHeight())
		pacmanList[i]->setY(0); // si sort en haut on tp bas
		if (pacmanList[i]->getPlayer())
		{
			for (int j = i + 1; j < (int)pacmanList.size(); j++)
			{
				if (pacmanList[i]->getIndexPos() == pacmanList[j]->getIndexPos() && pacmanList[j]->_state == 0)
				{
					if (pacmanList[i]->_isSuper && !pacmanList[j]->_isSuper)
					{
						pacmanList[j]->_state = -1;
						_score += 100;
						pacmanList[j]->_timer = 100;
					}
					else
					{
						pacmanList[i]->_state = -1;
						pacmanList[i]->_timer = 0;
					}
				}
			}
			vitesse = 0.4;
		}

		else if(pacmanList[i]->getGhost())
		{
			vitesse = 0.2;
			if(pacmanList[i]->_state == -1)
			{
				if(pacmanList[i]->_timer > 0)
					pacmanList[i]->_timer--;
				else
					pacmanList[i]->_state = 0;
			}
		}
		

		switch (pacmanList[i]->getDir())
		{
			case UP: //si haut est libre, on avance
			if (canTurn(pacmanList[i], UP))
			pacmanList[i]->setY(pacmanList[i]->getY() + vitesse);
			break;

			case DOWN: //même chose en bas
			if (canTurn(pacmanList[i], DOWN))
			pacmanList[i]->setY(pacmanList[i]->getY() - vitesse);
			break;

			case LEFT: // same a gauche
			if (canTurn(pacmanList[i], LEFT))
			pacmanList[i]->setX(pacmanList[i]->getX() - vitesse);
			break;

			case RIGHT: // de même a droite
			if (canTurn(pacmanList[i], RIGHT))
			pacmanList[i]->setX(pacmanList[i]->getX() + vitesse);
			break;
		}
	}
}


bool Game::canTurn(Pacman *pac, direction dir)
{
	const char neighborTile = _t.getNeighborTile({(float)pac->getIndexX(), (float)pac->getIndexY()}, dir, 1);

	return (neighborTile == ' ') || (neighborTile == '.') || (neighborTile == 'S');
}

void Game::actuPacgum()
{
	for (int i = 0; i < (int)pacmanList.size(); i++)
	{
		if (!pacmanList[i]->getGhost() && pacmanList[i]->getIndexX() >= 0 && pacmanList[i]->getIndexY() >= 0 && pacmanList[i]->getIndexX() < _t.getWidth() && pacmanList[i]->getIndexY() < _t.getHeight())
		{
			int j = 0;
			while ((pacgumList[j].getIndexX() != pacmanList[i]->getIndexX()) || (pacgumList[j].getIndexY() != pacmanList[i]->getIndexY())) // Cherche la pacgum ou est pacman
			{
				j++;
			}

			if (!pacgumList[j].getState()) //Si elle est vivante, il la mange
			{
				if (pacgumList[j].eat(_superPacgum))
				{
					pacmanList[i]->_isSuper = true; //On la retire des super si s'en était une (d'ou le nombre de super en param)
					pacmanList[i]->_timer = 0;
				}
				_score++;                                                                // On incrémente le score
				_t.setTile(pacgumList[j].getCoord().x, pacgumList[j].getCoord().y, ' '); //On transforme la case en vide
				pacgumEaten.push_back(j);                                                // On rajoute sont id aux pacgums à actu
			}
		}

		pacmanList[i]->actuState(); // Actualise l'état super-pacgum
	}

	for (int i = 0; i < (int)pacgumEaten.size(); i++) // Pour toutes les pacgums mangés
	{

		//if((pacgumList[pacgumEaten[i]].getIndexX() != Pac->getIndexX()) || (pacgumList[pacgumEaten[i]].getIndexY() != Pac->getIndexY()))
		//{   // Si pacman n'est pas dessus
		// Commenté pour l'instant car trop greedy

		if (pacgumList[pacgumEaten[i]].actu(_superPacgum)) // S
		{
			if (pacgumList[pacgumEaten[i]].getSuper()) // Si c'est une super
			{
				_t.setTile(pacgumList[pacgumEaten[i]].getCoord().x, pacgumList[pacgumEaten[i]].getCoord().y, 'S'); // On remplace son char par un S
			}
			else if (!pacgumList[pacgumEaten[i]].getSuper())
			{
				_t.setTile(pacgumList[pacgumEaten[i]].getCoord().x, pacgumList[pacgumEaten[i]].getCoord().y, '.'); // On remet un point sinon
			}

			pacgumEaten.erase(pacgumEaten.begin() + i); // On l'efface du tableau
			i--;
		}
	}
}
#pragma endregion

#pragma region pacgum
void Game::generatePacgum()
{
	for (int i = 0; i < _t.getWidth(); i++)
	{
		for (int j = 0; j < _t.getHeight(); j++)
		{
			if (_t.getTile(i, j) == ' ')
			{
				// _superPacgum est le compte total de super pac-gommes dans le jeu
				// On 'consomme' ici tous les points dans ce compteur pour les placer
				bool isSuper = false;
				if((rand()%1000) < 5 && _superPacgum > 0)
				{
					isSuper = true;
					_superPacgum--;
				}
				pacgumList.push_back(Pacgum(Point(i, j), isSuper));
				if (isSuper)
				_t.setTile(i, j, 'S');
				else
				_t.setTile(i, j, '.');
			}
		}
	}
}

#pragma endregion
#pragma region IA
void Game::actuDirGhost(Pacman *pac)
{
	int r = rand() % 100;
	int tourne = rand() % 100;
	if (tourne < 5 || !canTurn(pac, pac->getDir()))
	{
		switch (pac->getDir())
		{
			case UP:
			if (canTurn(pac, LEFT) && canTurn(pac, RIGHT))
			{
				if (r < 50)
				pac->_dirNext = (LEFT);
			}
			else if (canTurn(pac, LEFT) || canTurn(pac, RIGHT))
			{
				if (canTurn(pac, LEFT))
				pac->_dirNext = (LEFT);
				else
				pac->_dirNext = (RIGHT);
			}
			else if (!canTurn(pac, UP))
			pac->_dirNext = (DOWN);
			break;

			case DOWN:
			if (canTurn(pac, LEFT) && canTurn(pac, RIGHT))
			{
				if (r < 50)
				pac->_dirNext = (LEFT);
			}
			else if (canTurn(pac, LEFT) || canTurn(pac, RIGHT))
			{
				if (canTurn(pac, LEFT))
				pac->_dirNext = (LEFT);
				else
				pac->_dirNext = (RIGHT);
			}
			else if (!canTurn(pac, DOWN))
			pac->_dirNext = (UP);
			break;

			case LEFT:
			if (canTurn(pac, DOWN) && canTurn(pac, UP))
			{
				if (r < 50)
				pac->_dirNext = (DOWN);
			}
			else if (canTurn(pac, DOWN) || canTurn(pac, UP))
			{
				if (canTurn(pac, DOWN))
				pac->_dirNext = (DOWN);
				else
				pac->_dirNext = (UP);
			}
			else if (!canTurn(pac, LEFT))
			pac->_dirNext = (RIGHT);
			break;

			case RIGHT:
			if (canTurn(pac, DOWN) && canTurn(pac, UP))
			{
				if (r < 50)
				pac->_dirNext = (DOWN);
			}
			else if (canTurn(pac, DOWN) || canTurn(pac, UP))
			{
				if (canTurn(pac, DOWN))
				pac->_dirNext = (DOWN);
				else
				pac->_dirNext = (UP);
			}
			else if (!canTurn(pac, RIGHT))
			pac->_dirNext = (LEFT);
			break;

			default:
			break;
		}
	}
}
#pragma endregion
