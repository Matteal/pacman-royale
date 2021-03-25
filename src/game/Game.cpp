#include "Game.h"
#include <iostream>
#include <ctime>
#include <string.h>


#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "Renderer.h"


Game::Game() : _t(34,34,177013), Pac()
{
    _score = 0;
    _superPacgum = 5;
}

void Game::update()
{
    std::cout << *start_time << std::endl;
}


void Game::Start(bool console)
{
    init();
    mainloop();
    end();
}

void Game::init()
{
    _t.generateTerrain(); // genere le terrain
    generatePacgum();
    Pac.setDir(UP); // Le Pacman va monter dès l'exécution du programme
    Pac._dirNext = UP;
    Pac.setX(_t.getWidth()/2 - 1); //Le place
    Pac.setY(_t.getHeight()/2);
    pacmanList.push_back(&Pac);
    pacmanList.push_back(new Pacman);
    pacmanList[1]->setX(1);

    pacmanList.push_back(new Pacman);
    pacmanList[2]->setX(9);
}

void Game::mainloop()
{
    Renderer* renderer;

    // choisi le renderer à utiliser
    bool isTerminal = true;
    if(isTerminal) renderer= new ConsoleRenderer;
    else renderer = new SDLRenderer;



    // Initialisation du renderer
    renderer->init(&_t, &pacmanList);


    //début de la boucle
    bool quit = false; // Condition d'arret

    while(!quit) // Boucle d'initialisation
    {
        renderer->render();

        // Récupération des entrées utilisateur
        UserInput input = renderer->getInput();

        switch(input)
        {
          case QUIT:
            quit = true;
            break;
          case IDLE:
            break;
          case Z:
            Pac._dirNext = UP;
            pacmanList[1]->_dirNext = UP;
            pacmanList[2]->_dirNext = UP;
            break;
          case Q:
            Pac._dirNext = LEFT;
            pacmanList[1]->_dirNext = LEFT;
            pacmanList[2]->_dirNext = LEFT;
            break;
          case S:
            Pac._dirNext = DOWN;
            pacmanList[1]->_dirNext = DOWN;
            pacmanList[2]->_dirNext = DOWN;
            break;
          case D:
            Pac._dirNext = RIGHT;
            pacmanList[1]->_dirNext = RIGHT;
            pacmanList[2]->_dirNext = RIGHT;
            break;
        }

      turn();
      cout<<"Timer = "<<Pac._timer<<" isSuper = "<<Pac._isSuper<<endl;
      walk(); // on déplace pacman suivant sa direction
      actuPacgum();

      flushinp();
    }

    delete renderer;
}

void Game::end()
{
    _t.~Terrain(); // destruction terrain
}

#pragma region pacman

void Game::turn()
{
  for(int i=0; i<(int)pacmanList.size(); i++)
  {
    if(pacmanList[i]->_dirNext != pacmanList[i]->getDir())
    {
      if(canTurn(pacmanList[i], pacmanList[i]->_dirNext))
      {
        pacmanList[i]->setDir(pacmanList[i]->_dirNext);
      }
    }
  }
}

void Game::walk()
{
  for(int i=0; i<(int)pacmanList.size(); i++)
  {
    // on gère ici les sorties de tableau pour que le Pacman apparaisse de l'autre côté
    if(pacmanList[i]->getIndexX() < 0) // Si sort du tableau a gauche
    {
      if(canTurn(pacmanList[i], LEFT)) pacmanList[i]->setX(_t.getWidth() - 1); // on tp a droite
    }
    else if (pacmanList[i]->getIndexX() >= _t.getWidth()) // si sort a droite
    {
        if(canTurn(pacmanList[i], RIGHT)) pacmanList[i]->setX(0); // tp gauche
    }
    if(pacmanList[i]->getY() < 0) // si sort en bas
    {
        if(canTurn(pacmanList[i], DOWN)) pacmanList[i]->setY(_t.getHeight() - 1); // tp haut
    }
    else if (pacmanList[i]->getIndexY() >= _t.getHeight()) // si sort haut
    {
        if(canTurn(pacmanList[i], UP)) pacmanList[i]->setY(0); // tp bas
    }

    float vitesse = 0.4;
    switch (pacmanList[i]->getDir())
    {
    case UP: //si haut est libre, on avance
        if(canTurn(pacmanList[i], UP)) pacmanList[i]->setY(pacmanList[i]->getY() + vitesse);
        break;

    case DOWN: //même chose en bas
        if(canTurn(pacmanList[i], DOWN)) pacmanList[i]->setY(pacmanList[i]->getY() - vitesse);
        break;

    case LEFT: // same a gauche
        if(canTurn(pacmanList[i], LEFT)) pacmanList[i]->setX(pacmanList[i]->getX() - vitesse);
        break;

    case RIGHT: // de même a droite
        if(canTurn(pacmanList[i], RIGHT)) pacmanList[i]->setX(pacmanList[i]->getX() + vitesse);
        break;
    }
  }
}

