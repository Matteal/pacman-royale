#include <iostream>
#include <unistd.h>
#include "Terrain.h"
#include <curses.h>

int main(void)
{
    Terrain t(30, 30);
    t.handTerrain();
    WINDOW * w = subwin(stdscr, 10, 10, LINES, COLS);

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