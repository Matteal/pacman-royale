#include <iostream>
#include <unistd.h>
#include "game/Terrain.h"
#include <curses.h>

int main(void)
{
    Terrain t(10, 10);
    //t.handTerrain();
    WINDOW * w = subwin(stdscr, 10, 10, LINES, COLS);
    t.generateTerrain(3);

    initscr();
    bool quit = false;
    while(!quit)
    {
        clear();
        t.drawTerminal();
        refresh();
        if(getch() != 410) quit = true;
    }

    endwin();

    free(w);
    t.~Terrain();
    return 0;
}