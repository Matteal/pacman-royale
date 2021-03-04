#include "Room.h"
#include <iostream>
#include <string>
#include <functional>
Room::Room()
{
  std::cout<<"une room a été crée!"<<std::endl;
}

Room::~Room()
{
  sendAll(create_message(TEST, "Fermeture de la room"));
}

void Room::addConnection(connection* co) //TODO ajouter un utilisateur (dérivé de connection)
{
  std::cout<<"Add connection"<<std::endl;
  co->setCallback(std::bind(&Room::receiveMessage, this, std::placeholders::_1, co));
  co->startReadMessage();
  co->sendMessage(create_message(TEST, "Bienvenue sur la room"));

  Session s;
  s.co= co;
  s.id = m_list.size();
  m_list.push_back(s);

  sendAll(create_message(TEST, "Un nouvel utilisateur est arrive"));
}

void Room::sendAll(Message message)
{
  std::cout<<"size : "<<m_list.size()<<std::endl;
  for(int i=0; i<(int)m_list.size(); i++)
  {
    m_list[i].co->sendMessage(message);
  }
}

void Room::receiveMessage(Message msg, connection* co)
{
  switch(msg.type)
  {
    case MESSAGE:
      sendAll(msg);
      break;
    case TEST:
      std::cout<<"ceci est un test I guess.."<<std::endl;
  }
    std::cout<<"******************"<<std::endl;
    std::cout<<"Taille du message : "<< msg.corps.size() <<std::endl;
    std::cout<<"Type de message : " << msg.type << std::endl;
    std::cout << msg.corps << std::endl;
    std::cout<<"******************"<<std::endl;
  }
