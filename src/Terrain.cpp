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
    for(int i = 0; i < Width; i++)
    {
        for(int j = 0; j < Height; j++)
        {
            if((i == 0|| j == 0) || (i == Width - 1 || j == Height - 1))
            {
                Grille[j * Width + i] = 0;
            }
            else
            {
                Grille[j * Width + i] = 1;
            }
            
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
                if(Grille[(j/2) * Width + i] == 0)
                {
                    line[j] = '#';
                }
                else if(Grille[(j/2) * Width + i] == 1)
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
