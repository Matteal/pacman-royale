#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>
#include <array>
#include "Point.h"

class Character
{
public:
	/**
	@brief récupère le
	\param P : Position en Point(float) de l'objet
	*/
	virtual float getX() const = 0;
	virtual void setX(float) = 0;

	virtual float getY() const = 0;
	virtual void setY(float) = 0;

	virtual void setPos(Point) = 0;
	virtual Point getPos() const = 0;

	virtual char getRepr() = 0;

protected:
	Point _p;
	char _repr;
};

#endif
