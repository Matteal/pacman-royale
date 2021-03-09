#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>
#include <array>

class Character
{
public:

    /**
    @brief récupère le
    \param x : indice horizontale du pixel (unsigned int)
  	\param y : indice verticale du pixel(unsigned int)
	*/
    virtual int getX() = 0;
    virtual void setX(int) = 0;

    virtual int getY() = 0;
    virtual void setY(int) = 0;

    virtual char getRepr() = 0;

protected:
    int _x, _col;
    char _repr;
};

#endif // CHARACTER_H
