#ifndef TERRAIN_H
#define TERRAIN_H

#include <iostream>
#include <array>
#include <string>
#include <stdlib.h>
#include <vector>
#include <assert.h>
#include <string>
#include "Point.h"
#include "direction.h"

using namespace std;



class Terrain
{
private:
	int Width = 0;
	int Height = 0;
	int Seed = 0;
	char * Grille = nullptr;

	void flood(Point cell, vector<Point> & possibleDirection);
	void cutThrough(Point Cell);
	void enhancer();
	int countNeighbor(Point P) const;
	char tileType(Point P);


public:
	/// @brief Constructeur par argument
	/// @details
	/// Utilisation: Construit un Terrain avec les param
	/// @param width: largeur du terrain
	/// @param height: hauteur du terrain
	/// @param seed: graine du terrain
	Terrain(int width, int height, int seed);

	/// @brief Crée un terrain généré procéduralement
	void generateTerrain();

	/// @brief Mutateur case terrain
	/// @param x: index X de la case
	/// @param y: index Y de la case
	/// @param c: nouvelle valeur de la case
	void setTile(int x, int y, char c);

	/// @brief Acesseur case terrain
	/// @return valeur(char) de la case
	char getTile(int x, int y) const;

	/// @brief Acesseur Case terrain
	/// @return valeur (char) de la case 
	char getTile(Point P) const;

	/// @brief Acesseur Largeur terrain
	/// @return entier de la largeur
	int getWidth() const;

	/// @brief Acesseur hauteur terrain
	/// @return entier de la hauteur
	int getHeight() const;

	/// @brief Renvoit un point libre du terrain
	/// @return Point
	Point randomPointEmpty();

	/// @brief Acesseur Valeur voisin case
	/// @details 
	/// Renvoit la valeur du voisin de direction dir et de distance dist
	/// @param P: Position de la case
	/// @param dir: direction du voisin
	/// @param dist: distance du voisin
	/// @return valeur(char) du voisin
	char getNeighborTile(Point P, direction dir, int dist);

	/// @brief Acesseur Position voisin case
	/// @details 
	/// Renvoit la Position du voisin de direction dir et de distance dist
	/// @param P: Position de la case
	/// @param dir: direction du voisin
	/// @param dist: distance du voisin
	/// @return Poistion (Point) du voisin
	Point getNeighbor(Point P, direction dir, int dist);

	/// @brief Test si si la case est dans la bordure de distance du terrain
	/// @details 
	/// Renvoit la vrai si P est a DISTANCE cases d'une bordure
	/// @param P: Position de la case
	/// @param distance: taille de la bordure
	/// @return vrai si est a la bordure, faux sinon
	bool isInBordure(Point P, int distance);

	/// @brief Destructeur par défaut
	/// @details
	/// Utilisation: Détruit Terrain
	~Terrain();
};
#endif // TERRAIN_H
