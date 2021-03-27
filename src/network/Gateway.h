#ifndef GATEWAY_H
#define GATEWAY_H

#include "connection.h"
#include <thread>
#include "Room.h"

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
  void authentification();
  void printMessage(Message msg);

  bool isConnectionActive();

  // créer un accesseur
  connection* m_co;

private:
  int m_socket;
  int m_isActive;
};


class Server
{
public:
  /**
    @brief Constructeur par défaut: crée le socket serveur
  */
  Server();
  ~Server();

  /**
    @brief gère les connections entrantes au début du programme
  */
  void authentification(int socket);

  /**
    @brief écoute les connections entrantes et les renvoie dans wait_for_connection
  */
  void startListening();

  /**
    @brief éteind le service d'écoute de connections
  */
  void stopListening();

  /**
    @brief bloque le thread principal
  */
  void run();

private:
  std::thread* connectionListener;
  void wait_for_connection();

  int m_fdSocket;
  int m_socket;

  Room* m_room;

};

#endif //GATEWAY_H
