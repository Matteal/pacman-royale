#ifndef PACMAN_H
#define PACMAN_H

#include "Character.h"

class Pacman : public Character
{
public:
    Pacman(float x, float y, int dir,int r, int g, int b);
    Pacman();

    float getX();
    int getIndexX();
    void setX(float x);

    float getY();
    int getIndexY();
    void setY(float y);

    char getRepr();

    int getDir();
    void setDir(int dir);
    
protected:
    float _x, _y;
    int _dir, _r, _g, _b;
    char _repr;
};

#endif
