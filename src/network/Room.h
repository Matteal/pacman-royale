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

  /**
    @brief ajoute une connection à la liste des connections active
  */
  void addConnection(connection* co);

  /**
    @brief envoie un message à toutes les connections actives
  */
  void sendAll(Message message);


private:

  /**
    @brief c'est ici que les connections envoient leur message
  */
  void receiveMessage(Message msg, connection*);
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
