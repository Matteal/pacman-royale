#include "Pacman.h"

Pacman::Pacman(Point P = Point(0, 0), direction dir = UP, direction dirNext = UP, int timer = 0, bool isSuper = false, int state = 0, bool isPlayer = false, bool isGhost = false, int r=255, int g=255, int b=255) :  _dirNext(dirNext),  _timer(timer),_isSuper(isSuper), _state(state), _isPlayer(isPlayer), _isGhost(isGhost), _p(P), _dir(dir),_r(r), _g(g), _b(b)
{
    _repr = '@';
    for(int i = 0; i < 2; i++)
    {
        compteurAnimation[i] = 0;
    }
}

Pacman::Pacman()
{
    _p = Point(0, 0);
    _dir = UP;
    _dirNext = UP;
    _isSuper = false;
    //_isDead = false;
    _state= 0;
    _isPlayer = false;
    _isGhost = false;
    _timer = 0;

    for(int i = 0; i < 2; i++)
    {
        compteurAnimation[i] = 0;
    }

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

direction Pacman::getDir() const
{
    return _dir;
}
void Pacman::setDir(direction dir)
{
    _dir = dir;

}

int Pacman::getIndexX() const
{
    if(_dir == RIGHT)
    {
        if((_p.x - (int)_p.x) < 0.9)
        {
            return (int)(_p.x);
        }
        else
        {
            return  (int)(_p.x + 1);
        }
    }
    else
    {
        if((_p.x - (int)_p.x) < 0.1)
        {
            return (int)(_p.x);
        }
        else
        {
            return  (int)(_p.x + 1);
        }
    }

}

int Pacman::getIndexY() const
{
    if(_dir == UP)
    {
        if(_p.y - (int)(_p.y) < 0.9)
        {
            return (int)(_p.y);
        }
        else
        {
            return  (int)(_p.y + 1);
        }
    }
    else
    {
        if(_p.y - (int)(_p.y) < 0.1)
        {
            return (int)(_p.y);
        }
        else
        {
            return  (int)(_p.y + 1);
        }
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

Point Pacman::getIndexPos() const
{
    return Point(getIndexX(), getIndexY());
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

void Pacman::setPlayer(bool t)
{
    _isPlayer = t;
}

void Pacman::setGhost(bool t)
{
    _isGhost = t;
}

bool Pacman::getPlayer() const {return _isPlayer;}
bool Pacman::getGhost() const {return _isGhost;}

void Pacman::setColor(int r){_r = r;}
int Pacman::getColor() const {return _r;}