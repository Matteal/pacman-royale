#include "Ghost.h"
Ghost::Ghost(float x = 0, float y = 0, int r = 255, int g = 255, int b = 255) : _x(x), _y(y), _r(r), _g(g), _b(b)
{
    _repr = 'W';
}

float Ghost::getY() { return _y; }
void Ghost::setY(float y) { _y = y; }

float Ghost::getX() { return _x; }
void Ghost::setX(float x) { _x = x; }

char Ghost::getRepr() { return _repr; }
void Ghost::setRepr(char repr) { _repr = repr; }