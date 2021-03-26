#include <fstream>
#include <string>
#include <assert.h>


#include "Terrain.h"

#define NC "\e[0m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define CYN "\e[0;36m"
#define REDB "\e[41m"
Terrain::Terrain(int width, int height, int seed)
{
    Width = width + 1;
    Height = height + 1;
    assert(Width > 2);
    assert(Height > 2);
    Seed = seed;
    srand(seed); // Initialisation random
    Grille = new char[Width * Height]; // initialisation pointeur grille
    for(int i = 0; i < getWidth(); i++)
    {
        for(int j = 0; j < getHeight(); j++)
        {
            setTile(i, j, '#'); // initialisation case #
        }
    }
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

void Terrain::generateTerrain()
{
    Point P; // Cellule de départ
    assert(getWidth()%2 != 0 && getHeight()%2 !=0);
    P.x = rand()%getWidth();
    P.y = rand()%getHeight();

    //On prend un point pair random

    while((int)P.x % 2 != 0|| (int)P.y % 2 != 0)
    {
        P.x = rand()%getWidth();
        P.y = rand()%getHeight();
    }
    vector<Point> possibleDirection; // Tableau dynamique des cases a traiter

    bool start = false;

    while(possibleDirection.size() > 0 || !start)
    {
        setTile(P.x, P.y, ' '); // On transforme la case actuel en chemin

        flood(P, possibleDirection); // On ajoute les cases adjascente a la liste a traiter

        int randum = rand() % possibleDirection.size();
        Point nextDirection = possibleDirection[randum];
        possibleDirection.erase(possibleDirection.begin() + randum); // on choisit la case a traité au hasard

        cutThrough(P); // on la relie au chemin (' ') le plus proche

        P.x = nextDirection.x; //on passe sur cette case pour la prochaine itération
        P.y = nextDirection.y;

        start = true;
    }

    setTile(P.x, P.y, ' '); // On transforme la dernière case en vide

    char * temp = new char[(getWidth() - 1) * (getHeight() - 1)]; // Supression des bordures pour simplifier la transmission de données serveurs

    for(int i = 0; i < getWidth() - 1; i++)
    {
        for(int j = 0; j < getHeight() - 1; j++)
        {
            temp[j * (getWidth() - 1) + i] = getTile(i+1, j+1);
        }
    }

    delete[] Grille; // Supression de l'ancienne grille
    Grille = temp; // On lui donne les cases de la nouvelles

    Height = Height - 1; // ajustement longueur largeur
    Width = Width - 1;

    enhancer(); // Suppression des impasses




}

void Terrain::flood(Point Cell, vector<Point> &possibleDirection)
{

    for(int i = -2; i < 3; i+=2) // On check tout les voisins de la cellule actuel
        {

            if((Cell.x + i >= 0 && Cell.x + i < getWidth()) && (getTile(Cell.x + i, Cell.y) == '#')) // Si mur
            {
                setTile(Cell.x + i, Cell.y, 'F'); // On l'ajoute aux cases a traiter
                possibleDirection.push_back({Cell.x + i, Cell.y});
            }

            if((Cell.y + i >= 0 && Cell.y + i < getHeight()) && (getTile(Cell.x, Cell.y + i) == '#')) //Même chose maise n Y
            {
                setTile(Cell.x, Cell.y + i, 'F');
                possibleDirection.push_back({Cell.x, Cell.y + i});
            }

        }

}

void Terrain::cutThrough(Point Cell)
{
    vector<Point> canBeCut; // Cellule coupable autour de la cellule actuel
    if(Cell.x > 1)
    {
        if(getTile(Cell.x - 2, Cell.y) == ' ') // Si il y a un chemin a gauche, on ajoute
        {
            canBeCut.push_back({Cell.x - 1, Cell.y});
        }
    }
    if(Cell.x < getWidth() - 1)
    {

        if(getTile(Cell.x + 2, Cell.y) == ' ') // même chose a droite
        {
            canBeCut.push_back({Cell.x + 1, Cell.y});
        }
    }

    if(Cell.y > 1)
    {
        if(getTile(Cell.x, Cell.y - 2) == ' ') // même chose en bas
        {
            canBeCut.push_back({Cell.x, Cell.y - 1});
        }
    }

    if(Cell.y < getHeight() - 1)
    {
        if(getTile(Cell.x, Cell.y + 2) == ' ') // même chose en haut
        {
            canBeCut.push_back({Cell.x, Cell.y + 1});
        }
    }

    if(canBeCut.size() > 0) // si il y a un chemin
    {

        int randum = rand()%canBeCut.size(); // on choisit un des chemin a coupé au hasard
        Point toModif = canBeCut[randum]; //
        setTile(toModif.x, toModif.y, ' '); // on ccoupe le mur
    }

}

void Terrain::enhancer()
{
    for(int i = 0; i < getWidth(); i++)
    {
        for(int j = 0; j < getHeight(); j++) // on parcour le tablal
        {
            if(getTile(i, j) == '#') // si un mur
            {
                if(countNeighbor({(float)i, (float)j}) > 3) // a plus de trois voisin
                {
                   setTile(i, j, ' '); // COUPER, DECALER, RAIDEN
                }
            }
        }
    }

    vector<Point> pillier; // On cherche les mur seul

    for(int i = 0; i < getWidth(); i++)
    {
        for(int j = 0; j < getHeight(); j++)
        {
            if(getTile(i, j) == '#')
            {
                if(countNeighbor({(float)i, (float)j}) == 0) // On ajoute les murs avec 0 voisins
                {
                    pillier.push_back({(float)i, (float)j});
                }
            }
        }
    }



    while(pillier.size() > 0) // On parcour les pilliers
    {
        Point pil = pillier.back();
        pillier.pop_back(); // on prend le dernier


        int nNorth = countNeighbor({pil.x, pil.y + 2}); // on compte les voisins de ses voisins
        int nSouth = countNeighbor({pil.x, pil.y - 2});
        int nEast = countNeighbor({pil.x + 2, pil.y});
        int nWest = countNeighbor({pil.x - 2, pil.y});

        Point toChange;
        if(nNorth == 0 || nNorth == 2) toChange = getNeighbor(pil, UP, 1); //Si il y a 0 ou 2  voisin on prend le voisin
        else if(nWest == 0 || nNorth == 2) toChange = getNeighbor(pil, LEFT, 1);
        else if(nSouth == 0 || nNorth == 2) toChange = getNeighbor(pil, DOWN, 1);
        else if(nEast == 0 || nNorth == 2) toChange = getNeighbor(pil, RIGHT, 1);


        if(!(toChange.x == pil.x && toChange.y == pil.y)) // si ce n'est pas le même qu'au départ ont les relies
        {
            setTile(toChange.x, toChange.y, '#');
        }

        // TODO: Supprimer le voisin

    }

    for(int i = 0; i < getWidth(); i++)
    {
        for(int j = 0; j < getHeight(); j++)
        {
            if(getTile(i, j) != ' ')
            {
                setTile(i, j, tileType({(float)i, j}));
            }
        }
    }
}

int Terrain::countNeighbor(Point P) const
{
    if(P.x < 0) P.x = getWidth() - P.x;
    else if(P.x >= getWidth()) P.x = P.x - getWidth(); // si sort du terrain on recup

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

char Terrain::tileType(Point P)
{
    char tile;
    int count = 0;
    char neighborUP    = getTile(getNeighbor(P, UP, 1));
    char neighborDOWN  = getTile(getNeighbor(P, DOWN, 1));
    char neighborLEFT  = getTile(getNeighbor(P, LEFT, 1));
    char neighborRIGHT = getTile(getNeighbor(P, RIGHT, 1));

    if(neighborUP != ' ')
    {
        count++;
    }
    if(neighborDOWN != ' ')
    {
        count++;
    }
    if(neighborLEFT != ' ')
    {
        count++;
    }
    if(neighborRIGHT != ' ')
    {
        count++;
    }

    if(count == 4)
    {
        return '+'; // Coin ouvert
    }
    else if(count == 3)
    {
        if(neighborUP == ' ') return 'T'; // T
        else if(neighborDOWN == ' ') return 'D'; // T inversé
        else if(neighborLEFT == ' ') return 'L'; // T vers la droite
        else return 'R'; // T vers la gauche
    }
    else if(count == 2)
    {
        if(neighborUP == ' ' && neighborDOWN == ' ') return '='; // Ligne horizontale
        else if(neighborLEFT == ' ' && neighborRIGHT == ' ') return '|'; // Ligne verticale
        else if(neighborUP == ' ' && neighborLEFT == ' ') return '('; // L bas droite
        else if(neighborUP == ' ' && neighborRIGHT == ' ') return 'G'; // L bas gauche
        else if(neighborDOWN == ' ' && neighborRIGHT == ' ') return ']'; //L Haut Gauche
        else return '['; // L Haut Droit;
    }
    else if(count == 1)
    {
        if(neighborUP != ' ') return 'v'; // Haut ouvert
        else if(neighborDOWN != ' ') return '^'; // Bas Ouvert
        else if(neighborLEFT != ' ') return '>'; // gauche ouverte
        else return '<'; //droite ouverte
    }
    else return '#';
    return tile;
}

Point Terrain::getNeighbor(Point P, direction dir, int dist)
{
  switch(dir)
  {
    case UP:
      P.y = P.y + dist;
      if(P.y >= getHeight()) P.y = P.y - getHeight();
      break;
    case LEFT:
      P.x = P.x - dist;
      if(P.x < 0) P.x = getWidth() + P.x;
      break;
    case DOWN:
      P.y = P.y - dist;
      if(P.y < 0) P.y = getHeight() + P.y;
      break;
    case RIGHT:
      P.x = P.x + dist;
      if(P.x >= getWidth()) P.x = P.x - getWidth();
      break;
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

char Terrain::getTile(Point P) const
{
    return Grille[(int)P.y * getWidth() + (int)P.x];
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
            for(int i = 0; i < (int)line.length(); i++)
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

char Terrain::getNeighborTile(Point P, direction dir, int dist)
{
  switch(dir)
  {
    case UP:
      P.y = P.y + dist;
      if(P.y >= getHeight()) P.y = P.y - getHeight();
      break;
    case LEFT:
      P.x = P.x - dist;
      if(P.x < 0) P.x = getWidth() + P.x;
      break;
    case DOWN:
      P.y = P.y - dist;
      if(P.y < 0) P.y = getHeight() + P.y;
      break;
    case RIGHT:
      P.x = P.x + dist;
      if(P.x >= getWidth()) P.x = P.x - getWidth();
      break;
  }
  return getTile(P.x, P.y);
}
