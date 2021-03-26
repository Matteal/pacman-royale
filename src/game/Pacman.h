#ifndef PACMAN_H
#define PACMAN_H

#include "Character.h"
#include "direction.h"

class Pacman : public Character
{
public:
    Pacman(Point p, direction dir, direction dirNext, int timer, bool isSuper, bool isDead,int r, int g, int b);
    Pacman();
    int animState = 0;
    direction _dirNext;
    int _timer;
    bool _isSuper, _isDead;

    float getX() const;
    int getIndexX() const;
    void setX(float x);

    float getY() const;
    int getIndexY() const;
    void setY(float y);

    void setPos(Point);
    Point getPos() const;
    Point getIndexPos() const;

    char getRepr();

    direction getDir() const;
    void setDir(direction dir);

    void actuState();

protected:
    Point _p;
    direction _dir;
    int _r, _g, _b;

    char _repr;
};

#endif
