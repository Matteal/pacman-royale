#include "Game.h"
#include <iostream>
#include <ctime>
#include <string.h>


#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "Renderer.h"


Game::Game() : _t(50, 50,177013), Pac()
{
    _score = 0;
    _superPacgum = 5;
}

void Game::update()
{
    std::cout << *start_time << std::endl;
}


void Game::Start(enum launch aff)
{
    init();
    mainloop(aff);
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
}

void Game::mainloop(enum launch aff)
{
    Renderer* renderer;
    // choisi le renderer à utiliser
    if(aff == CONSOLE) renderer= new ConsoleRenderer;
    else if (aff == SDL) renderer = new SDLRenderer;



    // Initialisation du renderer
    std::vector<Pacman*> tableauPacman;
    tableauPacman.push_back(&Pac);

    renderer->init(&_t, &tableauPacman);


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

      turn();
      cout<<" PacX "<<Pac.getIndexX()<<" PacY "<<Pac.getIndexY()<<endl;
      cout<<" PacX "<<Pac.getX()<<" PacY "<<Pac.getY()<<endl;
      walk(); // on déplace pacman suivant sa direction
      actuPacgum();
      Pac.actuState(); // Actualise l'état pacgum
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
  if(Pac._dirNext != Pac.getDir())
  {
    if(canTurn(Pac._dirNext))
    {
        if(Pac.getDir() == UP || Pac.getDir() == DOWN) Pac.setY(Pac.getIndexY());
        else Pac.setX(Pac.getIndexX());
        Pac.setDir(Pac._dirNext);
    }
  }
}

void Game::walk()
{
    // on gère ici les sorties de tableau pour que le Pacman apparaisse de l'autre côté
    if(Pac.getIndexX() < 0) // Si sort du tableau a gauche
    {
      Pac.setX(_t.getWidth() - 1); // on tp a droite
    }
    else if (Pac.getIndexX() >= _t.getWidth()) // si sort a droite
    {
      Pac.setX(0); // tp gauche
    }
    if(Pac.getY() < 0) // si sort en bas
    {
      Pac.setY(_t.getHeight() - 1); // tp haut
    }
    else if (Pac.getIndexY() >= _t.getHeight()) // si sort haut
    {
      Pac.setY(0); // tp bas
    }

    float vitesse = 0.4;
    switch (Pac.getDir())
    {
    case UP: //si haut est libre, on avance
        if(canTurn(UP)) Pac.setY(Pac.getY() + vitesse);
        break;

    case DOWN: //même chose en bas
        if(canTurn(DOWN)) Pac.setY(Pac.getY() - vitesse);
        break;

    case LEFT: // same a gauche
        if(canTurn(LEFT)) Pac.setX(Pac.getX() - vitesse);
        break;

    case RIGHT: // de même a droite
        if(canTurn(RIGHT)) Pac.setX(Pac.getX() + vitesse);
        break;
    }
}

bool Game::canTurn(direction dir)
{
    return ((_t.getNeighborTile({(float)Pac.getIndexX(), (float)Pac.getIndexY()}, dir, 1) == ' ') || (_t.getNeighborTile({(float)Pac.getIndexX(), (float)Pac.getIndexY()}, dir, 1) == '.') || (_t.getNeighborTile({(float)Pac.getIndexX(), (float)Pac.getIndexY()}, dir, 1) == 'S'));
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
                if((rand()%1000) < 5 && _superPacgum > 0)
                {
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
    if(Pac.getIndexX() >= 0 && Pac.getIndexY() >= 0 && Pac.getIndexX() < _t.getWidth() && Pac.getIndexY() < _t.getHeight())
    {
        int i = 0;

        while((pacgumList[i].getIndexX() != Pac.getIndexX()) || (pacgumList[i].getIndexY() != Pac.getIndexY())) // Cherche la pacgum ou est pacman
        {
            i++;
        }

        if(!pacgumList[i].getState()) //Si elle est vivante, il la mange
        {
            if(pacgumList[i].eat(_superPacgum))
            {
                Pac._isSuper = true; //On la retire des super si s'en était une (d'ou le nombre de super en param)
                Pac._timer = 0;
            }
            _score++; // On incrémente le score
            _t.setTile(pacgumList[i].getCoord().x, pacgumList[i].getCoord().y, ' '); //On transforme la case en vide
            pacgumEated.push_back(i); // On rajoute sont id aux pacgums à actu
        }


        for(i = 0; i < (int)pacgumEated.size(); i++) // Pour toutes les pacgums mangés
        {

            if((pacgumList[pacgumEated[i]].getIndexX() != Pac.getIndexX()) || (pacgumList[pacgumEated[i]].getIndexY() != Pac.getIndexY()))
            {   // Si pacman n'est pas dessus
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
            }
        }
    }
}
#pragma endregion
