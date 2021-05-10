#ifndef PACGUM_H
#define PACGUM_H
#include "Point.h"
#include <iostream>
class Pacgum
{
private:
	/// @param Coord 2D servant pour représenté les coordonnées du pacman
	Point Coord;

	/// @param isEated: Bool représentant si la pacgum est mangé ou non
	bool isEated = false;

	/// @param isSuper: Bool représentant si la pacgum est une super pacgum
	bool isSuper = false;

	/// @param timer: entier gérant le temps de réapparition
	int timer = 0;

public:
	/// @brief Constructeur d'une Pacgum
	/// @details
	/// Utilisation: Construit une super Pacgum a partir de ses coordonées et si c'est une super
	/// @param c: Coordonnées de la pacgum
	/// @param isSuper: Est une super Pacgum
	Pacgum(Point c, bool isSuper);

	/// @brief Constructeur par défaut
	/// @details
	/// Utilisation: Construit une pacgum par défaut,
	Pacgum();

	/// @brief Actualise une pacgum
	/// @details
	/// Utilisation: Actualise le nombre de super si generate pacgum est vrai, se réafiche si le timer est assez grand
	/// @param nbS: nombre superPAcgum
	/// @param generatePacgum: bool peut générer pacgum
	/// @param FPS: nombre de fps pour les timer
	/// @return: bool vrai si super apcgum
	bool actu(int & nbS, bool generatePacgum, int FPS);

	/// @brief Actualise une pacgum
	/// @details
	/// Utilisation: Actualise le nombre de super si generate pacgum est vrai, se réafiche si le timer est assez grand
	/// @param nbS: nombre superPAcgum
	/// @return: bool vrai si super apcgum
	bool eat(int & nbS);

	/// @brief Acesseur etat pacgum (renvoit oui si mangé)
	/// @return: isEaten Pacgum
	bool getState() const;
	/// @brief Acesseur Position Pacgum
	/// @return: Point position
	Point getCoord() const;
	/// @brief Acesseur X Pacgum
	/// @return: int Index X
	int getIndexX() const;
	/// @brief Acesseur Y Pacgum
	/// @return: int Index Y
	int getIndexY() const;

	/// @brief Accesseur super pacgum
	/// @return: bool vrai si super pacgum
	bool getSuper() const;

	/// @brief Mutateur super pacgum
	/// @param bool: vrai si super
	void setSuper(bool b);

};

#endif
