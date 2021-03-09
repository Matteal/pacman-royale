#include <iostream>
#include <unistd.h>
#include "game/Game.h"
#include <curses.h>

int main(void)
{
    Game gear;

    gear.Start(true);

    return 0;
}