#include "Pacgum.h"
using namespace std;
Pacgum::Pacgum(Point c)
{
    this->Coord = c;
    this->isEated = false;
    this->isSuper = false;
    timer = 0;
}

Pacgum::Pacgum()
{
    this->Coord = {0, 0};
    this->isEated = false;
    this->isSuper = false;
    timer = 0;
}

bool Pacgum::actu()
{
   
    timer++;
    if(timer >= 130)
    {
        isEated = false;
        timer = 0;
        return true;
    }

   return false;

}

void Pacgum::eat()
{
    timer = 0;
    isEated = true;
}

bool Pacgum::getState() const
{
    return isEated;
}

Point Pacgum::getCoord() const
{
    return Coord;
}