#ifndef GAME_H
#define GAME_H

#include <ctime>
#include <curses.h>
#include <mutex>
#include <functional>

#include "Terrain.h"
#include "Pacman.h"
#include "Pacgum.h"


enum launch{CONSOLE, SDL};

class Game
{
private:

    clock_t * start_time; // Temps au moment du début de la partie
    int _score, _lives, _superPacgum;
    float _speed; // La vitesse des pacmans et des fantômes dépend du temps écoulé
    vector<Pacgum> pacgumList;
    std::vector<Pacman*> pacmanList;
    vector<int> pacgumEaten;

    std::function<void(int idJoueur, std::string message)> _instructionCallback;

    vector<string> instructionHeap;
    mutex mtxHeap;

    bool canTurn(Pacman*, direction);

    void generatePacgum();
    void actuPacgum();

    void turn();

public:

    Pacman Pac;
    Terrain _t; //passé en public pour accès direct pendant les tests

    Game(int t_width = 35, int t_height = 35, int t_seed=177013);

    void Start(enum launch);

    void mainloop(enum launch); // Affichage de l'état du jeu à la console
    void mainloopServer(); //server-side only

    void attributeConnection(); //server-side only

    void addInstruction(const string msg);

    void setCallback(std::function<void(int idJoueur, std::string message)> _callback) {_instructionCallback = _callback;};

    void init();   // Initialisation du jeu (chargement de la carte, des contrôles, etc)
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
    
};



#endif //GAME_H
