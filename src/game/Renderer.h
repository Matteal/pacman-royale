#ifndef RENDERER_H
#define RENDERER_H

#include "Terrain.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctime>
#include <string.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "Pacman.h"


// fast declaration of WINDOW
typedef struct _win_st WINDOW;

/// @brief Enum des touches possibles
enum UserInput
{
	Z, Q, S, D, IDLE, QUIT
};


class Renderer
{
protected:
	Terrain* m_terrain;
	std::vector<Pacman*>* m_tabPacman;

public:
	/// @brief Constructeur par défaut
	/// @details
	/// Utilisation: Construit un Renderer par défaut
	Renderer(){}

	/// @brief Destructeur par défaut
	/// @details
	/// Utilisation: Detruit le renderer
	virtual ~Renderer(){}

	/// @brief Initialise les variables
	/// @details
	/// Utilisation: Prend le terrain et les pacmans
	/// @param terrain: lien vers Terrain
	/// @param tabPacman: lien vers Vecteur de Pacman
	void init(Terrain* terrain, std::vector<Pacman*>* tabPacman)
	{
		m_terrain = terrain;
		m_tabPacman = tabPacman;
	}

	/// @brief Rend un affichage graphique
	/// @details
	/// Utilisation: Affiche le jeu
	/// @param terrain: entier représentant le pacman principal
	/// @param FPS: nombre de FPS
	virtual void render(int indexPacman, int FPS){}

	/// @brief Recupere les informations des touches
	/// @details
	/// Utilisation: Recupere les appuies touches
	/// @return: Enum Input
	virtual UserInput getInput(){return IDLE;}

};

class ConsoleRenderer: public Renderer
{
public:
	/// @brief Constructeur par défaut
	/// @details
	/// Utilisation: Construit un Renderer Console par défaut
	ConsoleRenderer();

	/// @brief Destructeur par défaut
	/// @details
	/// Utilisation: Detruit le renderer console
	~ConsoleRenderer();

	/// @brief Rend un affichage graphique
	/// @details
	/// Utilisation: Affiche le jeu avec la CONSOLE
	/// @param terrain: entier représentant le pacman principal
	/// @param FPS: nombre de FPS
	void render(int indexPacman, int FPS);

	/// @brief Recupere les informations des touches
	/// @details
	/// Utilisation: Recupere les appuies touches
	/// @return: Enum Input
	UserInput getInput();

private:
	WINDOW* m_window;
	bool to_clear = true;

};

class SDLRenderer: public Renderer
{
private:
	int width;
	SDL_Rect Camera;
	SDL_Window *fenetre = nullptr;
	SDL_Renderer *drawer = nullptr;

	/** INITIALISATION DES TEXTURES **/
	SDL_Texture * tMur = nullptr;
	SDL_Texture * tPacman = nullptr;
	SDL_Texture * tPacgum = nullptr;
	SDL_Texture * tSuperPacgum = nullptr;
	SDL_Texture * tLose = nullptr;
	SDL_Texture * tWin = nullptr;
	SDL_Texture * tPress = nullptr;
	SDL_Texture * tStart = nullptr;

	/** INITIALISATION DES SONS **/
	Mix_Chunk * sStart = nullptr;
	Mix_Chunk * sWaka = nullptr;
	Mix_Chunk * sFruit = nullptr;
	Mix_Chunk * sGhost = nullptr;
	Mix_Chunk * sDeath = nullptr;



	SDL_Event input;


	int alphaCounter = 0;
	int previousState = 0;


	void affEnd();
	void setWindowColor(unsigned char r, unsigned char g, unsigned char b);
	SDL_Texture* loadTexture( string path );

public:
	/// @brief Constructeur par défaut
	/// @details
	/// Utilisation: Construit un Renderer SDL par défaut
	SDLRenderer();

	/// @brief Destructeur par défaut
	/// @details
	/// Utilisation: Detruit le renderer SDL
	~SDLRenderer();

	/// @brief Rend un affichage graphique
	/// @details
	/// Utilisation: Affiche le jeu avec la SDL
	/// @param terrain: entier représentant le pacman principal
	/// @param FPS: nombre de FPS
	void render(int indexPacman, int FPS);

	/// @brief Renvoit texture en fonction du tableau
	/// @details
	/// Utilisation: Prend un char du terrain, son index, sa rotation, et son retournement et actualise ses parametre
	/// @param c: char du terrain
	/// @param index: référence vers Index de la texture dans la tileset
	/// @param rotation: référence vers rotation de la texture dans la tilset
	/// @param rotation: référence vers le retournement de la texture a appliquer
	void tileToTexture(char c, int & index, int & rotation, SDL_RendererFlip & flip);

	/// @brief Recupere les informations des touches
	/// @details
	/// Utilisation: Recupere les appuies touches
	/// @return: Enum Input
	UserInput getInput();
};



#endif // RENDERER_H
