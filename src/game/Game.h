#ifndef GAME_H
#define GAME_H

#include <ctime>
//#include <curses.h>
#include <mutex>
#include <functional>

#include "Terrain.h"
#include "Pacman.h"
#include "Pacgum.h"

#include "Renderer.h"

enum launch{CONSOLE, SDL};

const float FPS = 60;
const float UPDATEFREQ = ((float)1 / (float)FPS) * 1000.0f;

class Game
{
public:
	Game(int t_width = 50, int t_height = 50, int t_seed=177013);
	~Game();

	void init(unsigned pj, unsigned pnj, int numParticipant = -1);   // Initialisation du jeu (chargement de la carte, des contrôles, etc)
	void initRenderer(Renderer* rend);

	void addInstruction(const string msg);

	void setCallback(std::function<void(int idJoueur, std::string message)> _callback) {_instructionCallback = _callback;};

	void addPacman(int i, bool player, bool ghost);

	Pacman* getPac();
	std::vector<Pacman*>* getPacList() {return &pacmanList;}
	vector<Pacgum> pacgumList;

	void getInput(Pacman* Pac, bool& quit, direction& dirNext);
	void turn();
	void walk();
	void actuPacgum(bool generatePacgum);
	void Quit();

	void startChrono();
	void stopChrono();

private:
	Terrain _t; //passé en public pour accès direct pendant les tests
	Pacman* Pac;

	int _score,  _superPacgum;
	float _speed; // La vitesse des pacmans et des fantômes dépend du temps écoulé
	
	std::vector<Pacman*> pacmanList;
	vector<int> pacgumEaten;

	int nbEntityRemain = 0;
	int nbGhost;

	std::function<void(int idJoueur, std::string message)> _instructionCallback;

	vector<string> instructionHeap;
	mutex mtxHeap;

	Renderer* m_renderer;

	bool canTurn(Pacman*, direction);


	void generatePacgum();

	void actuDirGhost(Pacman * pac);
	void initJoueur();

	// Ces deux variables serviront à calculer l'écart entre deux frames
	// Le calcul de la différence servira à maintenir 60 FPS constants
	chrono::_V2::steady_clock::time_point startT, endT;
	std::chrono::milliseconds deltaT;
};



#endif //GAME_H
