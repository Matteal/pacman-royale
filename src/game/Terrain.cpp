#include "Terrain.h"
Terrain::Terrain(int width, int height, int seed)
{
    Width = width;
    Height = height;
    Seed = seed;
    srand(seed);
    Grille = new char[Width * Height];
    for(int i = 0; i < Width; i++)
    {
        for(int j = 0; j < Height; j++)
        {
            Grille[j * Width + i] = '#';
        }
    }
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

    for(int i = 0; i < Width; i++)
    {
        for(int j = 0; j < Height; j++)
        {
            Grille[j * Width + i] = grilleMap[j * Width + i];
        }
    }
}

void Terrain::generateTerrain()
{
    Point P;

    assert(Width%2 != 0 && Height%2 !=0);
    P.x = rand()%Width;
    P.y = rand()%Height;
    
    while(P.x % 2 != 0|| P.y % 2 != 0)
    {
        P.x = rand()%Width;
        P.y = rand()%Height;
    }
    vector<Point> possibleDirection;

    int iteration = 0;

    while(possibleDirection.size() > 0 || iteration == 0)
    {
        Grille[P.y * Width + P.x] = ' ';
        
        flood(P, possibleDirection);
        
        int randum = rand() % possibleDirection.size();
        Point nextDirection = possibleDirection[randum];
        possibleDirection.erase(possibleDirection.begin() + randum);

        cutThrough(P);
        
        P.x = nextDirection.x;
        P.y = nextDirection.y;


        iteration++;
    }

    Grille[P.y * Width + P.x] = ' ';

    enhancer();
   
}

void Terrain::flood(Point Cell, vector<Point> &possibleDirection)
{
   
    for(int i = -2; i < 3; i+=2)
        {
            
            if((Cell.x + i >= 0 && Cell.x + i < Width) && (Grille[Cell.y * Width + (Cell.x+i)] == '#'))
            {
                Grille[Cell.y * Width + (Cell.x+i)] = 'F';
                possibleDirection.push_back({Cell.x + i, Cell.y});
            }

            if((Cell.y + i >= 0 && Cell.y + i < Height) && (Grille[(Cell.y + i) * Width + Cell.x] == '#'))
            {
                Grille[(Cell.y + i) * Width + Cell.x] = 'F';
                possibleDirection.push_back({Cell.x, Cell.y + i});
            }

        }

}

void Terrain::cutThrough(Point Cell)
{
    vector<Point> canBeCut;
    if(Cell.x > 1)
    {
        if(Grille[Cell.y * Width + (Cell.x - 2)] == ' ')
        {
            canBeCut.push_back({Cell.x - 1, Cell.y});
        }
    }
    if(Cell.x < Width - 1)
    {
        
        if(Grille[Cell.y * Width + (Cell.x + 2)] == ' ')
        {
            canBeCut.push_back({Cell.x + 1, Cell.y});
        }
    }

    if(Cell.y > 1)
    {
        if(Grille[(Cell.y - 2) * Width + Cell.x] == ' ')
        {
            canBeCut.push_back({Cell.x, Cell.y - 1});
        }
    }

    if(Cell.y < Height - 1)
    {
        if(Grille[(Cell.y + 2) * Width + Cell.x] == ' ')
        {
            canBeCut.push_back({Cell.x, Cell.y + 1});
        }
    }

    if(canBeCut.size() > 0)
    {
        
        int randum = rand()%canBeCut.size();
        Point toModif = canBeCut[randum];
        Grille[toModif.y * Width + toModif.x] = ' ';
    }
    
}

void Terrain::enhancer()
{
    for(int i = 0; i < Width; i++)
    {
        for(int j = 0; j < Height; j++)
        {
            if(Grille[j * Width + i] == '#')
            {
                if(countNeighbor({i, j}) > 3)
                {
                    Grille[j * Width + i] = ' ';
                }
            }
        }
    }

    vector<Point> pillier;

    for(int i = 0; i < Width; i++)
    {
        for(int j = 0; j < Height; j++)
        {
            if(Grille[j * Width + i] == '#')
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
            Grille[toChange.y * Width + toChange.x] = '#';
        }

        // TODO: Supprimer le voisin

    }

    int c = 0;

    for(int i = 0; i < Width; i++)
    {
        for(int j = 0; j < Height; j++)
        {
            if(Grille[j * Width + i] == '#')
            {
                if(countNeighbor({i, j}) == 0)
                {
                    c++;
                }
            }
        }
    }

    cout<<c<<endl;
    


}

int Terrain::countNeighbor(Point P) const
{
    if(P.x < 0) P.x = Width - P.x;
    else if(P.x >= Width) P.x = P.x - Width;
    if(P.y < 0) P.y = Height - P.y;
    else if(P.y >= Width) P.y = P.y - Height;
    int count = 0;
    for(int x = -1; x < 2; x++)
    {
        for(int y = -1; y < 2; y++)
        {
            int jt = P.y + y;
            int it = P.x + x;
            if(!(x == 0 && y == 0))
            {
                if(it < 0) it = Width - it;
                else if(it >= Width) it = it - Width;
                if(jt < 0) jt = Height - jt;
                else if(jt >= Width) jt = jt - Height;

                if(Grille[jt * Width + it] == '#') count++;
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
        if(P.x < 0) P.x = Width - P.x;

    }
    else if(dir == 1) // SUD
    {
        P.x = P.x + dist;
        if(P.x >= Width) P.x = P.x - Width;

    }
    else if(dir == 2)
    {
        P.y = P.y - dist;
        if(P.y < 0) P.y = Height - P.y;

    }
    else if(dir == 3)
    {
        P.y = P.y + dist;
        if(P.y >= Height) P.y = P.y - Height;
    }

    return {P.x, P.y};
}

void Terrain::drawTerminal(int x, int y) const
{   
    char line[Width*2+1];
    for(int i = 0; i < Width; i++)
    {
        for(int j = 0; j < Width*2; j++)
        {
            if(j%2 == 0)
            {
                line[j] = Grille[i * Height + j/2];

            }
            else
            {
                line[j] = ' ';
            }
        }
        line[Width*2] = '\0';
        mvprintw((LINES / 2) - i + (Width /2), (COLS / 2) - (Width*2 / 2), line);
    }
}



Terrain::~Terrain()
{
    delete[] Grille;
    Grille = nullptr;
}
