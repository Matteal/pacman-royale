#include "Pacman.h"

Pacman::Pacman(float x=0, float y=0, int dir = 0, int dirNext = 0, int r=255, int g=255, int b=255) : _x(x), _y(y), _dir(dir), _dirNext(dirNext),_r(r), _g(g), _b(b)
{
    _repr = '@';
}

float Pacman::getY() 
{ 
    return _y; 
}
void Pacman::setY(float y) 
{ 
    _y = y; 
}

float Pacman::getX() 
{ 
    return _x; 
}
void Pacman::setX(float x) 
{ 
    _x = x; 
}

char Pacman::getRepr() 
{ 
    return _repr; 
}

int Pacman::getDir()
{
    return _dir;
}
void Pacman::setDir(int dir)
{
    _dir = dir;
}

int Pacman::getIndexX()
{
    if(_x - (int)(_x) < 0.5)
    {
        return (int)(_x);
    }
    else
    {
        return  (int)(_x + 1);
    }
}

int Pacman::getIndexY()
{
    if(_y - (int)(_y) < 0.5)
    {
        return (int)(_y);
    }
    else
    {
        return  (int)(_y + 1);
    }
}