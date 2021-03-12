#ifndef ROOM_H
#define ROOM_H

#include "connection.h"
#include "../game/Game.h"
#include <vector>

class Room
{
public:
  Room();
  ~Room();

  void addConnection(connection* co);
  void sendAll(Message message);
  void receiveMessage(Message msg, connection*);
private:
  Game m_game;
  struct Session
  {
    int id;
    std::string name;
    connection* co;
  };
  std::vector<Session> m_list;
};

#endif //ROOM_H
