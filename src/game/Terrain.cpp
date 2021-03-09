#include <fstream>
#include <string>
#include <assert.h>

#include "Terrain.h"

#define NC "\e[0m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define CYN "\e[0;36m"
#define REDB "\e[41m"

Terrain::Terrain(int width, int height)
{
    _width = width;
    _height = height;
    _grille = new unsigned char[_width * _height];
    for (int i = 0; i < _width; i++)
    {
        for (int j = 0; j < _height; j++)
        {
            _grille[j * _width + i] = 0;
        }
    }
}

Terrain::~Terrain()
{
    delete[] _grille;
    _grille = nullptr;
}

void Terrain::hardcodeTerrain()
{
    _width = 15;
    _height = 15;
    delete[] _grille;
    _grille = new unsigned char[_width * _height];

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

    for (int i = 0; i < _width; i++)
    {
        for (int j = 0; j < _height; j++)
        {
            _grille[j * _width + i] = grilleMap[j * _width + i];
        }
    }
}

void Terrain::drawToTerminal() const
{
    char line[_width * 2 + 1];
    for (int i = 0; i < _width; i++)
    {
        for (int j = 0; j < _width * 2; j++)
        {
            if (j % 2 == 0)
            {
                if (_grille[i * _height + j / 2] == 0)
                {
                    line[j] = '#';
                }
                else if (_grille[i * _height + j / 2] == 1)
                {
                    line[j] = '-';
                }
            }
            else
            {
                line[j] = ' ';
            }
        }
        line[_width * 2] = '\0';
        mvprintw((LINES / 4) + i, (COLS / 2) - (_width * 2 / 2), line);
    }
}

void Terrain::createTerrainFromFile(char filename[])
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
                _grille[nLine * _width + i] = line[i];
            }
            nLine++;
        }
        path.close();
    }
    else
        throw string("Impossible d'ouvrir le fichier ") + filename;
}