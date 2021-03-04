#include <iostream>
#include <SDL2/SDL_image.h>
#include <curses.h>
#include <stdlib.h>
#include <vector>
#include <assert.h>

using namespace std;

struct Point
{
    int x;
    int y;
};

class Terrain
{
private:
    int Width = 0;
    int Height = 0;
    int Seed = 0;
    char * Grille = nullptr;

    void flood(Point cell, vector<Point> & possibleDirection);
    void cutThrough(Point Cell);
public:
    Terrain(int width, int height, int seed);
    Terrain();
    void generateTerrain();
    void drawTerminal(int x, int y) const;
    ~Terrain();
};


