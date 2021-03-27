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
  cout<<"here"<<endl;
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

void ConsoleRenderer::render(int state)
{
  clear(); // Nettoie la fenetre
  if(state == 0)
  {
    // dessinne le terrain ligne par ligne
    char line[m_terrain->getWidth()*2+1]; // definition d'une ligne, *2 pour espacer le terrain
    for(int j = 0; j < m_terrain->getHeight(); j++) // On parcour les colones
    {
        for(int i = 0; i < m_terrain->getWidth() * 2; i++) // on parcour les lignes
        {
            if(i%2 == 0) // si i est pair, on affiche un char du terrain
            {
              line[i] = m_terrain->getTile(i/2, j);
              if(line[i] != '.' && line[i] != ' ' && line[i] != 'S') line[i] = '#';
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
            if(m_tabPacman->at(indice)->getGhost())
              line[m_tabPacman->at(indice)->getIndexX()*2] = 'n';
            else if(m_tabPacman->at(indice)->_isSuper)
              line[m_tabPacman->at(indice)->getIndexX()*2] = '0';
            else 
              line[m_tabPacman->at(indice)->getIndexX()*2] = 'o';
          }
        }
        line[m_terrain->getWidth()*2] = '\0'; // on termine la ligne
        mvprintw((LINES / 2) - j + (m_terrain->getWidth() /2), (COLS / 2) - (m_terrain->getWidth()*2 / 2), line); // on affiche la ligne
    }
  
  }
  else if(state == -1)
  {
    mvprintw((LINES / 2), (COLS / 2) - 6, "YOU ARE DEAD");
    mvprintw((LINES / 2) + 1, (COLS / 2) - 8, "NOT BIG SURPRISE");
  }
  else if(state == 1)
  {
    mvprintw((LINES / 2), (COLS / 2) - 4, "YOU WIN!");
    mvprintw((LINES / 2) + 1, (COLS / 2) - 8, "NOT BIG SURPRISE");
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

  tMur = loadTexture("./data/tileset.png");
  tPacman = loadTexture("./data/pacmantileset.png");
  tPacgum = loadTexture("./data/superPacgum.png");
  tSuperPacgum = loadTexture("./data/superPacgum.png");
  tSuperPacgum = loadTexture("./data/superPacgum.png");
  tLose = loadTexture("./data/death.jpg");
  tWin = loadTexture("./data/win.png");

}

void SDLRenderer::setWindowColor(unsigned char r, unsigned char g, unsigned char b)
{
	if (0 != SDL_SetRenderDrawColor(drawer, r, g, b, 255))
	{
		cerr << "Erreur SDL_SetRenderDrawColor : " << SDL_GetError() << endl;
		affEnd();
	}
}

void SDLRenderer::render(int state)
{
  SDL_Rect where;
  SDL_RendererFlip flip;
  SDL_Rect tWhere[6]=
  {
    {0,  0, 15, 15},
    {15, 0, 15, 15},
    {30, 0, 15, 15},
    {45, 0, 15, 15},
    {60, 0, 15, 15},
    {75, 0, 15, 15}
  };
  SDL_Point centre = {10, 10};
  int facteur = (int)((float)(width)/m_terrain->getWidth());
  SDL_RenderClear(drawer);
  if(state == 0)
  {
    for(int i = 0; i < m_terrain->getWidth(); i++)
    {
      for(int j = 0; j < m_terrain->getHeight(); j++)
      {
          if(m_terrain->getTile(i, j) != ' ' && m_terrain->getTile(i, j) != '.' && m_terrain->getTile(i, j) != 'S')
          {
            where = {i*facteur, width - j*facteur - 1*facteur, facteur, facteur};
            
            int indice, rotation;
            
            tileToTexture(m_terrain->getTile({(float)i, (float)j}), indice, rotation, flip);
            SDL_RenderCopyEx(drawer, tMur, &tWhere[indice], &where, rotation, &centre, flip);
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
    SDL_Rect PacWalk[2] =
    {
      {0, 0, 15, 15},
      {15, 0, 15, 15}
    };
    SDL_Rect GhostWalk[4][3] =
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
      {120, 30, 15, 15}}
    };
    for(int i = 0; i < (int)m_tabPacman->size(); i++)
    {

      Point PacPos = m_tabPacman->at(i)->getPos();
      SDL_Rect Tex;
      flip = SDL_FLIP_NONE;
      int r;
      where = {(int)(PacPos.x * facteur), (int)(width - PacPos.y * facteur - 1*facteur), (int)facteur, (int)facteur};
      if(m_tabPacman->at(i)->getGhost())
      {
        r = 0;
        int texIndice = (i - 1)%4;
        
        switch (m_tabPacman->at(i)->getDir())
        {
        case UP:
          Tex = GhostWalk[texIndice][1];
          break;

        case DOWN:
          Tex = GhostWalk[texIndice][2];
          break;

        case LEFT:
          flip = SDL_FLIP_HORIZONTAL;
          Tex = GhostWalk[texIndice][0];
          break;

        case RIGHT:
          Tex = GhostWalk[texIndice][0];
          break;
        }
      }
      else
      {
        if(m_tabPacman->at(i)->getDir() == LEFT || m_tabPacman->at(i)->getDir() == DOWN) r = 90 * m_tabPacman->at(i)->getDir();
        else if(m_tabPacman->at(i)->getDir() == RIGHT) r = 0;
        else r = -90;
        Tex = PacWalk[m_tabPacman->at(i)->_animState];
        if(m_tabPacman->at(i)->_animState == 0) m_tabPacman->at(i)->_animState++;
        else m_tabPacman->at(i)->_animState--;

        if(m_tabPacman->at(i)->_isSuper && m_tabPacman->at(i)->_timer%2 == 0) where = {0, 0, 0, 0};
      }
      
      SDL_RenderCopyEx(drawer, tPacman, &Tex, &where, r, &centre, flip);
    }
  }
  else if(state == -1)
  {
    SDL_Rect death = {45 + 15 * m_tabPacman->at(0)->_animState, 0, 15, 15};
    where = {width/2 - (int)(facteur), (int)(width - 15 * facteur - 1*facteur), (int)facteur*2, (int)facteur*2};
    if(m_tabPacman->at(0)->_timer < 101) m_tabPacman->at(0)->_timer+=4;
    else m_tabPacman->at(0)->_timer = 1000;
    m_tabPacman->at(0)->_animState = m_tabPacman->at(0)->_timer/10;
    
    SDL_RenderCopy(drawer, tLose, NULL, NULL);
    SDL_RenderCopy(drawer, tPacman, &death, &where);
  }
  else if(state == 1)
  { 
    SDL_RenderCopy(drawer, tWin, NULL, NULL);
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
      if(input.key.keysym.sym == SDLK_ESCAPE) return QUIT;
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
