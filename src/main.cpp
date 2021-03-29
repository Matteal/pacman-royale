#include <iostream>
#include <unistd.h>
#include <string.h>

using namespace std;

#include "game/Game.h"

void DoNothing(){};

int main(int argc, char *argv[])
{
    Game game;
    game.setCallback(std::bind(DoNothing));
    if(argc > 1)
    {
        if(strcmp(argv[1], "console") == 0)
        {
            cout<<"Affichage console enclanché"<<endl;
            game.Start(CONSOLE);
        }
        else if(strcmp(argv[1], "SDL") == 0)
        {
            cout<<"Affichage SDL Enclanché"<<endl;
            game.Start(SDL);
        }
        else
        {
            cout<<"Argument invalide, <<console>> pour le terminal et <<SDL>> pour la SDL, ARRET"<<endl;
            return EXIT_FAILURE;
        }
    }
    else
    {
        cout<<"Aucun argument, lancement en console"<<endl;
        game.Start(CONSOLE);
    }


    return EXIT_SUCCESS;
}
