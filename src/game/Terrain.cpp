#include "Terrain.h"
Terrain::Terrain(int width, int height)
{
    Width = width;
    Height = height;
    Grille = new char[Width * Height];
    for(int i = 0; i < Width; i++)
    {
        for(int j = 0; j < Height; j++)
        {
            Grille[j * Width + i] = '0';
        }
    }
}

void Terrain::handTerrain()
{
    Width = 15;
    Height = 15;
    delete[] Grille;
    Grille = new char[Width * Height];

    char grilleMap[] = 
    {
        '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#',
        '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#',
        '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#',
        '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#',
        '#', ' ', '#', ' ', '#', '#', '#', '#', '#', '#', '#', ' ', '#', ' ', '#',
        '#', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', '#',
        '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#',
        '#', ' ', '#', ' ', ' ', ' ', '#', ' ', '#', ' ', '#', ' ', ' ', ' ', '#',
        '#', ' ', '#', ' ', '#', ' ', '#', '#', '#', ' ', '#', ' ', '#', ' ', '#',
        '#', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', '#',
        '#', ' ', '#', ' ', '#', '#', '#', '#', '#', '#', '#', ' ', '#', ' ', '#',
        '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#',
        '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' ', '#',
        '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#',
        '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#',
    };

    for(int i = 0; i < Width; i++)
    {
        for(int j = 0; j < Height; j++)
        {
            Grille[j * Width + i] = grilleMap[j * Width + i];
        }
    }
}

void Terrain::generateTerrain(int seed)
{
    srand(seed);

    int CellX = rand()%Width;
    int CellY = rand()%Height;

    int iteration = 0;

    while(iteration != 1)
    {
        Grille[CellY * Width + CellX] = ' ';
            /*Grille[(CellY + 1) * Width + CellX] = '#';
            Grille[(CellY - 1) * Width + CellX] = '#';
            Grille[CellY * Width + (CellX + 1)] = '#';
            Grille[CellY * Width + (CellX - 1)] = '#';*/
        
       for(int i = -1; i < 2; i++)
       {
           if(Grille[CellY * Width + (CellX+i)] == '0')
           {
                Grille[CellY * Width + (CellX+i)] = '#';
           }

           if(Grille[CellY * Width + CellX] == '0')
           {
                Grille[CellY * Width + i] = '#';
           }
       }

        iteration++;
    }
}

void Terrain::drawTerminal() const
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
        mvprintw((LINES / 4) + i, (COLS / 2) - (Width*2 / 2), line);
    }
}



Terrain::~Terrain()
{
    delete[] Grille;
    Grille = nullptr;
}
