#include <iostream>
#include <unistd.h>
#include "game/Terrain.h"
#include <curses.h>

int main(void)
{
    Terrain t(61, 61, 42);
    //Terrain t;
    WINDOW * w = subwin(stdscr, 10, 10, LINES, COLS);
   
    int c = 0;
    t.generateTerrain();
    initscr();
    bool quit = false;

    refresh();

    while(!quit)
    {

        clear();

        t.drawTerminal(0, 0);


        if(wrefresh(w) == ERR)
        {
            cerr<<"ERROR :: REFRESHING WINDOW :"<<endl;
        }
        if(getch() != 410) quit = true;
        c++;

        int x = rand()%60;
        int y = rand()%60;

        

        


    }

    endwin();

    free(w);
    t.~Terrain();
    return 0;
}