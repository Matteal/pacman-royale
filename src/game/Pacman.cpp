#include "Pacman.h"

Pacman::Pacman(int x=0, int y=0, int r=255, int g=255, int b=255) : _x(x), _y(y), _r(r), _g(g), _b(b)
{
    _repr = '@';
}

int Pacman::getY() { return _y; }
void Pacman::setY(int y) { _y = y; }

int Pacman::getX() { return _x; }
void Pacman::setX(int x) { _x = x; }

char Pacman::getRepr() { return _repr; }