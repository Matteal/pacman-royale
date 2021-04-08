#ifndef ROOM_H
#define ROOM_H

#include "connection.h"
#include "../game/Game.h"
#include <vector>
#include <mutex>

class Room
{
public:
	Room();
	~Room();

	/**
	@brief ajoute une connection à la liste des connections active
	*/
	void addConnection(connection* co);

	/**
	@brief envoie un message à toutes les connections actives
	*/
	void sendAll(Message message);

	/**
	@brief attend qu'il y aie assez de connection active et lance la Game
	*/
	void run();

	void sendInstructionTo(int idJoueur, std::string message);

private:

	/**
	@brief c'est ici que les connections envoient leur message
	*/
	void receiveMessage(Message msg, connection*);
	Game* m_game;
	struct Session
	{
	int id;
	connection* co;
	};
	std::vector<Session> m_list;
	std::mutex mtxList; // fait en sorte que m_list ne soit pas modifié à deux endroits différents
	std::mutex inscription; // pas plus de deux inscriptions en meme temps

	bool isGameLaunched;
	const unsigned int limite_joueur;
};

#endif //ROOM_H
