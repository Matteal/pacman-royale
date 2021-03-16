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
    Pacgum(Point c);
    Pacgum & getPacgum(vector<Pacgum> terrain, int x, int y) const;
    void setPacgum(vector<Pacgum> terrain, int x, int y);
    void eat();

};

#endif