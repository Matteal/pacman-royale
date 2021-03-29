#ifndef GAME_H
#define GAME_H

#include <ctime>
#include <curses.h>
#include "Terrain.h"
#include "Pacman.h"
#include "Pacgum.h"

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

    int nbEntityRemain = 0;
    int nbGhost;

    bool canTurn(Pacman*, direction);

    void generatePacgum();
    void actuPacgum();
    void actuDirGhost(Pacman * pac);
    void initJoueur();
    void turn();

public:

    Pacman Pac;
    Terrain _t; //passé en public pour accès direct pendant les tests

    Game(int t_width = 50, int t_height = 50, int t_seed=177013);

    void Start(enum launch);

    void mainloop(enum launch); // Affichage de l'état du jeu à la console
    void mainloopServer(); //server-side only

    void attributeConnection(); //server-side only

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

    void addPacman(bool ghost);
};



#endif //GAME_H
