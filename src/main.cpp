#include <iostream>
#include <unistd.h>
#include <string.h>

using namespace std;

#include "game/Game.h"

void DoNothing(std::string instruction){std::cout<<instruction<<std::endl;};
int main(int argc, char *argv[])
{
    Game game(100, 100, 131489);
    game.init(2, 0 , 0);
    game.setCallback(std::bind(DoNothing, std::placeholders::_2));
    if(argc > 1)
    {
        if(strcmp(argv[1], "console") == 0)
        {
            cout<<"Affichage console enclanché"<<endl;
            game.mainloopDebug(CONSOLE);
        }
        else if(strcmp(argv[1], "SDL") == 0)
        {
            cout<<"Affichage SDL Enclanché"<<endl;
            game.mainloopDebug(SDL);
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
        game.mainloopDebug(CONSOLE);
    }


    return EXIT_SUCCESS;
}
