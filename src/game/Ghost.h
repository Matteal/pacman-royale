#ifndef GHOST_H
#define GHOST_H

#include "Character.h"
class Ghost : public Character
{
public:
    Ghost(Point P, int r, int g, int b);

    float getX() const;
    void setX(float);

    float getY() const;
    void setY(float);

    char getRepr() const;
    void setRepr(char);

    void setPos(Point);
    Point getPos() const;

protected:
    Point _p;
    int _r, _g, _b;
    char _repr;
};

#endif // GHOST_H
