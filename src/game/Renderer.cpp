#include "Renderer.h"

#include <locale.h>

//INFO SCREEN
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

#define WALL 1
#define PACGUM 2
#define SUPER_PACGUM 3
#define PACMAN 4
#define GHOST_RED 5
#define GHOST_CYAN 6



ConsoleRenderer::ConsoleRenderer(): Renderer()
{
	m_window = subwin(stdscr, 10, 10, LINES, COLS); // Créer une fenetre terminal
	initscr(); // initialise la fenetre
	noecho();  // Option fenetre
	scrollok(stdscr, TRUE);
	system("setterm -cursor off");
	nodelay(stdscr, TRUE); // Transforme getch en fonction non bloquante

	refresh(); // Rafraichissement page avant le start
	if (has_colors() == FALSE) 
	{
		endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
	setlocale(LC_ALL, "");

	start_color();
	init_pair(WALL, COLOR_BLUE, COLOR_BLACK);
	init_pair(PACGUM, COLOR_WHITE, COLOR_BLACK);
	init_pair(SUPER_PACGUM, COLOR_GREEN, COLOR_BLACK);
	init_pair(PACMAN, COLOR_YELLOW, COLOR_BLACK);
	init_pair(GHOST_RED, COLOR_RED, COLOR_BLACK);
	init_pair(GHOST_CYAN, COLOR_CYAN, COLOR_BLACK);
}

ConsoleRenderer::~ConsoleRenderer()
{



	system("setterm -cursor on");
	free(m_window); // libération fenetre
	endwin(); // destruction fenetre
	
}

UserInput ConsoleRenderer::getInput()
{
	char input = getch();
	flushinp(); // reset du buffer de getch pour éviter les input lags

	UserInput userInput;
	switch (input) // on vérifie la touche appuyée
	{
		case 'z':
		userInput = Z;
		break;
		case 'q':
		userInput = Q;
		break;
		case 's':
		userInput = S;
		break;
		case 'd':
		userInput = D;
		break;
		case 'p':
		userInput = PAUSE;
		break;
		case ' ':
		userInput = PAUSE;
		break;
		case 27: // si fonction (toutes touches non charactere = 27 avec curses :/)
		userInput = QUIT;
		break;
		default:
		userInput = IDLE;
		break;
	}
	flushinp(); // empèche la queue d'input de se former
	return userInput;
}

void ConsoleRenderer::render(int indexPacman, int FPS)
{

	if(indexPacman == -1 || (m_tabPacman->at(indexPacman)->_state == 42 || m_tabPacman->at(indexPacman)->_state == 0))
	{
		if(to_clear)
		{
			clear();
			to_clear = false;
			for(int j = 0; j < m_terrain->getHeight(); j++)
			{
				for(int i = 0; i < m_terrain->getWidth()*2; i++)
				{
					if(i%2 == 0) // si i est pair, on affiche un char du terrain
					{
						int COLOR = COLOR_PAIR(WALL);
						char c = m_terrain->getTile(i/2, j);
						if(c != '.' && c != ' ' && c != 'S') 
						{
							attron(COLOR);
							mvaddch(50-j, i+COLS/4 + 5, '#');
							attroff(COLOR);
						}
					}
				}
			}
			
		}
			
		for(int j = 0; j < m_terrain->getHeight(); j++) // On parcour les colones
		{
			for(int i = 0; i < m_terrain->getWidth() * 2; i+=2) // on parcour les lignes
			{
				if(i%2 == 0) // si i est pair, on affiche un char du terrain
				{
					int COLOR = COLOR_PAIR(WALL);
					char c = m_terrain->getTile(i/2, j);
					if(c != '.' && c != ' ' && c != 'S') c = '#';
					if(c == '.') 
					{
						COLOR = COLOR_PAIR(PACGUM);
					}
					else if(c == 'S') 
					{
						COLOR = COLOR_PAIR(SUPER_PACGUM);
					}
					else if(c==' ')
					{
						COLOR = COLOR_PAIR(WALL);
					}
					else COLOR_PAIR(WALL);

					if(!(c != '.' && c != ' ' && c != 'S'))
					{
						attron(COLOR);
						mvaddch(50-j, i+COLS/4 + 5, c);
						attroff(COLOR);
					}
				}
			}
		}
		for(int i=0; i<(int)m_tabPacman->size(); i++)
		{
			if(m_tabPacman->at(i)->_state != -1 || m_tabPacman->at(i)->getGhost())
			{
				char c;
				int COLOR = COLOR_PAIR(PACMAN);
				if(m_tabPacman->at(i)->getGhost())
				{
					if(m_tabPacman->at(i)->_state == -1)
					{
						c = '<';
					}
					else
					{
						c = 'n';
					}
					if(m_tabPacman->at(i)->getColor()< 3)
							COLOR = COLOR_PAIR(GHOST_CYAN);
						else 
							COLOR = COLOR_PAIR(GHOST_RED);
						
				}
				else if(m_tabPacman->at(i)->_isSuper)
				{
					if(m_tabPacman->at(i)->_timer > ((FPS*10)/4)*3 && m_tabPacman->at(i)->_timer%10 >= 5)
						c = ' ';
					else
						c = '0';
				}
				else
					c = 'o';

				int x = m_tabPacman->at(i)->getIndexX()*2;
				int y = m_tabPacman->at(i)->getIndexY();
				if(x < 0)
				{
					x = 0;
				}		
				else if(x > m_terrain->getWidth() * 2 - 2)
				{
					x = m_terrain->getWidth() - 2;
				}
				if(y < 0)
				{
					y = 0;
				}		
				else if(y > m_terrain->getHeight() -1 )
				{
					y = m_terrain->getHeight() - 1;
				}

				attron(COLOR);
				mvaddch(50-y, x+COLS/4 + 5, c);
				attroff(COLOR);
			}
		}
	}
	else if(m_tabPacman->at(indexPacman)->_state == -1)
	{
		mvprintw((LINES / 2), (COLS / 2) - 6, "YOU ARE DEAD");
		if(m_tabPacman->at(indexPacman)->_timer < FPS*5) m_tabPacman->at(indexPacman)->_timer+=4;
		else m_tabPacman->at(indexPacman)->_timer = FPS*20;
		to_clear = true;
	}
	else if(m_tabPacman->at(indexPacman)->_state == 1)
	{
		mvprintw((LINES / 2), (COLS / 2) - 4, "YOU WIN!");
		if(m_tabPacman->at(indexPacman)->_timer < FPS*5) m_tabPacman->at(indexPacman)->_timer+=4;
		else m_tabPacman->at(indexPacman)->_timer = FPS*20;
		to_clear = true;
		
	}
	else if(m_tabPacman->at(indexPacman)->_state == 43)
	{
		mvprintw(LINES/2, COLS/2 - 12, "PRESS SPACE OR P TO PLAY");
		to_clear = true;
	}

	refresh();

}

/*** SDL  ***/

SDLRenderer::SDLRenderer(): Renderer()
{

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		cerr << "Erreur a l'ini SDL =" << SDL_GetError() << endl;
		affEnd();
		quick_exit(EXIT_FAILURE);
	}

	if (SDL_Init(SDL_INIT_AUDIO) != 0)
	{
		cerr << "Erreur a l'ini SDL AUDIO =" << SDL_GetError() << endl;
		affEnd();
		quick_exit(EXIT_FAILURE);
	}

	if(Mix_OpenAudio(11025, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0)
	{
		cerr << "Erreur a l'ini MIXER =" << SDL_GetError() << endl;
		affEnd();
		quick_exit(EXIT_FAILURE);
	}


	fenetre = SDL_CreateWindow("Pacman Royal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (fenetre == nullptr)
	{
		cerr << "Erreur a la creation de fenetre SDL =" << Mix_GetError() << endl;
		affEnd();
		quick_exit(EXIT_FAILURE);
	}

	drawer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(drawer, SDL_BLENDMODE_BLEND);


	if (drawer == nullptr)
	{
		cerr << "Erreur a la creation du Renderer =" << SDL_GetError() << endl;
		affEnd();
		quick_exit(EXIT_FAILURE);
	}

	setWindowColor(0, 0, 0);

	if (0 != SDL_RenderClear(drawer))
	{
		cerr << "Erreur RenderClear : " << SDL_GetError() << endl;
		affEnd();
		quick_exit(EXIT_FAILURE);
	}

	int imgFlags = IMG_INIT_PNG;
	if(!(IMG_Init(imgFlags) & imgFlags))
	{
		cerr << "Erreur initialisation SDL_Image =" << SDL_GetError() << endl;
		affEnd();
		quick_exit(EXIT_FAILURE);
	}
	Mix_AllocateChannels(32);
	
	sStart = Mix_LoadWAV("./data/sound/start.wav");
	sWaka = Mix_LoadWAV("./data/sound/wakawaka.wav");
	sFruit = Mix_LoadWAV("./data/sound/eatfruit.wav");
	sGhost = Mix_LoadWAV("./data/sound/eatghost.wav");
	sDeath = Mix_LoadWAV("./data/sound/death.wav");

	if(Mix_PlayChannel(0, sStart, 0) != 0)
	{
		cout<<"Erreur son : "<<Mix_GetError()<<endl;
	}

	tMur = loadTexture("./data/sprite/tileset.png");
	tPacman = loadTexture("./data/sprite/pacmantileset.png");
	tPacgum = loadTexture("./data/sprite/pacgum.png");
	tSuperPacgum = loadTexture("./data/sprite/superPacgum.png");
	tLose = loadTexture("./data/sprite/death.png");
	tWin = loadTexture("./data/sprite/win.png");
	tPress = loadTexture("./data/sprite/press.png");
	tStart = loadTexture("./data/sprite/start.png");
	

	SDL_SetTextureAlphaMod(tLose, 0);
	SDL_SetTextureAlphaMod(tWin, 0);
	SDL_SetTextureAlphaMod(tPress, 0);
	SDL_SetTextureAlphaMod(tStart, 0);

	Camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

}

void SDLRenderer::setWindowColor(unsigned char r, unsigned char g, unsigned char b)
{
	if (0 != SDL_SetRenderDrawColor(drawer, r, g, b, 255))
	{
		cerr << "Erreur SDL_SetRenderDrawColor : " << SDL_GetError() << endl;
		affEnd();
	}
}

void SDLRenderer::render(int indexPacman, int FPS)
{
	int nbTiles = 20;
	float ratio = SCREEN_HEIGHT / nbTiles;
	SDL_Rect PacWalk[4] =
	{
		{0, 0, 15, 15},
		{15, 0, 15, 15},
		{120, 15, 15, 15},
		{135, 15, 15, 15}
	};
	SDL_Rect tWhere[6]=
	{
		{0,  0, 15, 15},
		{15, 0, 15, 15},
		{30, 0, 15, 15},
		{45, 0, 15, 15},
		{60, 0, 15, 15},
		{75, 0, 15, 15}
	};
	SDL_Rect GhostWalk[6][3] =
	{
		{{0, 15, 15, 15}, //ROUGE
		{15, 15, 15, 15},
		{30, 15, 15, 15}},

		{{0, 30, 15, 15}, //ROSE
		{15, 30, 15, 15},
		{30, 30, 15, 15}},

		{{45, 30, 15, 15}, //BLEU
		{60, 30, 15, 15},
		{75, 30, 15, 15}},

		{{90, 30, 15, 15}, //Vert
		{105, 30, 15, 15},
		{120, 30, 15, 15}},

		{{45, 15, 15, 15}, //MORT
		{60, 15, 15, 15},
		{75, 15, 15, 15}},

		{{0,0,0},{0,0,0},{0,0,0}}//RIEN

	};
	SDL_Rect GhostEat[2] = 
	{
		{90, 15, 15, 15},
		{105, 15, 15, 15}
	};

	int facteur = ratio;
	SDL_RenderClear(drawer);
	if(m_tabPacman->at(indexPacman)->_state == 0 || m_tabPacman->at(indexPacman)->_state == 42)
	{
		if(alphaCounter == 0) previousState = 0;
		Camera.x = (((m_tabPacman->at(indexPacman)->getX() * ratio) + ratio/2) - SCREEN_WIDTH/2);
		Camera.y = (((m_tabPacman->at(indexPacman)->getY() * ratio) - ratio/2) - SCREEN_HEIGHT/2);

		SDL_RendererFlip flip = SDL_FLIP_NONE;
		int index, rotation;
		index = 0;
		rotation = 0;
		for(int i = -10; i < m_terrain->getWidth() + 10; i++)
		for(int j = -10; j < m_terrain->getHeight() + 10; j++)
		{
			int x, y;
			x = i;
			y = j;
			if(x < 0) x = m_terrain->getWidth() + x;
			if(x > m_terrain->getWidth() - 1) x = x - m_terrain->getWidth();
			if(y < 0) y = m_terrain->getHeight() + y;
			if(y > m_terrain->getHeight() - 1) y = y - m_terrain->getHeight();
			if(m_terrain->getTile(x, y) != ' ' && m_terrain->getTile(x, y) != 'S' && m_terrain->getTile(x, y) != '.')
			{
				Point position = {i*ratio, j*ratio};
				SDL_Rect where = {(int)(position.x - Camera.x), (int)(SCREEN_HEIGHT - (position.y - Camera.y)), (int)ratio, (int)ratio};
				tileToTexture(m_terrain->getTile(x, y), index, rotation, flip);

				SDL_RenderCopyEx(drawer, tMur, &tWhere[index], &where, rotation, NULL, flip);
			}
			else if(m_terrain->getTile(x, y) == '.')
			{
				Point position = {i*ratio + ratio/4, j*ratio - ratio/4};
				SDL_Rect where = {(int)(position.x - Camera.x), (int)(SCREEN_HEIGHT - (position.y - Camera.y)), (int)(ratio/2), (int)(ratio/2)};
				SDL_RenderCopy(drawer, tPacgum, NULL, &where);
			}
			else if(m_terrain->getTile(x, y) == 'S')
			{
				Point position = {i*ratio, j*ratio};
				SDL_Rect where = {(int)(position.x - Camera.x), (int)(SCREEN_HEIGHT - (position.y - Camera.y)), (int)ratio, (int)ratio};
				SDL_RenderCopy(drawer, tSuperPacgum, NULL, &where);
			}
		}
		SDL_Rect Tex = {0, 0, 0, 0};
		for(int i = 0; i < (int)m_tabPacman->size(); i++)
		{
			if(m_tabPacman->at(i)->_state != -1 || m_tabPacman->at(i)->getGhost())
			{
				if(m_tabPacman->at(i)->_playSound != 0 && i == indexPacman)
				{
					if(!Mix_Playing(1) && m_tabPacman->at(i)->_playSound==1)
						Mix_PlayChannel(1, sWaka, 0);
					if(m_tabPacman->at(i)->_playSound==2)
						Mix_PlayChannel(1, sFruit, 0);
					if(m_tabPacman->at(i)->_playSound==3)
						Mix_PlayChannel(1, sGhost, 0);
					
				}
				if(m_tabPacman->at(i)->_playSound)
				{
					m_tabPacman->at(i)->_playSound = 0;
				}
				rotation = 0;
				flip = SDL_FLIP_NONE;
				Point position = {m_tabPacman->at(i)->getX() * ratio, m_tabPacman->at(i)->getY() * ratio};
				SDL_Rect where = {(int)(position.x - Camera.x), (int)(SCREEN_HEIGHT - (position.y - Camera.y)), (int)ratio, (int)ratio};
				if(m_tabPacman->at(i)->getGhost())
				{
					if(!m_tabPacman->at(indexPacman)->_isSuper || m_tabPacman->at(i)->_state == -1)
					{
						int color = m_tabPacman->at(i)->getColor();
						if(m_tabPacman->at(i)->_state == -1)
						{
							if(m_tabPacman->at(i)->_timer%10 <= 5 && m_tabPacman->at(i)->_timer > FPS)
								color = 5;
							else
								color = 4;
						}
							
						switch (m_tabPacman->at(i)->getDir())
						{
							case UP:
							Tex = GhostWalk[color][1];
							break;

							case DOWN:
							Tex = GhostWalk[color][2];
							break;

							case LEFT:
							flip = SDL_FLIP_HORIZONTAL;
							Tex = GhostWalk[color][0];
							break;

							case RIGHT:
							Tex = GhostWalk[color][0];
							break;

							default:
							break;
						}
					}
					else
						Tex = GhostEat[m_tabPacman->at(i)->getColor()%2];
					
					SDL_RenderCopyEx(drawer, tPacman, &Tex, &where, rotation, NULL, flip);
				}
				else
				{
					SDL_Rect pacWhere;
					Point position = {m_tabPacman->at(i)->getX() * ratio, m_tabPacman->at(i)->getY() * ratio};
					pacWhere = {(int)(position.x - Camera.x), (int)(SCREEN_HEIGHT - (position.y - Camera.y)), (int)ratio, (int)ratio};
					int texI = 0;
					if(m_tabPacman->at(i)->compteurAnimation[0] < 11)
					{
						if(m_tabPacman->at(i)->compteurAnimation[0] == 10) m_tabPacman->at(i)->compteurAnimation[0] = 0;
						if(m_tabPacman->at(i)->compteurAnimation[0] < 5) texI = 0;
						else texI = 1;
						m_tabPacman->at(i)->compteurAnimation[0]++;
					}
					if(m_tabPacman->at(i)->_isSuper) texI +=2;
					Tex = PacWalk[texI];
					int frame = m_tabPacman->at(i)->_timer%10;
					if(m_tabPacman->at(i)->_timer > ((FPS*10)/4)*3 && frame >= 5) pacWhere = {0, 0, 0, 0};
					rotation = 0;
					if(m_tabPacman->at(i)->getDir() == UP) rotation = -90;
					if(m_tabPacman->at(i)->getDir() == DOWN) rotation = 90;
					if(m_tabPacman->at(i)->getDir() == LEFT) flip = SDL_FLIP_HORIZONTAL;
					SDL_RenderCopyEx(drawer, tPacman, &Tex, &pacWhere, rotation, NULL, flip);
				}

				int distBordure = 12;
				if(m_terrain->isInBordure(m_tabPacman->at(i)->getPos(), distBordure) && m_terrain->isInBordure(m_tabPacman->at(indexPacman)->getPos(), distBordure)&& i != indexPacman)
				{
					float x, y;
					Point PP = m_tabPacman->at(indexPacman)->getPos();
					x = m_tabPacman->at(i)->getX();
					y = m_tabPacman->at(i)->getY();
					if(x < distBordure && PP.x > m_terrain->getWidth() - distBordure)
						x += m_terrain->getWidth();
					else if(x > m_terrain->getWidth() - distBordure && PP.x < distBordure) 
						x -= m_terrain->getWidth();
					if(y < distBordure && PP.y > m_terrain->getHeight() - distBordure)
					{
						y += m_terrain->getHeight();
					}
					else if(y > m_terrain->getHeight() - distBordure && PP.y < distBordure)
						y -= m_terrain->getHeight();
					Point position = {x* ratio, y* ratio};
					where = {(int)(position.x - Camera.x), (int)(SCREEN_HEIGHT - (position.y - Camera.y)), (int)ratio, (int)ratio};
					SDL_RenderCopyEx(drawer, tPacman, &Tex, &where, rotation, NULL, flip);
				}
			}
			

		}
		if(m_tabPacman->at(indexPacman)->_state == 42  || (previousState == 42 && alphaCounter > 0))
		{
			previousState = 42;
			SDL_Rect w = {SCREEN_WIDTH/4, SCREEN_HEIGHT/4, SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
			SDL_SetTextureAlphaMod(tPress, alphaCounter);
			SDL_RenderCopy(drawer, tPress, NULL, &w);
		}
	}
	if(m_tabPacman->at(indexPacman)->_state == -1 || (previousState == -1 && alphaCounter > 0))
	{
		previousState = -1;
		if(m_tabPacman->at(indexPacman)->_state == -1)
		{
			if(m_tabPacman->at(indexPacman)->_playSound == 4)
			{
				Mix_PlayChannel(1, sWaka, 0);
				m_tabPacman->at(indexPacman)->_playSound = 0;
			}
			
			SDL_Rect death = {30 + 15 * m_tabPacman->at(indexPacman)->compteurAnimation[1], 0, 15, 15};
			SDL_Rect where = {(int)(SCREEN_WIDTH/2 - (int)(facteur)), (int)(SCREEN_HEIGHT - 5*facteur), (int)facteur*2, (int)facteur*2};
			if(alphaCounter == 255)
			{
				if(m_tabPacman->at(indexPacman)->_timer < 1001) 
				{
					m_tabPacman->at(indexPacman)->_timer+=1;
					m_tabPacman->at(indexPacman)->compteurAnimation[1] = m_tabPacman->at(indexPacman)->_timer/100;
				}
				else m_tabPacman->at(indexPacman)->_timer = FPS*20;
			}
			else
			{
				m_tabPacman->at(indexPacman)->_timer=0;
			}
			//m_tabPacman->at(indexPacman)->compteurAnimation[1] = m_tabPacman->at(indexPacman)->_timer/100;
			//cout<<m_tabPacman->at(indexPacman)->compteurAnimation[1]<<endl;

			SDL_SetTextureAlphaMod(tPacman, alphaCounter);
			SDL_RenderCopy(drawer, tPacman, &death, &where);
		}

		SDL_SetTextureAlphaMod(tLose, alphaCounter);
		SDL_RenderCopy(drawer, tLose, NULL, NULL);
	}
	else if(m_tabPacman->at(indexPacman)->_state == 1 || (previousState == 1 && alphaCounter > 0))
	{
		previousState = 1;
		if(alphaCounter == 255)
		{
			if(m_tabPacman->at(indexPacman)->_timer < FPS*5) m_tabPacman->at(indexPacman)->_timer+=4;
			else m_tabPacman->at(indexPacman)->_timer = FPS*20;
		}
		else
		{
			m_tabPacman->at(indexPacman)->_timer=0;
		}
		SDL_SetTextureAlphaMod(tWin, alphaCounter);
		SDL_RenderCopy(drawer, tWin, NULL, NULL);
	}
	else if(m_tabPacman->at(indexPacman)->_state == 43  || (previousState == 43 && alphaCounter > 0))
	{
		previousState = 43;
		SDL_SetTextureAlphaMod(tStart, alphaCounter);
		SDL_RenderCopy(drawer, tStart, NULL, NULL);
	}
	if(m_tabPacman->at(indexPacman)->_state != 0) alphaCounter+=2;
	else if(alphaCounter > 0) alphaCounter-=10;
	if(alphaCounter < 0) alphaCounter = 0;
	if(alphaCounter >= 256) alphaCounter = 255;
	SDL_RenderPresent(drawer);
	//napms(50);
}

UserInput SDLRenderer::getInput()
{
	while(SDL_PollEvent(&input))
	{
		if(input.type == SDL_QUIT) return QUIT;
		else if(input.type == SDL_KEYUP)
		{
			if(input.key.keysym.sym == SDLK_ESCAPE) return QUIT;
			else if(input.key.keysym.sym == SDLK_z || input.key.keysym.sym == SDLK_UP) return Z;
			else if(input.key.keysym.sym == SDLK_s || input.key.keysym.sym == SDLK_DOWN) return S;
			else if(input.key.keysym.sym == SDLK_q || input.key.keysym.sym == SDLK_LEFT) return Q;
			else if(input.key.keysym.sym == SDLK_d || input.key.keysym.sym == SDLK_RIGHT) return D;
			else if(input.key.keysym.sym == SDLK_p || input.key.keysym.sym == SDLK_SPACE) return PAUSE;
		}
	}

	return IDLE;
}



void SDLRenderer::affEnd()
{
	// ~Textures
	SDL_DestroyTexture(tMur);
	SDL_DestroyTexture(tPacman);
	SDL_DestroyTexture(tPacgum);
	SDL_DestroyTexture(tSuperPacgum);
	SDL_DestroyTexture(tLose);
	SDL_DestroyTexture(tWin);
	SDL_DestroyTexture(tPress);
	SDL_DestroyTexture(tStart);

	//~Composant
	Mix_FreeChunk(sStart);
	Mix_FreeChunk(sWaka);
	Mix_FreeChunk(sFruit);
	Mix_FreeChunk(sGhost);
	Mix_FreeChunk(sDeath);
	Mix_CloseAudio();

	//Sons
	SDL_DestroyRenderer(drawer);
	SDL_DestroyWindow(fenetre);
	SDL_Quit();
}

void SDLRenderer::tileToTexture(char c, int& index, int& rotation, SDL_RendererFlip& flip)
{
	//SE REFERER TERRAIN POUR LES CHAR
	switch (c)
	{
		case '#':
		index = 0;
		rotation = 0;
		flip = SDL_FLIP_NONE;
		break;

		case 'T':
		index = 4;
		rotation = 0;
		flip = SDL_FLIP_NONE;
		break;

		case 'D':
		index = 4;
		rotation = 0;
		flip = SDL_FLIP_HORIZONTAL;
		break;

		case 'L':
		index = 4;
		rotation = 90;
		flip = SDL_FLIP_NONE;
		break;

		case 'R':
		index = 4;
		rotation = 90;
		flip = SDL_FLIP_NONE;
		break;

		case '=':
		index = 2;
		rotation = 0;
		flip = SDL_FLIP_NONE;
		break;

		case '|':
		index = 2;
		rotation = 90;
		flip = SDL_FLIP_NONE;
		break;

		case 'G':
		index = 3;
		rotation = 0;
		flip = SDL_FLIP_NONE;
		break;

		case '(':
		index = 3;
		rotation = 0;
		flip = SDL_FLIP_HORIZONTAL;
		break;

		case ']':
		index = 3;
		rotation = 0;
		flip = SDL_FLIP_VERTICAL;
		break;

		case '[':
		index = 3;
		rotation = 0;
		flip = SDL_RendererFlip(SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL);
		break;

		case 'v':
		index = 5;
		rotation = 90;
		flip = SDL_FLIP_NONE;
		break;

		case '^':
		index = 5;
		rotation = -90;
		flip = SDL_FLIP_NONE;
		break;

		case '>':
		index = 5;
		rotation = 0;
		flip = SDL_FLIP_NONE;
		break;

		case '<':
		index = 5;
		rotation = 0;
		flip = SDL_FLIP_HORIZONTAL;
		break;

		default:
		index = 1;
		rotation = 0;
		flip = SDL_FLIP_NONE;
		break;
	}
}

SDLRenderer::~SDLRenderer()
{
	affEnd();
}

SDL_Texture* SDLRenderer::loadTexture(string path)
{
	//Texture a renvoyé
	SDL_Texture* R = NULL;

	//On initialise une surface
	SDL_Surface* iniImage = IMG_Load(path.c_str());
	if(iniImage == nullptr) // Si Erreur
	{
		cerr<<"Impossible IMG LOAD : Chemin = "<<path.c_str()<< " ERROR = "<<IMG_GetError()<<endl;
	}
	else
	{
		//On transforme la surface en Texture
		R = SDL_CreateTextureFromSurface(drawer, iniImage);
		if(R == nullptr)
		{
			cerr<<"Impossible TEXTURE CREATE : Chemin = "<<path.c_str()<<" ERROR = "<<SDL_GetError()<<endl;
		}

		//Suppression de la surface temporaire
		SDL_FreeSurface(iniImage);
	}

	return R;
}
