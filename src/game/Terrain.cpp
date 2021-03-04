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

    cout<<P.x<<" "<<P.y<<endl;

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
