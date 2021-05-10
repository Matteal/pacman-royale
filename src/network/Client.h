#ifndef GATEWAY_H
#define GATEWAY_H

#include "connection.h"
#include <thread>
#include "../game/Game.h"

class Client
{
public:
	/**
	@brief Constructeur par défaut: crée le socket Client
	*/
	Client(const char* serverName);
	~Client();
	/**
	@brief établis la communication avec le serveur
	*/
	void run(launch aff);
	void printMessage(Message msg);

	bool isConnectionActive();

	// créer un accesseur
	connection* m_co;

	void setInstructionTo(std::string instruction);

private:
	Game* m_game;
	int m_socket;
	bool m_isActive, m_isGameLaunched;

	vector<string> instructionHeap;
	mutex mtxHeap;

	void mainloop(launch aff);
};

#endif //GATEWAY_H
