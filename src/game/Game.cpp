#include "Game.h"

#include <iostream>
#include <ctime>


// Game::Game() : _t(19, 21), _score(0), _lives(3)
// {
// }

Game::Game() : Pac(Map.getWidth()/2, Map.getHeight()/2, 200, 20, 120)
{
    Map = Terrain(35, 35, 35);
//    Pac = Pacman(Map.getWidth()/2, Map.getHeight()/2, 200, 20, 120);
}

void Game::update()
{
    std::cout << *start_time << std::endl;
}

void Game::render()
{
    _t.drawTerminal(0,0);
}


void Game::Start(bool console)
{
    init();
    if(console)
    {
        updateConsole();
    }
    else
    {
        //updateSDL();
    }

    end();
}

void Game::init()
{
    Map.generateTerrain();
}

void Game::updateConsole()
{
    WINDOW * w = subwin(stdscr, 10, 10, LINES, COLS);
    initscr();
    bool quit = false;
    refresh();
    while(!quit)
    {

        clear();

        drawConsole(0, 0);


        if(wrefresh(w) == ERR)
        {
            cerr<<"ERROR :: REFRESHING WINDOW :"<<endl;
        }
        if(getch() != 410) quit = true;
    }

    endwin();
    free(w);
}
void Game::end()
{
    Map.~Terrain();
}

void Game::drawConsole(int x, int y) const
{
    char line[Map.getWidth()*2+1];
    for(int i = 0; i < Map.getWidth(); i++)
    {
        for(int j = 0; j < Map.getWidth()*2; j++)
        {
            if(j%2 == 0)
            {
                line[j] = Map.getTile(i, j/2);

            }
            else
            {
                line[j] = ' ';
            }
        }
        line[Map.getWidth()*2] = '\0';
        mvprintw((LINES / 2) - i + (Map.getWidth() /2), (COLS / 2) - (Map.getWidth()*2 / 2), line);
    }
}
