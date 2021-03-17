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
    virtual float getX() = 0;
    virtual void setX(float) = 0;

    virtual float getY() = 0;
    virtual void setY(float) = 0;

    virtual char getRepr() = 0;

protected:
    float _x, _col;
    char _repr;
};

#endif // CHARACTER_H
