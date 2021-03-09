#pragma once

#include <iostream>
#include <array>
#include <string>

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
    template <typename T, size_t WIDTH, size_t HEIGHT>
    using Array2D = std::array<std::array<T, WIDTH>, HEIGHT>;

    Terrain(int width, int height);
    ~Terrain();

    void hardcodeTerrain();
    void drawToTerminal() const;
    void createTerrainFromFile(char[]);
};