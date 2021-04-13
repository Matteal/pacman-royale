#ifndef POINT_H
#define POINT_H

class Point
{
    public:
    float x, y;
    Point();
    Point(float x, float y);
    ~Point();

    Point operator=(const Point & P);

};

Point operator+(const Point & P0, const Point & P1);
Point operator-(const Point & P0, const Point & P1);
Point operator*(const Point & P0, const Point & P1);

Point operator/(const Point & P, float div);
Point operator*(const Point & P, float mul);
bool operator==(const Point & P0, const Point & P1);

bool operator!=(const Point & P0, const Point & P1);

#endif
