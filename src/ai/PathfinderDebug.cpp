
#include "Pathfinder.h"

#include "../game/Renderer.h"

SDLRenderer::SDLRenderer() : Renderer()
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
  if (!(IMG_Init(imgFlags) & imgFlags))
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
  tLose = loadTexture("./data/death.png");
  tWin = loadTexture("./data/win.png");
  tPress = loadTexture("./data/press.png");
  tStart = loadTexture("./data/start.png");
}

void SDLRenderer::setWindowColor(unsigned char r, unsigned char g, unsigned char b)
{
  if (0 != SDL_SetRenderDrawColor(drawer, r, g, b, 255))
  {
    cerr << "Erreur SDL_SetRenderDrawColor : " << SDL_GetError() << endl;
    affEnd();
  }
}

void SDLRenderer::render(int state, Pacman Pac)
{
  SDL_Rect where;
  SDL_RendererFlip flip;
  SDL_Rect tWhere[6] =
      {
          {0, 0, 15, 15},
          {15, 0, 15, 15},
          {30, 0, 15, 15},
          {45, 0, 15, 15},
          {60, 0, 15, 15},
          {75, 0, 15, 15}};
  SDL_Point centre = {10, 10};
  int facteur = (int)((float)(width) / m_terrain->getWidth());
  SDL_RenderClear(drawer);
  if (state == 0 || state == 42)
  {
    for (int i = 0; i < m_terrain->getWidth(); i++)
    {
      for (int j = 0; j < m_terrain->getHeight(); j++)
      {
        if (m_terrain->getTile(i, j) != ' ' && m_terrain->getTile(i, j) != '.' && m_terrain->getTile(i, j) != 'S')
        {
          where = {i * facteur, width - j * facteur - 1 * facteur, facteur, facteur};

          int indice, rotation;

          tileToTexture(m_terrain->getTile({(float)i, (float)j}), indice, rotation, flip);
          SDL_RenderCopyEx(drawer, tMur, &tWhere[indice], &where, rotation, &centre, flip);
        }
        else if (m_terrain->getTile(i, j) == '.')
        {
          where = {i * facteur + (facteur / 4), width - j * (facteur)-1 * facteur + facteur / 4, facteur / 2, facteur / 2};
          SDL_RenderCopy(drawer, tPacgum, NULL, &where);
        }
        else if (m_terrain->getTile(i, j) == 'S')
        {
          where = {(i * facteur), width - j * (facteur)-1 * facteur, facteur, facteur};
          SDL_RenderCopy(drawer, tPacgum, NULL, &where);
        }
      }
    }
    SDL_Rect PacWalk[2] =
        {
            {0, 0, 15, 15},
            {15, 0, 15, 15}};
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
             {120, 30, 15, 15}}};
    for (int i = 0; i < (int)m_tabPacman->size(); i++)
    {

      Point PacPos = m_tabPacman->at(i)->getPos();
      SDL_Rect Tex;
      flip = SDL_FLIP_NONE;
      int r;
      where = {(int)(PacPos.x * facteur), (int)(width - PacPos.y * facteur - 1 * facteur), (int)facteur, (int)facteur};
      if (m_tabPacman->at(i)->getGhost())
      {
        r = 0;
        int texIndice = (i - 1) % 4;

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
        if (m_tabPacman->at(i)->getDir() == LEFT || m_tabPacman->at(i)->getDir() == DOWN)
          r = 90 * m_tabPacman->at(i)->getDir();
        else if (m_tabPacman->at(i)->getDir() == RIGHT)
          r = 0;
        else
          r = -90;
        Tex = PacWalk[m_tabPacman->at(i)->_animState];
        if (m_tabPacman->at(i)->_animState == 0)
          m_tabPacman->at(i)->_animState++;
        else
          m_tabPacman->at(i)->_animState--;

        if (m_tabPacman->at(i)->_isSuper && m_tabPacman->at(i)->_timer % 2 == 0)
          where = {0, 0, 0, 0};
      }

      SDL_RenderCopyEx(drawer, tPacman, &Tex, &where, r, &centre, flip);
    }

    if (state == 42)
    {
      SDL_Rect w = {width / 4, width / 4, width / 2, width / 2};
      SDL_RenderCopy(drawer, tPress, NULL, &w);
    }
  }
  else if (state == -1)
  {
    SDL_Rect death = {45 + 15 * m_tabPacman->at(0)->_animState, 0, 15, 15};
    where = {width / 2 - (int)(facteur), (int)(width - 15 * facteur - 1 * facteur), (int)facteur * 2, (int)facteur * 2};
    if (m_tabPacman->at(0)->_timer < 101)
      m_tabPacman->at(0)->_timer += 4;
    else
      m_tabPacman->at(0)->_timer = 1000;
    m_tabPacman->at(0)->_animState = m_tabPacman->at(0)->_timer / 10;

    SDL_RenderCopy(drawer, tLose, NULL, NULL);
    SDL_RenderCopy(drawer, tPacman, &death, &where);
    SDL_Rect w = {width / 4, width / 4, width / 2, width / 2};
    SDL_RenderCopy(drawer, tPress, NULL, &w);
  }
  else if (state == 1)
  {
    SDL_RenderCopy(drawer, tWin, NULL, NULL);
    SDL_Rect w = {width / 4, width / 4, width / 2, width / 2};
    SDL_RenderCopy(drawer, tPress, NULL, &w);
  }
  else if (state == 43)
  {
    SDL_RenderCopy(drawer, tStart, NULL, NULL);
    SDL_Rect w = {width / 4, width / 4, width / 2, width / 2};
    SDL_RenderCopy(drawer, tPress, NULL, &w);
  }
  SDL_RenderPresent(drawer);
  napms(50);
}

UserInput SDLRenderer::getInput()
{
  while (SDL_PollEvent(&input))
  {
    if (input.type == SDL_QUIT)
      return QUIT;
    else if (input.type == SDL_KEYUP)
    {
      if (input.key.keysym.sym == SDLK_ESCAPE)
        return QUIT;
      else if (input.key.keysym.sym == SDLK_z || input.key.keysym.sym == SDLK_UP)
        return Z;
      else if (input.key.keysym.sym == SDLK_s || input.key.keysym.sym == SDLK_DOWN)
        return S;
      else if (input.key.keysym.sym == SDLK_q || input.key.keysym.sym == SDLK_LEFT)
        return Q;
      else if (input.key.keysym.sym == SDLK_d || input.key.keysym.sym == SDLK_RIGHT)
        return D;
      else if (input.key.keysym.sym == SDLK_p || input.key.keysym.sym == SDLK_SPACE)
        return PAUSE;
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

void SDLRenderer::tileToTexture(char c, int &index, int &rotation, SDL_RendererFlip &flip)
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

SDL_Texture *SDLRenderer::loadTexture(string path)
{
  //Texture a renvoyé
  SDL_Texture *R = NULL;

  //On initialise une surface
  SDL_Surface *iniImage = IMG_Load(path.c_str());
  if (iniImage == nullptr) // Si Erreur
  {
    cerr << "Impossible IMG LOAD : Chemin = " << path.c_str() << " ERROR = " << IMG_GetError() << endl;
  }
  else
  {
    //On transforme la surface en Texture
    R = SDL_CreateTextureFromSurface(drawer, iniImage);
    if (R == nullptr)
    {
      cerr << "Impossible TEXTURE CREATE : Chemin = " << path.c_str() << " ERROR = " << SDL_GetError() << endl;
    }

    //Suppression de la surface temporaire
    SDL_FreeSurface(iniImage);
  }

  return R;
}


&