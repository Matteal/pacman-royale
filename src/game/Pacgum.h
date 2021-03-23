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
    Pacgum(Point c, bool isSuper);
    Pacgum();
    bool actu(int & nbS);
    void eat(int & nbS);

    bool getState() const;
    Point getCoord() const;
    int getIndexX() const;
    int getIndexY() const;

    bool getSuper() const;
    bool setSuper(bool b);

};

#endif