#include "Pacman.h"

Pacman::Pacman(Point P = Point(0, 0), int dir = 0, int dirNext = 0, int timer = 0, bool isSuper = false, bool isDead = false, int r=255, int g=255, int b=255) : _p(P), _dir(dir), _dirNext(dirNext), _timer(timer),_isSuper(isSuper), _isDead(isDead),_r(r), _g(g), _b(b)
{
    _repr = '@';
}

Pacman::Pacman()
{
    _p = Point(0, 0);
    _dir = 0;
    _dirNext = 0;
    _isSuper = false;
    _isDead = false;
    _timer = 0;

    _r = _g = _b = 255;
}

float Pacman::getY() const
{ 
    return _p.y; 
}
void Pacman::setY(float y) 
{ 
    _p.y = y; 
}

float Pacman::getX() const
{ 
    return _p.x; 
}
void Pacman::setX(float x) 
{ 
    _p.x = x; 
}

char Pacman::getRepr() 
{ 
    return _repr; 
}

int Pacman::getDir() const
{
    return _dir;
}
void Pacman::setDir(int dir)
{
    _dir = dir;
}

int Pacman::getIndexX() const
{
    if((_p.x - (int)_p.x) < 0.5)
    {
        return (int)(_p.x);
    }
    else
    {
        return  (int)(_p.x + 1);
    }
}

int Pacman::getIndexY() const
{
    if(_p.y - (int)(_p.y) < 0.5)
    {
        return (int)(_p.y);
    }
    else
    {
        return  (int)(_p.y + 1);
    }
}

void Pacman::setPos(Point P)
{
    _p = P;
}

Point Pacman::getPos() const
{
    return _p;
}

void Pacman::actuState()
{
    if(_isSuper)
    {
        _timer++;
        if(_timer > 200)
        {
            _isSuper = false;
            _timer = 0;
        }
    }
}