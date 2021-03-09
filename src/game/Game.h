#ifndef GAME_H
#define GAME_H

#include <ctime>
#include <curses.h>
#include "Terrain.h"
#include "Pacman.h"
class Game
{
private:
    Terrain _t;
    int _score, _lives;
    time_t *start_time; // Temps au moment du début de la partie
    float _speed; // La vitesse des pacmans et des fantômes dépend du temps écoulé

public:
    Pacman Pac;

    Game();
    void Start(bool console = true);

    void renderConsole(); // Affichage de l'état du jeu à la console
    void drawConsole(int, int) const; 
    void init();   // Initialisation du jeu (chargement de la carte, des contrôles, etc)
    void update(); // Mise à jour de l'état du jeu

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
