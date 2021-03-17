#ifndef TERRAIN_H
#define TERRAIN_H

#include <iostream>
#include <array>
#include <string>
#include <stdlib.h>
#include <vector>
#include <assert.h>
#include <string>

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
    void enhancer();
    int countNeighbor(Point P) const;


public:
    Terrain(int width, int height, int seed);
    Terrain();

    Terrain(const std::string& copy);
    std::string exportToString();

    void generateTerrain();
    void drawTerminal(int x, int y) const;
    void setTile(int x, int y, char c);
    char getTile(int x, int y) const;
    int getWidth() const;
    int getHeight() const;

    //merge conflict part 1
    char* getGrille() const;
    //mege conflict part 2
    char getNeighborTile(Point P, int dir, int dist);
    Point getNeighbor(Point P, int dir, int dist);
    //end merge conflict
    
    ~Terrain();

    void hardcodeTerrain();
    //void drawToTerminal() const;
    void createTerrainFromFile(const char*);
};
#endif // TERRAIN_H
