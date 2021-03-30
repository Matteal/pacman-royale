#include "Pacgum.h"
using namespace std;
Pacgum::Pacgum(Point c, bool isSuper)
{
    this->Coord = c;
    this->isEated = false;
    this->isSuper = isSuper;
    timer = 0;
}

Pacgum::Pacgum()
{
    this->Coord = {0, 0};
    this->isEated = false;
    this->isSuper = false;
    timer = 0;
}

bool Pacgum::actu(int & nbS)
{
   
    timer++;
    if(timer >= 30)
    {
        isEated = false;
        int r = rand()%100;
        if(r < 1 && nbS > 0) 
        {
            isSuper = true;
            nbS--;
        }
        timer = 0;
        return true;
    }

   return false;

}

bool Pacgum::eat(int & nbS)
{
    timer = 0;
    isEated = true;
    if(isSuper)
    {
        isSuper = false;
        nbS++;
        return true;
    }
    return false;
}

bool Pacgum::getState() const
{
    return isEated;
}

bool Pacgum::getSuper() const
{
    return isSuper;
}

void Pacgum::setSuper(bool b)
{
    isSuper = b;
}

Point Pacgum::getCoord() const
{
    return Coord;
}

int Pacgum::getIndexX() const
{
    return (int)Coord.x;
}

int Pacgum::getIndexY() const
{
    return (int)Coord.y;
}