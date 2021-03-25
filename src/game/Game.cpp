#include "Game.h"
#include <iostream>
#include <string.h>

#include <chrono>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "Renderer.h"

const float FPS = 60;

Game::Game(int t_width, int t_height, int t_seed) : _t(t_width, t_height, t_seed), Pac()
{
    _score = 0;
    _superPacgum = 5;
}

void Game::update()
{
    std::cout << "update!" << std::endl;
}

void Game::Start(enum launch aff)
{
    init();
    mainloop(aff);
    end();
}

void Game::init()
{
    _t.generateTerrain(); // Génère le terrain
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

void Game::mainloop(enum launch aff)
{
    Renderer *renderer;
    // Choisit le renderer à utiliser
    if (aff == CONSOLE)
        renderer = new ConsoleRenderer;
    else if (aff == SDL)
        renderer = new SDLRenderer;

    // Initialisation du renderer
    renderer->init(&_t, &pacmanList);

    // Début de la boucle
    bool quit = false; // Condition d'arrêt

    // Ces deux variables serviront à calculer l'écart entre deux frames et
    // maintenir 60 UPS (update per second) constants (et FPS, car liés*)
    chrono::_V2::steady_clock::time_point start, end;
    std::chrono::milliseconds delta;
    /*

    *: C'est problématique sur le long terme car UPS et FPS étant liés, le jeu
    ralentira si le programme ralentit. Une solution utilisée dans la plupart
    des jeux modernes est de sauter des frames, sauf qu'ici ça ferait sauter
    des updates.

    Idéalement il faudrait séparer FPS et UPS, mais on ne prévoit pas de faire
    un jeu trop gourmand donc on se permet l'écart. -TF

    */

    // Stocke la fréquence de mise à jour en Hertz
    float updateFrequency = (float)1 / (float)FPS;

    int tour_de_boucle = 0;
    while (!quit) // Boucle d'initialisation
    {

        // Calcule le temps pris par la frame précedente
        delta = chrono::duration_cast<chrono::milliseconds>(end - start);

        // On redémarre le chrono immédiatement pour être aussi fiable que possible
        start = chrono::steady_clock::now();

        // Si la mise à jour a été trop rapide, on attend pour garder le rythme
        if (delta.count() < updateFrequency)
            usleep(delta.count() - updateFrequency);

        renderer->render();
        // Récupération des entrées utilisateur
        UserInput input = renderer->getInput();

        switch (input)
        {
        case QUIT:
            quit = true;
            break;
        case IDLE:
            break;
        case Z:
            Pac._dirNext = UP;
            break;
        case Q:
            Pac._dirNext = LEFT;
            break;
        case S:
            Pac._dirNext = DOWN;
            break;
        case D:
            Pac._dirNext = RIGHT;
            break;
        }

        if(tour_de_boucle%15==0)
        {
          switch(rand()%4)
          {
            case 0:
              pacmanList[1]->_dirNext = UP;
              pacmanList[2]->_dirNext = RIGHT;
              break;
            case 1:
              pacmanList[1]->_dirNext = DOWN;
              pacmanList[2]->_dirNext = UP;
              break;
            case 2:
              pacmanList[1]->_dirNext = LEFT;
              pacmanList[2]->_dirNext = LEFT;
              break;
            case 3:
              pacmanList[1]->_dirNext = RIGHT;
              pacmanList[2]->_dirNext = DOWN;
              break;
          }
        }
        tour_de_boucle++;

      turn();
      cout<<"Timer = "<<Pac._timer<<" isSuper = "<<Pac._isSuper<<endl;
      walk(); // on déplace pacman suivant sa direction
      actuPacgum();

      flushinp();

      end = chrono::steady_clock::now();
    }

    delete renderer;
}

void Game::end()
{
    _t.~Terrain(); // Destruction terrain
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
    for (int i = 0; i < _t.getWidth(); i++)
    {
        for (int j = 0; j < _t.getHeight(); j++)
        {
            if (_t.getTile(i, j) == ' ')
            {
                // _superPacgum est le compte total de super pac-gommes dans le jeu
                // On 'consomme' ici tous les points dans ce compteur pour les placer
                bool isSuper = false;
                if ((rand() % 100) < 1 && _superPacgum > 0)
                {
                    isSuper = true;
                    _superPacgum--;
                }
                pacgumList.push_back(Pacgum(Point(i, j), isSuper));
                if (isSuper)
                    _t.setTile(i, j, 'S');
                else
                    _t.setTile(i, j, '.');
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
                pacgumEaten.push_back(j); // On rajoute sont id aux pacgums à actu
            }
        }

        pacmanList[i]->actuState(); // Actualise l'état super-pacgum
    }


    for(int i = 0; i < (int)pacgumEaten.size(); i++) // Pour toutes les pacgums mangés
    {

        //if((pacgumList[pacgumEaten[i]].getIndexX() != Pac.getIndexX()) || (pacgumList[pacgumEaten[i]].getIndexY() != Pac.getIndexY()))
        //{   // Si pacman n'est pas dessus
        // commenté pour l'instant car trop greedy

            if(pacgumList[pacgumEaten[i]].actu(_superPacgum)) //on l'actualise
            {
                if(pacgumList[pacgumEaten[i]].getSuper()) //Si c'est une super
                {
                    _t.setTile(pacgumList[pacgumEaten[i]].getCoord().x, pacgumList[pacgumEaten[i]].getCoord().y, 'S'); // On remplace son char par un S
                }
                else if(!pacgumList[pacgumEaten[i]].getSuper())
                {
                    _t.setTile(pacgumList[pacgumEaten[i]].getCoord().x, pacgumList[pacgumEaten[i]].getCoord().y, '.'); // On remet un point sinon
                }

                pacgumEaten.erase(pacgumEaten.begin() + i); // On l'éface du tableau
                i--;
            }
        //}
    }

}
#pragma endregion
