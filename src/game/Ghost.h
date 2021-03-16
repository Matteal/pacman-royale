#ifndef GHOST_H
#define GHOST_H

#include "Character.h"
class Ghost : public Character
{
public:
    Ghost(float x, float y, int r, int g, int b);

    float getX();
    void setX(float);

    float getY();
    void setY(float);

    char getRepr();
    void setRepr(char);

protected:
    float _x, _y;
    int _r, _g, _b;
    char _repr;
};

#endif // GHOST_H
