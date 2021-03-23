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

Game::Game() : _t(34,34,177013), Pac(Point(0, 0), 0, 0, 255, 255, 255)
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
    generatePacgum();
    Pac.setDir(-1); // Donne une direction négative a pacman pour qu'il soit immobile
    Pac._dirNext = -1;
    Pac.setX(_t.getWidth()/2 - 1); //Le place
    Pac.setY(_t.getHeight()/2);
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
        turn();
        cout<<"X = i:"<<Pac.getIndexX()<<" f: "<<Pac.getX()<<" Y = i: "<<Pac.getIndexY()<<" f:"<<Pac.getY()<<endl;
        walk(); // on déplace pacman suivant sa direction
        actuPacgum();
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
      if(_t.getNeighborTile({(float)Pac.getIndexX(), (float)Pac.getIndexY()}, 2, 1) != '#') Pac.setX(_t.getWidth() - 1); // on tp a droite
    }
    else if (Pac.getIndexX() >= _t.getWidth()) // si sort a droite
    {
        if(_t.getNeighborTile({(float)Pac.getIndexX(), (float)Pac.getIndexY()}, 3, 1) != '#') Pac.setX(0); // tp gauche
    }
    if(Pac.getY() < 0) // si sort en bas
    {
        if(_t.getNeighborTile({(float)Pac.getIndexX(), (float)Pac.getIndexY()}, 1, 1) != '#') Pac.setY(_t.getHeight() - 1); // tp haut
    }
    else if (Pac.getIndexY() >= _t.getHeight()) // si sort haut
    {
        if(_t.getNeighborTile({(float)Pac.getIndexX(), (float)Pac.getIndexY()}, 0, 1) != '#') Pac.setY(0); // tp bas
    }

    switch (Pac.getDir()) 
    {
    case 0: //si haut est libre, on avance
        if(_t.getNeighborTile({(float)Pac.getIndexX(), (float)Pac.getIndexY()}, 0, 1) != '#') Pac.setY(Pac.getY() + vitesse);
        break;
    
    case 1: //même chose en bas
        if(_t.getNeighborTile({(float)Pac.getIndexX(), (float)Pac.getIndexY()}, 1, 1) != '#') Pac.setY(Pac.getY() - vitesse);
        break;

    case 2: // same a gauche
        if(_t.getNeighborTile({(float)Pac.getIndexX(), (float)Pac.getIndexY()}, 2, 1) != '#') Pac.setX(Pac.getX() - vitesse);
        break;

    case 3: // de même a droite
        if(_t.getNeighborTile({(float)Pac.getIndexX(), (float)Pac.getIndexY()}, 3, 1) != '#') Pac.setX(Pac.getX() + vitesse);
        break;

    default:
        break;
    }
}

bool Game::canTurnUp() // On vérifie que pacman peut tourner en haut
{
    return (_t.getNeighborTile({(float)Pac.getIndexX(), (float)Pac.getIndexY()}, 0, 1) != '#');
}

bool Game::canTurnDown() // On vérifie que pacman peut tourner en bas
{
    return (_t.getNeighborTile({(float)Pac.getIndexX(), (float)Pac.getIndexY()}, 1, 1) != '#');
}

bool Game::canTurnLeft() // On vérifie que pacman peut tourner a gauche
{
    return (_t.getNeighborTile({(float)Pac.getIndexX(), (float)Pac.getIndexY()}, 2, 1) != '#');
}

bool Game::canTurnRight() // On vérifie que pacman peut tourner a droite
{
    return (_t.getNeighborTile({(float)Pac.getIndexX(), (float)Pac.getIndexY()}, 3, 1) != '#');
}

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
        int i = 0;

        while((pacgumList[i].getIndexX() != Pac.getIndexX()) || (pacgumList[i].getIndexY() != Pac.getIndexY()))
        {
            i++;
        }
        
        if(!pacgumList[i].getState())
        {
            pacgumList[i].eat(_superPacgum);
            _score++;
            _t.setTile(pacgumList[i].getCoord().x, pacgumList[i].getCoord().y, ' ');
            pacgumEated.push_back(i);
        }
        

        for(i = 0; i < pacgumEated.size(); i++)
        {   
            
            if(Point(Pac.getIndexX(), Pac.getIndexY()) != pacgumList[pacgumEated[i]].getCoord())
            {  
                if(pacgumList[pacgumEated[i]].actu(_superPacgum)) 
                {
                    if(pacgumList[pacgumEated[i]].getSuper()) 
                    {
                        _t.setTile(pacgumList[pacgumEated[i]].getCoord().x, pacgumList[pacgumEated[i]].getCoord().y, 'S');
                    }
                    else if(!pacgumList[pacgumEated[i]].getSuper())
                    {
                        _t.setTile(pacgumList[pacgumEated[i]].getCoord().x, pacgumList[pacgumEated[i]].getCoord().y, '.');
                    }
                    
                    pacgumEated.erase(pacgumEated.begin() + i);
                    i--;
                }
            }
            
        }
    
    
}

