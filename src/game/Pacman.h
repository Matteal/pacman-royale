#ifndef PACMAN_H
#define PACMAN_H

#include "direction.h"
#include "iostream"
#include "Point.h"

class Pacman
{
public:
	/// @brief Constructeur d'un pacman
	/// @details
	/// Utilisation: Construit un pacman prenant toutes les données suivante
	/// @param p: point position
	/// @param dir: direction
	/// @param dirNext: direction futur
	/// @param timer: int timer pour super/animations
	/// @param isSuper: Si est super pacman
	/// @param state: vivant / mort / gagnant
	/// @param isPlayer: est un joueur
	/// @param isGhost: est un fantome
	/// @param r: couleur
	/// @param g: couleur
	/// @param b: couleur
	Pacman(Point p, direction dir, direction dirNext, int timer, bool isSuper, int state, bool isPlayer, bool isGhost, int r, int g, int b);
	
	/// @brief Constructeur d'un pacman par defaut
	/// @details
	/// Utilisation: Construit un pacman par defaut, ni fantome, ni joueur
	Pacman();

	/// @brief Enum direction pacman suivante
	direction _dirNext;
	
	/// @brief entier timer super/animation
	int _timer;

	/// @brief bool  vrai si super
	bool _isSuper;

	/// @brief int 0 = partie en cours; -1 = mort, 1 = gagnant
	int _state; 

	/// @brief compteur animation pour le render
	int compteurAnimation[2];

	/// @brief son a jouer pour le render
	int _playSound = 0;

	/// @brief Accesseur position x
	/// @return: float
	float getX() const;

	/// @brief Accesseur position tableau x
	/// @return: int
	int getIndexX() const;

	/// @brief mutateur position x 
	/// @param x: float nouvelle position x
	void setX(float x);

	/// @brief Accesseur position y
	/// @return: float
	float getY() const;

	/// @brief Accesseur position tableau y
	/// @return: int
	int getIndexY() const;

	/// @brief mutateur position y
	/// @param y:float nouvelle position y
	void setY(float y);

	/// @brief mutateur position
	/// @param Point: Objet Point nouvelles coordonnées
	void setPos(Point);

	/// @brief Acesseur position
	/// @return: Point position
	Point getPos() const;

	/// @brief Accesseur position tableau index
	/// @return: Point position tableau
	Point getIndexPos() const;

	/// @brief Accesseur direction pacman
	/// @return: Direction
	direction getDir() const;

	/// @brief Mutateur direction
	/// @param dir: enum direction
	void setDir(direction dir);

	/// @brief Mutateur joueur
	/// @param t: vrai si joueur
	void setPlayer(bool t);

	/// @brief Mutateur fantome
	/// @param t: vrai si fantome
	void setGhost(bool t);

	/// @brief Accesseur joueur
	/// @return: renvoit vrai si joueur
	bool getPlayer() const;

	/// @brief Accesseur fantome
	/// @return: renvoit vrai si fantome
	bool getGhost() const;

	/// @brief Actualise l'état du pacman
	/// @details
	/// Utilisation: A utiliser quand le pacman est sur une pacgum
	/// @param FPS: nombre de FPS pour le timer
	void actuState(int FPS);

	/// @brief Mutateur fantome
	/// @param t: vrai si fantome
	void setColor(int r);

	/// @brief Acesseur couleur fantome
	/// @return: int fantome
	int getColor() const;

	/// @brief Mutateur index liste pacman
	/// @param i: index du tableau
	void setIndex(int i);

	/// @brief Acesseur index tableau
	/// @return: entier
	int getIndex() const;

protected:
	/// @brief: Position
	Point _p;

	/// @brief: Direction
	direction _dir;

	/// @brief: couleurs
	int _r, _g, _b;
	
	/// @brief: Index dans le tableau
	int _i;

	/// @brief: Bool représentant le joueur
	bool _isPlayer;

	/// @brief: bool représentant si le pacman est un fantome
	bool _isGhost;
};

#endif
