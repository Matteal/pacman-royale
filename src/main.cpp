#include <iostream>
#include <unistd.h>
#include "game/Terrain.h"
#include <curses.h>

int main(void)
{
    Terrain t(30, 30);
    t.hardcodeTerrain();
    WINDOW * w = subwin(stdscr, 10, 10, LINES, COLS);

    initscr();
    bool quit = false;
    while(!quit)
    {
        clear();
        t.drawToTerminal();
        refresh();
        if(getch() != 410) quit = true;
    }

    endwin();

    free(w);
    t.~Terrain();
    return 0;
}