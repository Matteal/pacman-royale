#ifndef GAME_H
#define GAME_H

#include <ctime>
//#include <curses.h>
#include <mutex>
#include <functional>

#include "Terrain.h"
#include "Pacman.h"
#include "Pacgum.h"

#include "Renderer.h"

enum launch{CONSOLE, SDL};

const float FPS = 60;
const float UPDATEFREQ = ((float)1 / (float)FPS) * 1000.0f;

class Game
{
public:
	/// @brief Constructeur de Game
	/// @details
	/// Construit le jeu avec une taille de terrain et sa seed
	/// @param t_width: largeur du terrain
	/// @param t_height: hauteur du terrain
	/// @param seed: entier
	Game(int t_width = 50, int t_height = 50, int t_seed=177013);

	/// @brief Destructeur de game
	/// @details
	/// Detruit les pointeurs de game
	~Game();

	/// @brief Initialise la partie
	/// @details
	/// Utilisation: Ajoute aux pacman pj joueurs a tuer et pnj fantomes
	/// @param pj: Entier non signé indiquant le nombre de joueurs
	/// @param pnj: Entier non signé indiquant le nombre de fantomes
	/// @param numParticipant Entier qui indique le joueur local (-1 si en debug ou si serveur)
	void init(unsigned pj, unsigned pnj, int numParticipant = -1);   // Initialisation du jeu (chargement de la carte, des contrôles, etc)

	/// @brief Initialise l'affichage
	/// @details
	/// Utilisation: On lui donne un SDLRenderer ou un CONSOLERenderer, il va initialisé les variables interne de chacun
	/// @param rend: Un Renderer (CONSOLERenderer ou SDLRenderer)
	void initRenderer(Renderer* rend);

	/// @brief Envoit un instruction aux joueurs ou au serveur
	/// @details
	/// Utilisation: Envoit un string au serveur ou aux clients (si serveur)
	/// @param msg: string a envoyé
	void addInstruction(const string msg);

	/// @brief définit la fonction de traitement des instructions
	/// @details
	/// Utilisation: Lui passer le nom d'une fonction qui va servir a traiter les données
	/// @param _callback: Pointeur vers fonction
	void setCallback(std::function<void(int idJoueur, std::string message)> _callback) {_instructionCallback = _callback;};

	/// @brief Ajoute un pacman a la partie
	/// @details
	/// Utilisation: Recupere l'indice dans le tableau, si c'est un joueur et si c'est un fantome
	/// @param i: index dans le tableau
	/// @param player: bool, true si c'est un joueur false sinon
	/// @param ghost: bool, true si c'est un fantome, false sinon
	void addPacman(int i, bool player, bool ghost);

	/// @brief Renvoit le pacman principal de la session
	/// @details
	/// Utilisation: Renvoit un pointeur vers le pacman du joueur
	/// @return Un pointeur vers pacman
	Pacman* getPac();

	/// @brief Recupere les input clients
	/// @details
	/// Utilisation:
	/// @param Pac: Pointeur vers le pacman de la session
	/// @param quit:  Bool, si true, arrête la fenêtre
	/// @param dirNext: recupere la direction futur
	void getInput(Pacman* Pac, bool& quit, direction& dirNext);

	/// @brief Actualise les directions des pacmans/fantomes
	/// @details
	/// Utilisation: L'utiliser avant walk(), change les direction des pacmans
	void turn();

	/// @brief Actualise la position des Pacmans/fantomes suivant leurs direction
	/// @details
	/// Utilisation: L'utiliser aprés turn(), change la position des pacmans s'ils peuvent bouger
	void walk();

	/// @brief Actualise l'affichage et l'état des pacgum/super pacgum
	/// @details
	/// Utilisation: L'utiliser aprés le mouvement des pacmans, se sert de la pos des pacmans pour déterminer les pacgum manger
	/// @param generatePacgum: bool, si vrai genere et envoit la position des super pacgum a la machine distante
	void actuPacgum(bool generatePacgum);

	/// @brief Démarre le chrono d'une itération de la boucle
	/// @details
	/// Utilisation: Au démarrage de l'itération, gère l'attente pour les FPS
	void startChrono();

 	/// @brief Démarre le chrono d'une itération de la boucle
	/// @details
	/// Utilisation: Au démarrage de l'itération, gère l'attente pour les FPS
	void stopChrono();

	/// @brief Accesseur de la liste de pacman
	/// @details
	/// Renvoit le tableau de Pacman
	/// @return Vecteur de lien vers pacmans
	std::vector<Pacman*>* getPacList() {return &pacmanList;}

	/// @param pacgumList Vecteur de pacgum
	vector<Pacgum> pacgumList;

	/// @param _t Objet de type terrain
	Terrain _t;

private:
	 //passé en public pour accès direct pendant les tests
	Pacman* Pac;

	/// @param _superPacgum: Nombre max de superPacgum
	int _superPacgum;

	/// @param pacmanList: vecteur de lien vers pacman
	std::vector<Pacman*> pacmanList;

	/// @param pacgumEaten: vecteur de lien vers les pacgum mangés
	vector<int> pacgumEaten;

	/// @param nbEntityRemain: entier représentant le nombre de joueurs
	int nbEntityRemain = 0;

	/// @param _instructionCallback: Lien vers fonction
	std::function<void(int idJoueur, std::string message)> _instructionCallback;

	/// @param instructionHeap: Vecteur de string recevant les instructions
	vector<string> instructionHeap;

	/// @param mtxHeap: Verroue sur une variable pour éviter les problème de threading
	mutex mtxHeap;

	/// @param m_renderer: Objet de type renderer servant au rendu du client (SDLRenderer CONSOLERenderer)
	Renderer* m_renderer;

	/// @brief définit la fonction de traitement des instructions
	/// @details
	/// Utilisation: Lui passer le nom d'une fonction qui va servir a traiter les données
	/// @param Pac: Lien vers pacman
	/// @param Dir: Direction futur du Pacman
	/// @return Si le pacman peut changer de direction ou non
	bool canTurn(Pacman* Pac, direction Dir);

	/// @brief Genere les pacgum au démarrage de la partie
	/// @details
	/// Utilisation: A lancer a l'init de game, ajoute les pacgums dans le terrain et dans le tableau de game
	void generatePacgum();

	/// @brief Change la direction des fantomes
	/// @details
	/// Utilisation: Lui passer un pacman fantome
	/// @param Pac: Lien vers pacman
	void actuDirGhost(Pacman * pac);

	/// @brief Initialise un joueur
	/// @details
	/// Utilisation: A mettre dans l'initialisation, ajoute un joueur au tableau
	void initJoueur();

	// Ces deux variables serviront à calculer l'écart entre deux frames
	// Le calcul de la différence servira à maintenir 60 FPS constants
	chrono::_V2::steady_clock::time_point startT, endT;
	std::chrono::milliseconds deltaT;
};



#endif //GAME_H
