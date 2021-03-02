#include "Character.h"
class Ghost : public Character
{
public:
    Ghost(int x, int y, int r, int g, int b);

    int getX();
    void setX(int);

    int getY();
    void setY(int);

    char getRepr();
    void setRepr(char);

protected:
    int _x, _y, _r, _g, _b;
    char _repr;
};