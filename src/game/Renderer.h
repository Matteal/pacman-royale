#ifndef RENDERER_H
#define RENDERER_H

#include "Terrain.h"
#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <ctime>
#include <string.h>


#include <curses.h>
#include "Pacman.h"

enum UserInput
{
  Z, Q, S, D, IDLE, QUIT,
};


class Renderer
{
protected:
  Terrain* m_terrain;
  std::vector<Pacman*>* m_tabPacman;

public:
  Renderer(){}
  virtual ~Renderer(){}

  void init(Terrain* terrain, std::vector<Pacman*>* tabPacman)
  {
    m_terrain = terrain;
    m_tabPacman = tabPacman;
  }
  virtual void render(){}
  virtual UserInput getInput(){return IDLE;}
};

class ConsoleRenderer: public Renderer
{
public:
  ConsoleRenderer();
  ~ConsoleRenderer();

  void render();
  UserInput getInput();

private:
  WINDOW* m_window;

};

class SDLRenderer: public Renderer
{
public:
  SDLRenderer(): Renderer(){};

  void render(){};
  UserInput getInput(){return IDLE;};
};



#endif // RENDERER_H
