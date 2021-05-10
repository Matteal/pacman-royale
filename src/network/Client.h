#ifndef GATEWAY_H
#define GATEWAY_H

#include "connection.h"
#include <thread>
#include "../game/Game.h"

class Client
{
public:
	/// @brief Constructeur de Client
	/// @details
	/// Crée un socket et établis une connexion avec le serveur
	/// @param serverName: nom du serveur
	Client(const char* serverName);

	/// @brief Destructeur de Client
	~Client();

	/// @brief programme principal
	/// @details
	/// Attend les informations du serveur,
	/// lance mainloop quand l'information du début de partie est reçu
	/// @param aff: mode d'affichage
	void run(launch aff);

	/// @brief traite les messages reçus
	/// @details
	/// réceptionne et traite les messages selon leur type
	/// @param msg: Message à traiter
	void handleMessage(Message msg);

	/// @brief renvoi l'état de la connexion
	/// @details permet de savoir si la connexion à déjà été fermée ou non
	bool isConnectionActive();

	/// @brief Callback des instructions
	/// @details
	/// permet de définir un callback pour l'objet Game, 
	/// renvoie les messages passé en argument au serveur
	/// @param instruction: message à envoyer au serveur
	void setInstructionTo(std::string instruction);

private:
	connection* m_co;
	int m_socket;
	bool m_isActive, m_isGameLaunched;

	Game* m_game;

	vector<string> instructionHeap;
	mutex mtxHeap;

	void mainloop(launch aff);
};

#endif //GATEWAY_H
