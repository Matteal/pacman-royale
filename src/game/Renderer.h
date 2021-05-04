#ifndef RENDERER_H
#define RENDERER_H

#include "Terrain.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctime>
#include <string.h>

//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_mixer.h>

#include "Pacman.h"


// fast declaration of WINDOW
typedef struct _win_st WINDOW;


enum UserInput
{
	Z, Q, S, D, IDLE, QUIT, PAUSE
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
	virtual void render(int indexPacman, int FPS){}
	virtual UserInput getInput(){return IDLE;}

};

class ConsoleRenderer: public Renderer
{
public:
	ConsoleRenderer();
	~ConsoleRenderer();

	void render(int indexPacman, int FPS);
	UserInput getInput();

private:
	WINDOW* m_window;
	bool to_clear = true;

};
//
//class SDLRenderer: public Renderer
//{
//private:
//	int width;
//	SDL_Rect Camera;
//	SDL_Window *fenetre = nullptr;
//	SDL_Renderer *drawer = nullptr;
//
//	/** INITIALISATION DES TEXTURES **/
//	SDL_Texture * tMur = nullptr;
//	SDL_Texture * tPacman = nullptr;
//	SDL_Texture * tPacgum = nullptr;
//	SDL_Texture * tSuperPacgum = nullptr;
//	SDL_Texture * tLose = nullptr;
//	SDL_Texture * tWin = nullptr;
//	SDL_Texture * tPress = nullptr;
//	SDL_Texture * tStart = nullptr;
//
//	/** INITIALISATION DES SONS **/
//	Mix_Chunk * sStart = nullptr;
//	Mix_Chunk * sWaka = nullptr;
//	Mix_Chunk * sFruit = nullptr;
//	Mix_Chunk * sGhost = nullptr;
//	Mix_Chunk * sDeath = nullptr;
//
//
//
//	SDL_Event input;
//
//
//	int alphaCounter = 0;
//	int previousState = 0;
//
//
//	void affEnd();
//	void setWindowColor(unsigned char r, unsigned char g, unsigned char b);
//	SDL_Texture* loadTexture( string path );
//
//public:
//	SDLRenderer();
//	~SDLRenderer();
//	void render(int indexPacman, int FPS);
//	void tileToTexture(char c, int & index, int & rotation, SDL_RendererFlip & flip);
//	UserInput getInput();
//};



#endif // RENDERER_H
