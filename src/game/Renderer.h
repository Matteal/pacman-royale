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
class Renderer
{
protected:
  Terrain* m_terrain;
  std::vector<Pacman*>* m_tabPacman;

public:
  Renderer(){}
  ~Renderer(){}

  void init(Terrain* terrain, std::vector<Pacman*>* tabPacman)
  {
    m_terrain = terrain;
    m_tabPacman = tabPacman;
  }
  virtual void render(){}
  virtual char getInput(){}
};

class ConsoleRenderer: public Renderer
{
public:
  ConsoleRenderer();
  ~ConsoleRenderer();

  void render();
  char getInput();

private:
  WINDOW* m_window;

};

class SDLRenderer: Renderer
{
public:
  SDLRenderer(){};

  void render(){};
  char getInput(){};
};



#endif // RENDERER_H
