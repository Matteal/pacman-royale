#include "Game.h"

#include <iostream>
#include <ctime>


// Game::Game() : _t(19, 21), _score(0), _lives(3)
// {
// }

Game::Game() : _t(35,35,35), Pac(_t.getWidth()/2, _t.getHeight()/2, 200, 20, 120)
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
    _t.generateTerrain();
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
    _t.~Terrain();
}

void Game::drawConsole(int x, int y) const
{
    char line[_t.getWidth()*2+1];
    for(int i = 0; i < _t.getWidth(); i++)
    {
        for(int j = 0; j < _t.getWidth()*2; j++)
        {
            if(j%2 == 0)
            {
                line[j] = _t.getTile(i, j/2);

            }
            else
            {
                line[j] = ' ';
            }
        }
        line[_t.getWidth()*2] = '\0';
        mvprintw((LINES / 2) - i + (_t.getWidth() /2), (COLS / 2) - (_t.getWidth()*2 / 2), line);
    }
}
