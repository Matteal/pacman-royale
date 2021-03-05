#include <iostream>
#include <unistd.h>
#include "game/Terrain.h"
#include <curses.h>

int main(void)
{
    Terrain t(61, 61, 42);
    //Terrain t;
    WINDOW * w = subwin(stdscr, 10, 10, LINES, COLS);
    t.generateTerrain();

    initscr();
    bool quit = false;
    while(!quit)
    {
        clear();
        t.drawTerminal(0, 0);
        refresh();
        if(getch() != 410) quit = true;
    }

    endwin();

    free(w);
    t.~Terrain();
    return 0;
}