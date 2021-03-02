#include "Character.h"
class Pacman : public Character
{
public:
    Pacman(int x, int y, int r, int g, int b);

    int getX();
    void setX(int);

    int getY();
    void setY(int);

    char getRepr();

protected:
    int _x, _y, _r, _g, _b;
    char _repr;
};