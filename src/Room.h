#ifndef ROOM_H
#define ROOM_H

#include "connection.h"

#include <vector>

class Room
{
public:
  Room();
  ~Room();

  void addConnection(connection* co);
  void sendAll(Message message);
  void receiveMessage(Message msg);
  void receiveMessageCo(Message msg, connection* co);
private:
  struct Session
  {
    int id;
    std::string name;
    connection* co;
  };
  std::vector<Session> m_list;
};

#endif //ROOM_H
