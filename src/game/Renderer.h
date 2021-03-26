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

#include <SDL2/SDL_image.h>
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
private:
  int width;
  SDL_Window *fenetre = nullptr;
  SDL_Renderer *drawer = nullptr;
  SDL_Texture * tMur = nullptr;
  SDL_Texture * tPacman = nullptr;
  SDL_Texture * tPacgum = nullptr;
  SDL_Texture * tSuperPacgum = nullptr;
  SDL_Texture * tLose = nullptr;
  SDL_Event input;
  

  void affEnd();
  void setWindowColor(unsigned char r, unsigned char g, unsigned char b);
  SDL_Texture* loadTexture( string path );
  
public:
  SDLRenderer();
  ~SDLRenderer();
  void render();
  void tileToTexture(char c, int & index, int & rotation, SDL_RendererFlip & flip);
  UserInput getInput();
};



#endif // RENDERER_H
