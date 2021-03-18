#include "Ghost.h"
Ghost::Ghost(Point P = {0, 0}, int r = 255, int g = 255, int b = 255) : _p(P), _r(r), _g(g), _b(b)
{
    _repr = 'W';
}

float Ghost::getY() const { return _p.y; }
void Ghost::setY(float y) { _p.y = y; }

float Ghost::getX() const { return _x; }
void Ghost::setX(float x) { _x = x; }

char Ghost::getRepr() const { return _repr; }
void Ghost::setRepr(char repr) { _repr = repr; }

void Ghost::setPos(Point P)
{
    _p = P;    
}
Point Ghost::getPos() const
{
    return _p;
}