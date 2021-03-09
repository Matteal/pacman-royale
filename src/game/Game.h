#include "Terrain.h"
#include "Pacman.h"
class Game
{
public:
    
    Terrain Map;
    Pacman Pac;

    Game();

    void Start(bool console = false);

private:
    void init();
    void updateConsole();
    void updateSDL();
    void end();

    void drawConsole(int x, int y) const;
   

};