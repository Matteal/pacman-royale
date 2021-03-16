#ifndef PACGUM_H
#define PACGUM_H
#include "Game.h"
class Pacgum
{
private:
    Point Coord;
    bool isEated;
    bool isSuper;
public:
    pacgum(Point c);
    pacgum & getPacgum(vector<pacgum> terrain, int x, int y) const;
    void setPacgum(vector<pacgum> terrain, int x, int y);
    void eat();

};

#endif