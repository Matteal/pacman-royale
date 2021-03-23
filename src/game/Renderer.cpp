#include "Renderer.h"

ConsoleRenderer::ConsoleRenderer(): Renderer()
{
  m_window = subwin(stdscr, 10, 10, LINES, COLS); // Créer une fenetre terminal
  initscr(); // initialise la fenetre
  noecho();  // Option fenetre
  scrollok(stdscr, TRUE);
  nodelay(stdscr, TRUE); // Transforme getch en fonction non bloquante

  refresh(); // Rafraichissement page avant le start
}

ConsoleRenderer::~ConsoleRenderer()
{
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
