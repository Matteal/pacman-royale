#ifndef PACGUM_H
#define PACGUM_H
#include "Game.h"
class Pacgum
{
private:
    Point Coord;
    bool isEated = false;
    bool isSuper = false;
    int timer = 0;

public:
    Pacgum(Point c);
    void actu(vector<Pacgum> pacList, const Point & Pacman)
    void eat();

};

#endif