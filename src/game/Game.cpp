#include "Game.h"
#include <iostream>
#include <string.h>

#include <chrono>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "Renderer.h"

const FPS = 60;

Game::Game() : _t(34, 34, 177013), Pac()
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
    Pac.setDir(LEFT); // Le Pacman va monter dès l'exécution du programme
    Pac._dirNext = LEFT;
    Pac.setX(_t.getWidth() / 2 - 1); // Le place
    Pac.setY(_t.getHeight() / 2);
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
    std::vector<Pacman *> tableauPacman;
    tableauPacman.push_back(&Pac);

    renderer->init(&_t, &tableauPacman);

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

        turn();
        cout << "Bonus Time = " << Pac._timer << " Point = " << _score << endl;
        walk(); // On déplace pacman suivant sa direction
        actuPacgum();
        Pac.actuState(); // Actualise l'état pacgum
        flushinp();
        delta = chrono::steady_clock::now();
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
    if (Pac._dirNext != Pac.getDir())
    {
        if (canTurn(Pac._dirNext))
        {
            Pac.setDir(Pac._dirNext);
        }
    }
}

void Game::walk()
{
    // On gère ici les sorties de tableau pour que le Pacman apparaisse de l'autre côté
    if (Pac.getIndexX() < 0) // Si sort du tableau a gauche
    {
        if (canTurn(LEFT))
            Pac.setX(_t.getWidth() - 1); // On le tp à droite
    }
    else if (Pac.getIndexX() >= _t.getWidth()) // S'il sort à droite
    {
        if (canTurn(RIGHT))
            Pac.setX(0); // On le tp à gauche
    }
    if (Pac.getY() < 0) // si sort en bas
    {
        if (canTurn(DOWN))
            Pac.setY(_t.getHeight() - 1); // On le tp en haut
    }
    else if (Pac.getIndexY() >= _t.getHeight()) // S'il sort en haut
    {
        if (canTurn(UP))
            Pac.setY(0); // On le tp bas
    }

    float vitesse = 0.4;
    switch (Pac.getDir())
    {
    case UP: // Si haut est libre, on avance
        if (canTurn(UP))
            Pac.setY(Pac.getY() + vitesse);
        break;

    case DOWN: // Même chose en bas
        if (canTurn(DOWN))
            Pac.setY(Pac.getY() - vitesse);
        break;

    case LEFT: // Gauche
        if (canTurn(LEFT))
            Pac.setX(Pac.getX() - vitesse);
        break;

    case RIGHT: // Droite
        if (canTurn(RIGHT))
            Pac.setX(Pac.getX() + vitesse);
        break;
    }
}

bool Game::canTurn(direction dir)
{
    // Si la case dans la direction du Pacman est libre, true; sinon false
    return (_t.getNeighborTile({(float)Pac.getIndexX(), (float)Pac.getIndexY()}, dir, 1) != '#');
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
    if (Pac.getX() >= 0 && Pac.getY() >= 0 && Pac.getX() < _t.getWidth() - 1 && Pac.getY() < _t.getHeight() - 1)
    {
        int i = 0;

        while ((pacgumList[i].getIndexX() != Pac.getIndexX()) || (pacgumList[i].getIndexY() != Pac.getIndexY())) // Cherche la pacgum ou est pacman
        {
            i++;
        }

        if (!pacgumList[i].getState()) // Si elle est vivante, il la mange
        {
            if (pacgumList[i].eat(_superPacgum))
            {
                Pac._isSuper = true; // On la retire des super si ç'en était une (d'où le nombre de super en param)
                Pac._timer = 0;
            }
            _score++;                                                                // On incrémente le score
            _t.setTile(pacgumList[i].getCoord().x, pacgumList[i].getCoord().y, ' '); // On transforme la case en case vide
            pacgumEaten.push_back(i);                                                // On rajoute son id aux pacgums à actu
        }

        for (i = 0; i < (int)pacgumEaten.size(); i++) // Pour toutes les pacgums mangés
        {

            if ((pacgumList[pacgumEaten[i]].getIndexX() != Pac.getIndexX()) || (pacgumList[pacgumEaten[i]].getIndexY() != Pac.getIndexY()))
            {                                                      // Si pacman n'est pas dessus
                if (pacgumList[pacgumEaten[i]].actu(_superPacgum)) // On l'actualise
                {
                    if (pacgumList[pacgumEaten[i]].getSuper()) // Si c'est une super
                    {
                        // On remplace son char par un S
                        _t.setTile(pacgumList[pacgumEaten[i]].getCoord().x, pacgumList[pacgumEaten[i]].getCoord().y, 'S');
                    }
                    else if (!pacgumList[pacgumEaten[i]].getSuper())
                    {
                        // On remet un point sinon
                        _t.setTile(pacgumList[pacgumEaten[i]].getCoord().x, pacgumList[pacgumEaten[i]].getCoord().y, '.');
                    }

                    pacgumEaten.erase(pacgumEaten.begin() + i); // On l'éface du tableau
                    i--;
                }
            }
        }
    }
}
#pragma endregion
