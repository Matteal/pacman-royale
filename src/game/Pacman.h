#ifndef PACMAN_H
#define PACMAN_H

#include "Character.h"


class Pacman : public Character
{
public:
    Pacman(Point p, int dir, int dirNext, int timer, bool isSuper, bool isDead,int r, int g, int b);
    Pacman();

    int _dirNext, _timer;
    bool _isSuper, _isDead;

    float getX() const;
    int getIndexX() const;
    void setX(float x);

    float getY() const;
    int getIndexY() const;
    void setY(float y);

    void setPos(Point);
    Point getPos() const;

    char getRepr();

    int getDir() const;
    void setDir(int dir);

    void actuState();
    
protected:
    Point _p;
    int _dir, _r, _g, _b;
    
    char _repr;
};

#endif
