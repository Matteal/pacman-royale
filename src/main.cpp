#include <iostream>
#include <unistd.h>
#include <string>
#include <curses.h>

#include "game/Terrain.h"
#include "game/Game.h"

int main(void)
{
    Game game;
    game.init();
    WINDOW *w = subwin(stdscr, 10, 10, LINES, COLS);

    initscr();
    bool quit = false;
    while (!quit)
    {
        clear();
        game.update();
        game.render();
        if (getch() != 410)
            quit = true;
    }

    endwin();

    free(w);
    game.end();
    return 0;
}