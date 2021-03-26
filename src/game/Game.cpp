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


Game::Game() : _t(50, 50,177013), Pac()
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
    Pac.setPlayer(true);
    pacmanList.push_back(&Pac);

    pacmanList.push_back(new Pacman);
    pacmanList[1]->setPos(_t.randomPointEmpty());
    pacmanList[1]->setGhost(true);

    pacmanList.push_back(new Pacman);
    pacmanList[2]->setPos(_t.randomPointEmpty());
    pacmanList[2]->setGhost(true);
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
    while (!quit) // Boucle d'initialisation
    {

        // Calcule le temps pris par la frame précedente
        //delta = chrono::duration_cast<chrono::milliseconds>(end - start);

        // On redémarre le chrono immédiatement pour être aussi fiable que possible
        //start = chrono::steady_clock::now();

        // Si la mise à jour a été trop rapide, on attend pour garder le rythme
        if (delta.count() < updateFrequency)
            usleep(delta.count() - updateFrequency);
        cout<<"truc"<<endl;
        renderer->render(Pac._isDead);
        // Récupération des entrées utilisateur
        UserInput input = renderer->getInput();
        //cout<<input<<endl;

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
        };

      turn();

      cout<<"Timer = "<<Pac._timer<<" isSuper = "<<Pac._isSuper<<endl;
      walk(); // on déplace pacman suivant sa direction
      //cout<<Pac._isDead<<endl;
      if(Pac._isDead) quit = true;
      actuPacgum();

      //flushinp();
      //end = chrono::steady_clock::now();
    }
    
    quit = false;
    while(!quit && Pac._isDead)
    {
        renderer->render(Pac._isDead);
        UserInput input = renderer->getInput();
        if(input != IDLE) quit = true;
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
    if(pacmanList[i]->getGhost())
    {
        actuDirGhost(pacmanList[i]);
    }
    if(pacmanList[i]->_dirNext != pacmanList[i]->getDir())
    {
        if(canTurn(pacmanList[i], pacmanList[i]->_dirNext))
        {
            if(pacmanList[i]->getDir() == UP || pacmanList[i]->getDir() == DOWN) pacmanList[i]->setY(pacmanList[i]->getIndexY());
            else pacmanList[i]->setX(pacmanList[i]->getIndexX());
            pacmanList[i]->setDir(pacmanList[i]->_dirNext);
        }     
    }
  }
}

void Game::walk()
{
    float vitesse = 0.4;
  for(int i=0; i<(int)pacmanList.size(); i++)
  {
    // on gère ici les sorties de tableau pour que le Pacman apparaisse de l'autre côté
    if(pacmanList[i]->getIndexX() < 0) pacmanList[i]->setX(_t.getWidth() - 1); // Si sort du tableau a gauche on tp a droite
    else if (pacmanList[i]->getIndexX() >= _t.getWidth()) pacmanList[i]->setX(0); // Si sort a droite on tp gauche
    if(pacmanList[i]->getY() < 0) pacmanList[i]->setY(_t.getHeight() - 1); // si sort en bas on tp haut
    else if (pacmanList[i]->getIndexY() >= _t.getHeight()) pacmanList[i]->setY(0); // si sort en haut on tp bas
    if(pacmanList[i]->getPlayer())
    {
        for(int j = i+1; j < (int)pacmanList.size(); j++)
        {
            if(pacmanList[i]->getIndexPos() == pacmanList[j]->getIndexPos() && !pacmanList[j]->_isDead)
            {
                if(pacmanList[i]->_isSuper)
                {
                    pacmanList[j]->_isDead = true;
                    _score+=100;
                    pacmanList.erase(pacmanList.begin() + j);
                    j--;
                }
                else
                {
                    pacmanList[i]->_isDead = true;
                    pacmanList[i]->_timer = 0;
                }
            }
        }
        vitesse = 0.4;
    }
    else vitesse = 0.3;
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
    return ((_t.getNeighborTile({(float)pac->getIndexX(), (float)pac->getIndexY()}, dir, 1) == ' ') || (_t.getNeighborTile({(float)pac->getIndexX(), (float)pac->getIndexY()}, dir, 1) == '.') || (_t.getNeighborTile({(float)pac->getIndexX(), (float)pac->getIndexY()}, dir, 1) == 'S'));
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
                if((rand()%1000) < 5 && _superPacgum > 0)
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
        if(pacmanList[i]->getIndexX() >= 0 && pacmanList[i]->getIndexY() >= 0 && pacmanList[i]->getIndexX() < _t.getWidth() && pacmanList[i]->getIndexY() < _t.getHeight())
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
#pragma region IA
void Game::actuDirGhost(Pacman * pac)
{
    if(!canTurn(pac, pac->getDir()))
    {
        int r = rand()%100;
        switch (pac->getDir())
        {
        case UP:
            if(canTurn(pac, LEFT) && canTurn(pac, RIGHT)) {if(r < 50) pac->_dirNext = (LEFT);}
            else if(canTurn(pac, LEFT) || canTurn(pac, RIGHT)) 
            {
                if(canTurn(pac, LEFT)) pac->_dirNext = (LEFT); 
                else pac->_dirNext = (RIGHT);
            }
            else pac->_dirNext = (DOWN);
            break;
        
        case DOWN:
            if(canTurn(pac, LEFT) && canTurn(pac, RIGHT)) {if(r < 50) pac->_dirNext = (LEFT);}
            else if(canTurn(pac, LEFT) || canTurn(pac, RIGHT)) 
            {
                if(canTurn(pac, LEFT)) pac->_dirNext = (LEFT); 
                else pac->_dirNext = (RIGHT);
            }
            else pac->_dirNext = (UP);
            break;

        case LEFT:
            if(canTurn(pac, DOWN) && canTurn(pac, UP)) {if(r < 50) pac->_dirNext = (DOWN);}
            else if(canTurn(pac, DOWN) || canTurn(pac, UP)) 
            {
                if(canTurn(pac, DOWN)) pac->_dirNext = (DOWN); 
                else pac->_dirNext = (UP);
            }
            else pac->_dirNext = (RIGHT);
            break;

        case RIGHT:
            if(canTurn(pac, DOWN) && canTurn(pac, UP)) {if(r < 50) pac->_dirNext = (DOWN);}
            else if(canTurn(pac, DOWN) || canTurn(pac, UP)) 
            {
                if(canTurn(pac, DOWN)) pac->_dirNext = (DOWN); 
                else pac->_dirNext = (UP);
            }
            else pac->_dirNext = (LEFT);
            break;
        
        default:
            break;
        }
    }
}
#pragma endregion