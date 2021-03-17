#include "Game.h"
#include <iostream>
#include <ctime>
#include <string.h>


#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// Game::Game() : _t(19, 21), _score(0), _lives(3)
// {
// }

Game::Game(int t_width, int t_height, int t_seed) : _t(t_width, t_height, t_seed), Pac(0, 0, 0, 255, 255, 255)
{
      init();
}


void Game::update()
{
    std::cout << *start_time << std::endl;
}


void Game::Start(bool console)
{
    if(console)
    {
        renderConsole();
    }
    else
    {
        //updateSDL();
    }

    end();
}

void Game::init()
{
    _t.generateTerrain(); // genere le terrain
    Pac.setDir(-1); // Donne une direction négative a pacman pour qu'il soit immobile
    Pac.setX(_t.getWidth()/2); //Le place
    Pac.setY(_t.getHeight()/2 - 1);
}

void Game::renderConsole()
{

    WINDOW * w = subwin(stdscr, 10, 10, LINES, COLS); // Créer une fenetre terminal
    initscr(); // initialise la fenetre
    noecho();  // Option fenetre
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE); // Transforme getch en fonction non bloquante
    bool quit = false; // Condition d'arret
    int r; // Retour d'erreur refresh
    refresh(); // Rafraichissement page avant le start
    int ch; // Retour d'erreur get character (getch, renvoie ERR si pas de touches appuyé)


    while(!quit) // Boucle d'initialisation
    {

        clear(); // Nettoie la fenetre

        drawConsole(); // dessinne le terrain

        napms(50); // Attend 50 ms

        r = wrefresh(w); // rafraichie la fenêtre


        if(r == ERR) // Si erreur ...
        {
            cerr<<"ERROR :: REFRESHING WINDOW :"<<strerror(errno)<<endl;
        }

        ch = getch(); // Recupère les input de touches
        if(ch != ERR) // si pas d'erreur (appuie touches) on gère la direction
        {
            inputHandler(ch, quit);
        }
        walk(); // on déplace pacman suivant sa direction
        flushinp(); // reset du buffer de getch pour éviter les input lags

    }
    endwin(); // destruction fenetre
    free(w); // libération fenetre
}
void Game::end()
{
    _t.~Terrain(); // destruction terrain
}

void Game::drawConsole()
{
    char line[_t.getWidth()*2+1]; // definition d'une ligne, *2 pour espacer le terrain
    for(int j = 0; j < _t.getHeight(); j++) // On parcour les colones
    {
        for(int i = 0; i < _t.getWidth() * 2; i++) // on parcour les lignes
        {
            if(i%2 == 0) // si i est pair, on affiche un char du terrain
            {
                if(i/2 == Pac.getIndexX()  && j == Pac.getIndexY()) // si Pacman, on affiche le char O
                {
                    line[i] = 'O';
                }
                else // sinon grille
                {
                    line[i] = _t.getTile(i/2, j);
                }

            }
            else // si pas pair, on affiche un espace
            {
                line[i] = ' ';
            }
        }
        line[_t.getWidth()*2] = '\0'; // on termine la ligne
        mvprintw((LINES / 2) - j + (_t.getWidth() /2), (COLS / 2) - (_t.getWidth()*2 / 2), line); // on affiche la ligne
    }
}

void Game::inputHandler(int input, bool & quit)
{
    switch (input) // on vérifie la touche appuyée
    {
    case 27: // si fonction (toutes touches non charactere = 27 avec curses :/)
        quit = true;
        break;
    case 'z':
        if(canTurnUp()) Pac.setDir(0); // tourne en haut si possible
        break;
    case 's':
        if(canTurnDown()) Pac.setDir(1); // same en bas
        break;
    case 'q':
        if(canTurnLeft()) Pac.setDir(2); // same a gauche
        break;
    case 'd':
        if(canTurnRight()) Pac.setDir(3); // same a droite
        break;
    default:
        break;
    }
}

void Game::walk()
{
    float vitesse = 0.4;
    if(Pac.getIndexX() < 0) // Si sort du tableau a gauche
    {
      if(_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 2, 1) == ' ') Pac.setX(_t.getWidth() - 1); // on tp a droite
    }
    else if (Pac.getIndexX() >= _t.getWidth()) // si sort a droite
    {
        if(_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 3, 1) == ' ') Pac.setX(0); // tp gauche
    }
    if(Pac.getY() < 0) // si sort en bas
    {
        if(_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 1, 1) == ' ') Pac.setY(_t.getHeight() - 1); // tp haut
    }
    else if (Pac.getIndexY() >= _t.getHeight()) // si sort haut
    {
        if(_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 0, 1) == ' ') Pac.setY(0); // tp bas
    }

    switch (Pac.getDir())
    {
    case 0: //si haut est libre, on avance
        if(_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 0, 1) == ' ') Pac.setY(Pac.getY() + vitesse);
        break;

    case 1: //même chose en bas
        if(_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 1, 1) == ' ') Pac.setY(Pac.getY() - vitesse);
        break;

    case 2: // same a gauche
        if(_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 2, 1) == ' ') Pac.setX(Pac.getX() - vitesse);
        break;

    case 3: // de même a droite
        if(_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 3, 1) == ' ') Pac.setX(Pac.getX() + vitesse);
        break;

    default:
        break;
    }
}

bool Game::canTurnUp() // On vérifie que pacman peut tourner en haut
{
    return (_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 0, 1) == ' ');
}

bool Game::canTurnDown() // On vérifie que pacman peut tourner en bas
{
    return (_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 1, 1) == ' ');
}

bool Game::canTurnLeft() // On vérifie que pacman peut tourner a gauche
{
    return (_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 2, 1) == ' ');
}

bool Game::canTurnRight() // On vérifie que pacman peut tourner a droite
{
    return (_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 3, 1) == ' ');
}
