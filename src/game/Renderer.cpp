#include "Renderer.h"

ConsoleRenderer::ConsoleRenderer(): Renderer()
{
  m_window = subwin(stdscr, 10, 10, LINES, COLS); // Créer une fenetre terminal
  initscr(); // initialise la fenetre
  noecho();  // Option fenetre
  scrollok(stdscr, TRUE);
  system("setterm -cursor off");
  nodelay(stdscr, TRUE); // Transforme getch en fonction non bloquante

  refresh(); // Rafraichissement page avant le start
}

ConsoleRenderer::~ConsoleRenderer()
{
  system("setterm -cursor on");
  endwin(); // destruction fenetre
  free(m_window); // libération fenetre
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
    case 27: // si fonction (toutes touches non charactere = 27 avec curses :/)
      userInput = QUIT;
      break;
    default:
      userInput = IDLE;
      break;
  }

  return userInput;
}

void ConsoleRenderer::render()
{
  clear(); // Nettoie la fenetre

  // dessinne le terrain ligne par ligne
  char line[m_terrain->getWidth()*2+1]; // definition d'une ligne, *2 pour espacer le terrain
  for(int j = 0; j < m_terrain->getHeight(); j++) // On parcour les colones
  {
      for(int i = 0; i < m_terrain->getWidth() * 2; i++) // on parcour les lignes
      {
          if(i%2 == 0) // si i est pair, on affiche un char du terrain
          {
            line[i] = m_terrain->getTile(i/2, j);
          }
          else // si pas pair, on affiche un espace
          {
              line[i] = ' ';
          }
      }

      // pour toutes les lignes, on regarde s'il n'y a pas un pacman
      for(int indice=0; indice<(int)m_tabPacman->size(); indice++)
      {
        if(j == m_tabPacman->at(indice)->getIndexY()) // si Pacman, on affiche le char correspondant à son état
        {
          if(m_tabPacman->at(indice)->_isSuper)
            line[m_tabPacman->at(indice)->getIndexX()*2] = '0';
          else 
            line[m_tabPacman->at(indice)->getIndexX()*2] = 'o';
        }
      }
      line[m_terrain->getWidth()*2] = '\0'; // on termine la ligne
      mvprintw((LINES / 2) - j + (m_terrain->getWidth() /2), (COLS / 2) - (m_terrain->getWidth()*2 / 2), line); // on affiche la ligne
  }


  if(wrefresh(m_window) == ERR && errno !=0) // Si erreur ...
  {
      cerr<<"ERROR :: REFRESHING WINDOW :"<<strerror(errno)<<strerror(errno)<<endl;
  }
  napms(50); // Attend 50 ms
}

/*** SDL  ***/

SDLRenderer::SDLRenderer(): Renderer()
{
  width = 1000;
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		cerr << "Erreur a l'ini SDL =" << SDL_GetError() << endl;
		affEnd();
    quick_exit(EXIT_FAILURE);
	}

	fenetre = SDL_CreateWindow("Pacman Royal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, width, SDL_WINDOW_SHOWN);

	if (fenetre == nullptr)
	{
		cerr << "Erreur a la creation de fenetre SDL =" << SDL_GetError() << endl;
		affEnd();
    quick_exit(EXIT_FAILURE);
	}

	drawer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);

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

  tMur = loadTexture("./data/mur.png");
  tPacman = loadTexture("./data/pacman.png");
  tPacgum = loadTexture("./data/superPacgum.png");
  tSuperPacgum = loadTexture("./data/superPacgum.png");


}

void SDLRenderer::setWindowColor(unsigned char r, unsigned char g, unsigned char b)
{
	if (0 != SDL_SetRenderDrawColor(drawer, r, g, b, 255))
	{
		cerr << "Erreur SDL_SetRenderDrawColor : " << SDL_GetError() << endl;
		affEnd();
	}
}

void SDLRenderer::render()
{
  SDL_Rect where;
  int facteur = (int)((float)(width)/m_terrain->getWidth());
  SDL_RenderClear(drawer);
  for(int i = 0; i < m_terrain->getWidth(); i++)
  {
    for(int j = 0; j < m_terrain->getHeight(); j++)
    {
        if(m_terrain->getTile(i, j) == '#')
        {
          where = {i*facteur, width - j*facteur - 1*facteur, facteur, facteur};
          SDL_RenderCopy(drawer, tMur, NULL, &where);
        }
        else if(m_terrain->getTile(i, j) == '.')
        {
          where = {i*facteur+(facteur/4), width - j*(facteur) - 1*facteur + facteur/4, facteur/2, facteur/2};
          SDL_RenderCopy(drawer, tPacgum, NULL, &where);
        }
        else if(m_terrain->getTile(i, j) == 'S')
        {
          where = {(i*facteur), width - j*(facteur) - 1*facteur, facteur, facteur};
          SDL_RenderCopy(drawer, tPacgum, NULL, &where);
        }
    }
  }

  for(int i = 0; i < (int)m_tabPacman->size(); i++)
  {
    Point PacPos = m_tabPacman->at(i)->getPos();
    where = {(int)(PacPos.x * facteur), (int)(width - PacPos.y * facteur - 1*facteur), (int)facteur, (int)facteur};
    SDL_RenderCopy(drawer, tPacman, NULL, &where);
  }
  
  SDL_RenderPresent(drawer);
  napms(50);
}

UserInput SDLRenderer::getInput()
{
  while(SDL_PollEvent(&input))
  {
    if(input.type == SDL_QUIT) return QUIT;
    else if(input.type)
    {
      if(input.key.keysym.sym == SDL_SCANCODE_ESCAPE) return QUIT;
      else if(input.key.keysym.sym == SDLK_ESCAPE) return QUIT;
      else if(input.key.keysym.sym == SDLK_z || input.key.keysym.sym == SDLK_UP) return Z;
      else if(input.key.keysym.sym == SDLK_s || input.key.keysym.sym == SDLK_DOWN) return S;
      else if(input.key.keysym.sym == SDLK_q || input.key.keysym.sym == SDLK_LEFT) return Q;
      else if(input.key.keysym.sym == SDLK_d || input.key.keysym.sym == SDLK_RIGHT) return D;
    }
  }

  return IDLE;
}

void SDLRenderer::affEnd()
{
  SDL_DestroyTexture(tMur);
  SDL_DestroyTexture(tPacman);
  SDL_DestroyTexture(tPacgum);
  SDL_DestroyTexture(tSuperPacgum);
	SDL_DestroyRenderer(drawer);
	SDL_DestroyWindow(fenetre);
	SDL_Quit();
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
