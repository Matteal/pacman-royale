#ifndef PACMAN_H
#define PACMAN_H

#include "Character.h"
#include "direction.h"
#include "iostream"

class Pacman : public Character
{
public:
    Pacman(Point p, direction dir, direction dirNext, int timer, bool isSuper, int state, bool isPlayer, bool isGhost, int r, int g, int b);
    Pacman();
    int _animState = 0;
    int indice;
    direction _dirNext;
    int _timer;
    bool _isSuper;
    int _state; // 42 = pause; 0 = partie en cours; 43 = début 

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

    void setPlayer(bool t);
    void setGhost(bool t);

    bool getPlayer() const;
    bool getGhost() const;

    void actuState();

    void setColor(int r);
    int getColor() const;

protected:
    Point _p;
    direction _dir;
    int _r, _g, _b;
    bool _isPlayer, _isGhost;

    char _repr;
};

#endif
