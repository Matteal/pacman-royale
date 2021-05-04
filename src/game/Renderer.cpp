#include "Renderer.h"

#include <curses.h>
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

	if(m_tabPacman->at(indexPacman)->_state == 42 || m_tabPacman->at(indexPacman)->_state == 0)
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
	else if(m_tabPacman->at(indexPacman)->_state == -1)
	{
		mvprintw((LINES / 2), (COLS / 2) - 6, "YOU ARE DEAD");
		mvprintw((LINES / 2) + 1, (COLS / 2) - 25/2, "PRESS SPACE OR P TO RESET");
		to_clear = true;
	}
	else if(m_tabPacman->at(indexPacman)->_state == 1)
	{
		mvprintw((LINES / 2), (COLS / 2) - 4, "YOU WIN!");
		mvprintw((LINES / 2) + 1, (COLS / 2) - 25/2, "PRESS SPACE OR P TO RESET");
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
