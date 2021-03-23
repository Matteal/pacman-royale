#ifndef GAME_H
#define GAME_H

#include <ctime>
#include <curses.h>
#include "Terrain.h"
#include "Pacman.h"
#include "Pacgum.h"
class Game
{
private:
    Terrain _t;
    int _score, _lives, _superPacgum;
    clock_t * start_time; // Temps au moment du début de la partie
    float _speed; // La vitesse des pacmans et des fantômes dépend du temps écoulé
    vector<Pacgum> pacgumList;
    vector<int> pacgumEated;

    bool canTurn(direction);

    void generatePacgum();
    void actuPacgum();

    void turn();

public:
    Pacman Pac;

    Game();
    void Start(bool console = true);

    void mainloop(); // Affichage de l'état du jeu à la console

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
