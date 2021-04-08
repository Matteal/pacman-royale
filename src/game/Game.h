#ifndef GAME_H
#define GAME_H

#include <ctime>
#include <curses.h>
#include <mutex>
#include <functional>

#include "Terrain.h"
#include "Pacman.h"
#include "Pacgum.h"

#include "Renderer.h"

enum launch{CONSOLE, SDL};


class Game
{
private:
    //Terrain _t;
    clock_t * start_time; // Temps au moment du début de la partie
    int _score, _lives, _superPacgum;
    float _speed; // La vitesse des pacmans et des fantômes dépend du temps écoulé
    vector<Pacgum> pacgumList;
    std::vector<Pacman*> pacmanList;
    vector<int> pacgumEaten;

    std::function<void(int idJoueur, std::string message)> _instructionCallback;

    vector<string> instructionHeap;
    mutex mtxHeap;
    int nbEntityRemain = 0;
    int nbGhost;

    bool canTurn(Pacman*, direction);

    void generatePacgum();

    void actuDirGhost(Pacman * pac);
    void initJoueur();

    // Ces deux variables serviront à calculer l'écart entre deux frames et à maintenir 60 FPS constants
    chrono::_V2::steady_clock::time_point startT, endT;
    std::chrono::milliseconds deltaT;

public:
	
    Terrain _t; //passé en public pour accès direct pendant les tests
    Pacman* Pac;


    Game(int t_width = 50, int t_height = 50, int t_seed=177013);

    void Start(enum launch);

    void mainloop(enum launch); // Affichage de l'état du jeu à la console
    void mainloopServer(); //server-side only

    void attributeConnection(); //server-side only

    void addInstruction(const string msg);

    void setCallback(std::function<void(int idJoueur, std::string message)> _callback) {_instructionCallback = _callback;};

    void init(unsigned pj, unsigned pnj, int numParticipant = -1);   // Initialisation du jeu (chargement de la carte, des contrôles, etc)
    void update(); // Mise à jour de l'état du jeu
    void walk();

    void end(); // Fin de la partie (affichage du score)

    void set_map(Terrain); // Changement de carte

    int get_lives();
    int get_score();
    float get_speed();

    void set_lives(int);
    void set_score(int);
    void set_speed(float);

    void addPacman(bool ghost);

    void initRenderer(Renderer* rend);
    Renderer* m_renderer;
    Pacman* getPac();
    void getInput(Pacman* Pac, bool& quit, direction& dirNext);
    void turn();
    void actuPacgum();

    void startChrono();
    void stopChrono();
};



#endif //GAME_H
