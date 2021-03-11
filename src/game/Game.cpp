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

Game::Game() : _t(35,35,177013), Pac(0, 0, 0, 255, 255, 255)
{
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
    _t.generateTerrain();
    Pac.setDir(3);
    Pac.setX(_t.getWidth()/2);
    Pac.setY(_t.getHeight()/2 - 1);
}

void Game::renderConsole()
{
    
    WINDOW * w = subwin(stdscr, 10, 10, LINES, COLS);
    initscr();
    noecho();
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    bool quit = false;
    int r;
    refresh();
    int ch;

   
    while(!quit)
    {
        
        clear();
        
        float t0 = clock();
        drawConsole();
        float t1 = clock() - t0;
        t1 = t1;
        napms(t1);

        r = wrefresh(w);
        

        if(r == ERR)
        {
            cerr<<"ERROR :: REFRESHING WINDOW :"<<strerror(errno)<<endl;
        }
        ch = getch();
        if(ch != ERR)
        {
            inputHandler(ch, quit);
        }
        walk();

    }
    endwin();
    free(w);
}
void Game::end()
{
    _t.~Terrain();
}

void Game::drawConsole()
{
    char line[_t.getWidth()*2+1];
    for(int j = 0; j < _t.getHeight(); j++)
    {
        for(int i = 0; i < _t.getWidth() * 2; i++)
        {
            if(i%2 == 0)
            {
                if(i/2 == Pac.getIndexX()  && j == Pac.getIndexY())
                {
                    line[i] = 'O';
                }
                else
                {
                    line[i] = _t.getTile(i/2, j);
                }
                
            }
            else
            {
                line[i] = ' ';
            }
        }
        line[_t.getWidth()*2] = '\0';
        mvprintw((LINES / 2) - j + (_t.getWidth() /2), (COLS / 2) - (_t.getWidth()*2 / 2), line);
    }
}

void Game::inputHandler(int input, bool & quit)
{
    switch (input)
    {
    case 27:
        quit = true;
        break;
    case 'z':
        if(canTurnUp()) Pac.setDir(0);
        break;
    case 's':
        if(canTurnDown()) Pac.setDir(1);
        break;
    case 'q':
        if(canTurnLeft()) Pac.setDir(2);
        break;
    case 'd':
        if(canTurnRight()) Pac.setDir(3);
        break;
    default:
        break;
    }
}

void Game::walk()
{
    float vitesse = 0.4;
    if(Pac.getIndexX() < 0)
    {
      if(_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 2, 1) == ' ') Pac.setX(_t.getWidth() - 1);
    }
    else if (Pac.getIndexX() >= _t.getWidth())
    {
        Pac.setX(34.49);
        if(_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 3, 1) == ' ') Pac.setX(0);
    }
    if(Pac.getY() < 0)
    {
        if(_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 1, 1) == ' ') Pac.setY(_t.getHeight() - 1);
    }
    else if (Pac.getIndexY() >= _t.getHeight())
    {
        Pac.setY(34.49);
        if(_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 0, 1) == ' ') Pac.setY(0);
    }

    switch (Pac.getDir())
    {
    case 0:
        if(_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 0, 1) == ' ') Pac.setY(Pac.getY() + vitesse);
        break;
    
    case 1:
        if(_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 1, 1) == ' ') Pac.setY(Pac.getY() - vitesse);
        break;

    case 2:
        if(_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 2, 1) == ' ') Pac.setX(Pac.getX() - vitesse);
        break;

    case 3:
        if(_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 3, 1) == ' ') Pac.setX(Pac.getX() + vitesse);
        break;

    default:
        break;
    }
}

bool Game::canTurnUp()
{
    return (_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 0, 1) == ' ');
}

bool Game::canTurnDown()
{
    return (_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 1, 1) == ' ');
}

bool Game::canTurnLeft()
{
    return (_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 2, 1) == ' ');
}

bool Game::canTurnRight()
{
    return (_t.getNeighborTile({Pac.getIndexX(), Pac.getIndexY()}, 3, 1) == ' ');
}