bool Game::canTurn(Pacman* pac, direction dir)
{
  return (_t.getNeighborTile({(float)pac->getIndexX(), (float)pac->getIndexY()}, dir, 1) != '#');
}

#pragma endregion
#pragma region pacgum
void Game::generatePacgum()
{
    for(int i = 0; i < _t.getWidth(); i++)
    {
        for(int j = 0; j < _t.getHeight(); j++)
        {
            if(_t.getTile(i, j) == ' ')
            {
                bool isSuper = false;
                if((rand()%100) < 1 && _superPacgum > 0)
                {
                    cout<<"X : "<<i<<" Y : "<<j<<endl;
                    isSuper = true;
                    _superPacgum--;
                }
                pacgumList.push_back(Pacgum(Point(i, j), isSuper));
                if(isSuper) _t.setTile(i, j, 'S');
                else _t.setTile(i, j, '.');
            }
        }
    }
}

void Game::actuPacgum()
{
    for(int i=0; i<(int)pacmanList.size(); i++)
    {
        if(pacmanList[i]->getX() >= 0 && pacmanList[i]->getY() >= 0 && pacmanList[i]->getX() < _t.getWidth() - 1 && pacmanList[i]->getY() < _t.getHeight() - 1)
        {
            int j = 0;

            while((pacgumList[j].getIndexX() != pacmanList[i]->getIndexX()) || (pacgumList[j].getIndexY() != pacmanList[i]->getIndexY())) // Cherche la pacgum ou est pacman
            {
                j++;
            }

            if(!pacgumList[j].getState()) //Si elle est vivante, il la mange
            {
                if(pacgumList[j].eat(_superPacgum))
                {
                    pacmanList[i]->_isSuper = true; //On la retire des super si s'en était une (d'ou le nombre de super en param)
                    pacmanList[i]->_timer = 0;
                }
                _score++; // On incrémente le score
                _t.setTile(pacgumList[j].getCoord().x, pacgumList[j].getCoord().y, ' '); //On transforme la case en vide
                pacgumEated.push_back(j); // On rajoute sont id aux pacgums à actu
            }
        }

        pacmanList[i]->actuState(); // Actualise l'état super-pacgum
    }


    for(int i = 0; i < (int)pacgumEated.size(); i++) // Pour toutes les pacgums mangés
    {

        //if((pacgumList[pacgumEated[i]].getIndexX() != Pac.getIndexX()) || (pacgumList[pacgumEated[i]].getIndexY() != Pac.getIndexY()))
        //{   // Si pacman n'est pas dessus
        // commenté pour l'instant car trop greedy

            if(pacgumList[pacgumEated[i]].actu(_superPacgum)) //on l'actualise
            {
                if(pacgumList[pacgumEated[i]].getSuper()) //Si c'est une super
                {
                    _t.setTile(pacgumList[pacgumEated[i]].getCoord().x, pacgumList[pacgumEated[i]].getCoord().y, 'S'); // On remplace son char par un S
                }
                else if(!pacgumList[pacgumEated[i]].getSuper())
                {
                    _t.setTile(pacgumList[pacgumEated[i]].getCoord().x, pacgumList[pacgumEated[i]].getCoord().y, '.'); // On remet un point sinon
                }

                pacgumEated.erase(pacgumEated.begin() + i); // On l'éface du tableau
                i--;
            }
        //}
    }

}
#pragma endregion
