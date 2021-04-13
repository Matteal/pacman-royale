#include "Point.h"
#include <iostream>

using namespace std;

Point::Point(float X, float Y)
{
	x = X;
	y = Y;
}

Point::Point()
{
	x = 0;
	y = 0;
}

Point operator+(const Point & P0, const Point & P1)
{
	Point R;
	R.x = P0.x + P1.x;
	R.y = P0.y + P1.y;
	return R;
}

Point operator-(const Point & P0, const Point & P1)
{
	Point R;
	R.x = P0.x - P1.x;
	R.y = P0.y - P1.y;
	return R;
}

Point Point::operator=(const Point & P)
{
	x = P.x;
	y = P.y;

	return *this;
}

Point operator*(const Point & P0, const Point & P1)
{
	Point R;
	R.x = P0.x * P1.x - P0.y * P1.y;
	R.y = P0.x * P1.y + P0.y * P1.x;
	return R;
}

Point operator/(const Point & P, float div)
{
	Point R;
	R.x = P.x / div;
	R.y = P.y / div;
	return R;
}

Point operator*(const Point & P, float mul)
{
	Point R;
	R.x = P.x * mul;
	R.y = P.y * mul;
	return R;
}

bool operator==(const Point & P0, const Point & P1)
{
	return ((P0.x == P1.x) && (P0.y == P1.y));
}

bool operator!=(const Point & P0, const Point & P1)
{
	return ((P0.x != P1.x) || (P0.y != P1.y));
}

Point::~Point()
{

}
