#include "Game.h"
#include <iostream>
#include <ctime>
#include <string.h>


#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "Renderer.h"


Game::Game() : _t(34,34,177013), Pac(Point(0, 0), 0, 0, 255, 255, 255)
{
}

void Game::update()
{
    std::cout << *start_time << std::endl;
}


void Game::Start(bool console)
{
  mainloop();
  end();
}

void Game::init()
{
    _t.generateTerrain(); // genere le terrain
    Pac.setDir(-1); // Donne une direction négative a pacman pour qu'il soit immobile
    Pac._dirNext = -1;
    Pac.setX(_t.getWidth()/2 - 1); //Le place
    Pac.setY(_t.getHeight()/2 - 1);
}

void Game::mainloop()
{
  Renderer* renderer;

  // choisi le renderer à utiliser
  if(true) // vrai si console, faux si SDL
  {
    ConsoleRenderer console_renderer;
    renderer=&console_renderer;
  }else
  {
    SDLRenderer SDL_renderer;
    renderer=&SDL_renderer;
  }

  // Initialisation du renderer
  std::vector<Pacman*> tableauPacman;
  tableauPacman.push_back(&Pac);

  renderer->init(&_t, &tableauPacman);


  //début de la boucle
  bool quit = false; // Condition d'arret

    while(!quit) // Boucle d'initialisation
    {

        renderer->render();

        // Retour d'erreur get character
        char input = renderer->getInput();
        if(input != ERR) // si pas d'erreur (appuie touches) on gère la direction
        {
            inputHandler(input, quit);
        }
        turn();
        walk(); // on déplace pacman suivant sa direction

        cout<<Pac._dirNext<<endl;
    }

}
void Game::end()
{
    _t.~Terrain(); // destruction terrain
}

void Game::drawConsole()
{
}

void Game::inputHandler(int input, bool & quit)
{
    switch (input) // on vérifie la touche appuyée
    {
    case 27: // si fonction (toutes touches non charactere = 27 avec curses :/)
        quit = true;
        break;
    case 'z':
        Pac._dirNext = 0;
        break;
    case 's':
        Pac._dirNext = 1;
        break;
    case 'q':
        Pac._dirNext = 2;
        break;
    case 'd':
        Pac._dirNext = 3;
        break;
    default:
        break;
    }
}

void Game::turn()
{
    if(Pac._dirNext != -1)
    {
        switch (Pac._dirNext) // on vérifie la touche appuyée
         {
        case 0:
            if(canTurnUp())
            {
                Pac.setDir(0); // tourne en haut si possible
                Pac._dirNext = -1;
            }

            break;
        case 1:
            if(canTurnDown())
            {
                Pac.setDir(1); // same en bas
                Pac._dirNext = -1;
            }

            break;
        case 2:
            if(canTurnLeft())
            {
                Pac.setDir(2); // same a gauche
                Pac._dirNext = -1;
            }

            break;
        case 3:
            if(canTurnRight())
            {
                Pac.setDir(3); // same a droite
                Pac._dirNext = -1;
            }

            break;
        default:
            break;
         }


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
