#ifndef POINT_H
#define POINT_H

#include <math.h> 
#include <iostream>
class Point
{
    public:
    /// @brief float position x
    float x;
    /// @brief float position y
    float y;

    /// @brief Constructeur par défaut
	/// @details
	/// Utilisation: Construit un point par défaut
    Point();
    /// @brief Constructeur par argument
	/// @details
	/// Utilisation: Construit une pacgum par argument
    /// @param x: float position x
    /// @param y: float position y
    Point(float x, float y);

    /// @brief Destructeur par défaut
	/// @details
	/// Utilisation: détruit un point
    ~Point();

    /// @brief opération =
	/// @details
	/// Utilisation: Point A = [Point] B
    /// @return un Point = a celui en parametre
    Point operator=(const Point & P);

    /// @brief Renvoit la longueur
	/// @details
	/// Utilisation: Renvoit la longueur d'un point
    float norme();

};

/// @brief opération +
/// @details
/// Utilisation: Point A + Point B
/// @return Addition des deux points en param
Point operator+(const Point & P0, const Point & P1);

/// @brief opération -
/// @details
/// Utilisation: Point A - Point B
/// @return Soustraction des deux points en param
Point operator-(const Point & P0, const Point & P1);

/// @brief opération *
/// @details
/// Utilisation: Point A * Point B
/// @return multiplication des deux points en param
Point operator*(const Point & P0, const Point & P1);

/// @brief opération /
/// @details
/// Utilisation: Point A / facteur
/// @return Division d'un point par un facteur
Point operator/(const Point & P, float div);

/// @brief opération *
/// @details
/// Utilisation: Point A * facteur
/// @return Multiplication d'un point par un facteur
Point operator*(const Point & P, float mul);

/// @brief opération ==
/// @details
/// Utilisation: Point A == Point B
/// @return Si les deux points sont égaux
bool operator==(const Point & P0, const Point & P1);

/// @brief opération !=
/// @details
/// Utilisation: Point A != Point B
/// @return Si les deux points sont inégaux
bool operator!=(const Point & P0, const Point & P1);

#endif
