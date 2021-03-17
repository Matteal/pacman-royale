#ifndef GAME_H
#define GAME_H

#include <ctime>
#include <curses.h>
#include "Terrain.h"
#include "Pacman.h"
class Game
{
private:

    int _score, _lives;
    clock_t * start_time; // Temps au moment du début de la partie
    float _speed; // La vitesse des pacmans et des fantômes dépend du temps écoulé

    bool canTurnUp();
    bool canTurnDown();
    bool canTurnRight();
    bool canTurnLeft();

    bool Up();
    bool down();
    bool right();
    bool left();

public:
    Pacman Pac;
    Terrain _t; //passé en public pour accès direct pendant les tests

    Game(int t_width = 35, int t_height = 35, int t_seed=177013);

    void Start(bool console = true);

    void renderConsole(); // Affichage de l'état du jeu à la console
    void drawConsole();
    void init();   // Initialisation du jeu (chargement de la carte, des contrôles, etc)
    void update(); // Mise à jour de l'état du jeu
    void inputHandler(int input, bool & quit);
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
