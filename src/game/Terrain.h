#include <iostream>
#include <SDL2/SDL_image.h>
#include <curses.h>
using namespace std;

class Terrain
{
private:
    int _width = 0;
    int _height = 0;
    unsigned char *_grille = nullptr;

public:
    Terrain(int width, int height);
    void hardcodeTerrain();
    void drawToTerminal() const;
    ~Terrain();
};
