#include <iostream>
#include <SDL2/SDL_image.h>
#include <curses.h>
#include <stdlib.h>

using namespace std;

class Terrain
{
private:
    int Width = 0;
    int Height = 0;
    char * Grille = nullptr;
public:
    Terrain(int width, int height);
    void handTerrain();
    void generateTerrain(int seed);
    void drawTerminal() const;
    ~Terrain();
};

