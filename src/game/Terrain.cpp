#include <fstream>
#include <string>
#include <assert.h>

#include "Terrain.h"

//sleep, à supprimer
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define NC "\e[0m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define CYN "\e[0;36m"
#define REDB "\e[41m"
Terrain::Terrain(int width, int height, int seed)
{
    Width = width;
    Height = height;
    Seed = seed;
    srand(seed);
    Grille = new char[Width * Height];
    for(int i = 0; i < getWidth(); i++)
    {
        for(int j = 0; j < getHeight(); j++)
        {
            setTile(i, j, '#');
        }
    }
}

Terrain::Terrain(const std::string& copy) : Width(copy[0]+128), Height(copy[1]+128)
{
  std::cout<<"width: "<<Width<<" and height: "<<Height<<std::endl;

  Grille = new char[Width * Height];
  for(int i = 0; i < getWidth(); i++)
  {
      for(int j = 0; j < getHeight(); j++)
      {
          setTile(i, j, copy[i*getHeight() + j]);
          std::cout<<copy[i*getHeight() + j];
      }
  }
}

std::string Terrain::exportToString()
{
  std::string toExport;
  toExport.reserve(Width*Height+2);
  toExport.push_back((char) Width-128);
  toExport.push_back((char) Height-128);

  for(int i = 0; i < getWidth(); i++)
  {
      for(int j = 0; j < getHeight(); j++)
      {
          toExport.push_back(Grille[i * getWidth() + j]);
          std::cout<<Grille[i * getWidth() + j];
      }
  }
  //std::cout<<toExport;
  return toExport;
}

Terrain::~Terrain()
{
    delete[] Grille;
    Grille = nullptr;
}


Terrain::Terrain()
{
    Width = 15;
    Height = 15;
    Grille = new char[Width * Height];

    char grilleMap[] =
    {
        '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#',
        '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#',
        ' ', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', ' ',
        '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#',
        '#', ' ', '#', ' ', '#', '#', '#', '#', '#', '#', '#', ' ', '#', ' ', '#',
        '#', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', '#',
        '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#',
        '#', ' ', '#', ' ', ' ', ' ', '#', 'P', '#', ' ', '#', ' ', ' ', ' ', '#',
        '#', ' ', '#', ' ', '#', ' ', '#', '#', '#', ' ', '#', ' ', '#', ' ', '#',
        '#', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', '#',
        '#', ' ', '#', ' ', '#', '#', '#', '#', '#', '#', '#', ' ', '#', ' ', '#',
        '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#',
        ' ', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', ' ',
        '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#',
        '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#', '#',
    };

    for(int i = 0; i < getWidth(); i++)
    {
        for (int j = 0; j < getHeight(); j++)
        {
            setTile(i, j, grilleMap[j * getWidth() + i]);
        }
    }
}

char* Terrain::getGrille() const
{
  return Grille;
}

void Terrain::generateTerrain()
{
    Point P;
    assert(getWidth()%2 != 0 && getHeight()%2 !=0);
    P.x = rand()%getWidth();
    P.y = rand()%getHeight();

    cout<<P.x<<" "<<P.y<<endl;

    while(P.x % 2 != 0|| P.y % 2 != 0)
    {
        P.x = rand()%getWidth();
        P.y = rand()%getHeight();
    }
    vector<Point> possibleDirection;

    int iteration = 0;

    while(possibleDirection.size() > 0 || iteration == 0)
    {
        setTile(P.x, P.y, ' ');

        flood(P, possibleDirection);

        int randum = rand() % possibleDirection.size();
        Point nextDirection = possibleDirection[randum];
        possibleDirection.erase(possibleDirection.begin() + randum);

        cutThrough(P);

        P.x = nextDirection.x;
        P.y = nextDirection.y;

        iteration++;
    }

    setTile(P.x, P.y, ' ');

    enhancer();

}

void Terrain::flood(Point Cell, vector<Point> &possibleDirection)
{

    for(int i = -2; i < 3; i+=2)
        {

            if((Cell.x + i >= 0 && Cell.x + i < getWidth()) && (getTile(Cell.x + i, Cell.y) == '#'))
            {
                setTile(Cell.x + i, Cell.y, 'F');
                possibleDirection.push_back({Cell.x + i, Cell.y});
            }

            if((Cell.y + i >= 0 && Cell.y + i < getHeight()) && (getTile(Cell.x, Cell.y + i) == '#'))
            {
                setTile(Cell.x, Cell.y + i, 'F');
                possibleDirection.push_back({Cell.x, Cell.y + i});
            }

        }

}

void Terrain::cutThrough(Point Cell)
{
    vector<Point> canBeCut;
    if(Cell.x > 1)
    {
        if(getTile(Cell.x - 2, Cell.y) == ' ')
        {
            canBeCut.push_back({Cell.x - 1, Cell.y});
        }
    }
    if(Cell.x < getWidth() - 1)
    {

        if(getTile(Cell.x + 2, Cell.y) == ' ')
        {
            canBeCut.push_back({Cell.x + 1, Cell.y});
        }
    }

    if(Cell.y > 1)
    {
        if(getTile(Cell.x, Cell.y - 2) == ' ')
        {
            canBeCut.push_back({Cell.x, Cell.y - 1});
        }
    }

    if(Cell.y < getHeight() - 1)
    {
        if(getTile(Cell.x, Cell.y + 2) == ' ')
        {
            canBeCut.push_back({Cell.x, Cell.y + 1});
        }
    }

    if(canBeCut.size() > 0)
    {

        int randum = rand()%canBeCut.size();
        Point toModif = canBeCut[randum];
        setTile(toModif.x, toModif.y, ' ');
    }

}

void Terrain::enhancer()
{
    for(int i = 0; i < getWidth(); i++)
    {
        for(int j = 0; j < getHeight(); j++)
        {
            if(getTile(i, j) == '#')
            {
                if(countNeighbor({i, j}) > 3)
                {
                   setTile(i, j, ' ');
                }
            }
        }
    }

    vector<Point> pillier;

    for(int i = 0; i < getWidth(); i++)
    {
        for(int j = 0; j < getHeight(); j++)
        {
            if(getTile(i, j) == '#')
            {
                if(countNeighbor({i, j}) == 0)
                {
                    pillier.push_back({i, j});
                }
            }
        }
    }



    while(pillier.size() > 0)
    {
        Point pil = pillier.back();
        pillier.pop_back();


        int nNorth = countNeighbor({pil.x, pil.y + 2});
        int nSouth = countNeighbor({pil.x, pil.y - 2});
        int nEast = countNeighbor({pil.x + 2, pil.y});
        int nWest = countNeighbor({pil.x - 2, pil.y});

        Point toChange;
        if(nNorth == 0 || nNorth == 2) toChange = getNeighbor(pil, 0, 1);
        else if(nSouth == 0 || nNorth == 2) toChange = getNeighbor(pil, 1, 1);
        else if(nEast == 0 || nNorth == 2) toChange = getNeighbor(pil, 2, 1);
        else if(nWest == 0 || nNorth == 2) toChange = getNeighbor(pil, 3, 1);

        if(!(toChange.x == pil.x && toChange.y == pil.y))
        {
            setTile(toChange.x, toChange.y, '#');
        }

        // TODO: Supprimer le voisin

    }
}

int Terrain::countNeighbor(Point P) const
{
    if(P.x < 0) P.x = getWidth() - P.x;
    else if(P.x >= getWidth()) P.x = P.x - getWidth();

    if(P.y < 0) P.y = getHeight() - P.y;
    else if(P.y >= getWidth()) P.y = P.y - getHeight();
    int count = 0;
    for(int x = -1; x < 2; x++)
    {
        for(int y = -1; y < 2; y++)
        {
            int jt = P.y + y;
            int it = P.x + x;
            if(!(x == 0 && y == 0))
            {
                if(it < 0) it = getWidth() + it;
                else if(it >= getWidth()) it = it - getWidth();
                if(jt < 0) jt = getHeight() + jt;
                else if(jt >= getWidth()) jt = jt - getHeight();

                if(getTile(it, jt) == '#') count++;
            }

        }
    }

    return count;

}

Point Terrain::getNeighbor(Point P, int dir, int dist)
{
    if(dir == 0) // NORD
    {
        P.x = P.x - dist;
        if(P.x < 0) P.x = getWidth() - P.x;

    }
    else if(dir == 1) // SUD
    {
        P.x = P.x + dist;
        if(P.x >= getWidth()) P.x = P.x - getWidth();

    }
    else if(dir == 2)
    {
        P.y = P.y - dist;
        if(P.y < 0) P.y = getHeight() - P.y;

    }
    else if(dir == 3)
    {
        P.y = P.y + dist;
        if(P.y >= getHeight()) P.y = P.y - getHeight();
    }

    return {P.x, P.y};
}

void Terrain::setTile(int x, int y, char c)
{
    Grille[y * getWidth() + x] = c;
}

char Terrain::getTile(int x, int y) const
{
    return Grille[y * getWidth() + x];
}

int Terrain::getWidth() const
{
    return Width;
}

int Terrain::getHeight() const
{
    return Height;
}

//Terrain::~Terrain()
void Terrain::createTerrainFromFile(const char* filename)
{
    // '#' = mur, ' ' = vide, '·' = pastille, 'C' = pacman

    string line;
    ifstream path(filename);
    if (path.is_open())
    {
        int nLine = 0;
        while (getline(path, line))
        {
            for(int i = 0; i < line.length(); i++)
            {
                Grille[nLine * Width + i] = line[i];
            }
            nLine++;
        }
        path.close();
    }
    else
        throw string("Impossible d'ouvrir le fichier ") + filename;
}
