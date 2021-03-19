#ifndef PACGUM_H
#define PACGUM_H
#include "Point.h"
#include <iostream>
class Pacgum
{
private:
    Point Coord;
    bool isEated = false;
    bool isSuper = false;
    int timer = 0;

public:
    Pacgum(Point c);
    Pacgum();
    bool actu();
    void eat();

    bool getState() const;
    Point getCoord() const;

};

#endif