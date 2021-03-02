#include "Ghost.h"
Ghost::Ghost(int x=0, int y=0, int r=255, int g=255, int b=255) : _x(x), _y(y), _r(r), _g(g), _b(b)
{
    _repr = 'W';
}

int Ghost::getY() { return _y; }
void Ghost::setY(int y) { _y = y; }

int Ghost::getX() { return _x; }
void Ghost::setX(int x) { _x = x; }

char Ghost::getRepr() { return _repr; }
void Ghost::setRepr(char repr) { _repr = repr; }