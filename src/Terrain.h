#include <iostream>
#include <SDL2/SDL_image.h>
#include <curses.h>
using namespace std;

class Terrain
{
private:
    int Width = 0;
    int Height = 0;
    unsigned char * Grille = nullptr;
public:
    Terrain(int width, int height);
    void handTerrain();
    void drawTerminal() const;
    ~Terrain();
};

