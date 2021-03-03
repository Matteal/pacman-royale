#ifndef GATEWAY_H
#define GATEWAY_H

#include "connection.h"
#include <thread>
#include "Room.h"

class Client
{
public:
  Client(const char* serverName);
  ~Client();
  void printMessage(Message msg, connection*);

  connection* m_co;

private:
  int m_socket;
};


class Server
{
public:
  Server();
  ~Server();

  void authentification(int socket) const;
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
