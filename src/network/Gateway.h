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
  void authentification();
  void printMessage(Message msg);

  // créer un accesseur
  connection* m_co;

private:
  int m_socket;
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
  void startListening(void (*function)(int));
  std::thread* connectionListener;
private:
  void wait_for_connection();
  void (*m_function)(int);
  int m_fdSocket;
  int m_socket;

  Room* m_room;

};

#endif //GATEWAY_H
