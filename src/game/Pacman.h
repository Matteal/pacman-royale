#ifndef PACMAN_H
#define PACMAN_H

#include "direction.h"
#include "iostream"
#include "Point.h"

class Pacman
{
public:
	Pacman(Point p, direction dir, direction dirNext, int timer, bool isSuper, int state, bool isPlayer, bool isGhost, int r, int g, int b);
	Pacman();
	direction _dirNext;
	int _timer;
	bool _isSuper;
	int _state; // 42 = pause; 0 = partie en cours; 43 = début
	int compteurAnimation[2];
	int _playSound = 0;

	float getX() const;
	int getIndexX() const;
	void setX(float x);

	float getY() const;
	int getIndexY() const;
	void setY(float y);

	void setPos(Point);
	Point getPos() const;
	Point getIndexPos() const;

	char getRepr();

	direction getDir() const;
	void setDir(direction dir);

	void setPlayer(bool t);
	void setGhost(bool t);

	bool getPlayer() const;
	bool getGhost() const;

	void actuState(int FPS);

	void setColor(int r);
	int getColor() const;

protected:
	Point _p;
	direction _dir;
	int _r, _g, _b;
	bool _isPlayer, _isGhost;

	char _repr;
};

#endif
