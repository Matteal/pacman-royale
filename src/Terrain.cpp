#include "Terrain.h"
Terrain::Terrain(int width, int height)
{
    Width = width;
    Height = height;
    Grille = new unsigned char[Width * Height];
    for(int i = 0; i < Width; i++)
    {
        for(int j = 0; j < Height; j++)
        {
            Grille[j * Width + i] = 0;
        }
    }
}

void Terrain::handTerrain()
{
    Width = 15;
    Height = 15;
    delete[] Grille;
    Grille = new unsigned char[Width * Height];

    int grilleMap[] = 
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
        0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0,
        0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0,
        0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0,
        0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
        0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0,
        0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0,
        0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0,
        0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0,
        0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0,
        0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };

    for(int i = 0; i < Width; i++)
    {
        for(int j = 0; j < Height; j++)
        {
            Grille[j * Width + i] = grilleMap[j * Width + i];
        }
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
                if(Grille[i * Height + j/2] == 0)
                {
                    line[j] = '#';
                }
                else if(Grille[i * Height + j/2] == 1)
                {
                    line[j] = '-';
                }
                
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
