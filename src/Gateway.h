#ifndef GATEWAY_H
#define GATEWAY_H

#include "connection.h"
#include <thread>

class Client
{
public:
  Client(const char* serverName);
  ~Client();

  connection* m_co;

private:
  int m_socket;
};


class Server
{
public:
  Server();
  ~Server();

  void startListening(void (*function)(int));
  std::thread* connectionListener;
private:
  void wait_for_connection();
  void (*m_function)(int);
  int m_fdSocket;
  int m_socket;

};

#endif //GATEWAY_H
