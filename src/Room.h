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
  void sendAll(std::string message);
  static void receiveMessage(Message msg);
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
