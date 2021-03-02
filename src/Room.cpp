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
  sendAll("Fermeture de la room");
}

void Room::addConnection(connection* co) //TODO ajouter un utilisateur (dérivé de connection)
{
  std::cout<<"Add connection"<<std::endl;
  co->setDestination(&Room::receiveMessage, this);
  co->startReadMessage();
  co->sendMessage(TEST, "Bienvenue sur la room");

  Session s;
  s.co= co;
  s.id = m_list.size();
  m_list.push_back(s);

  sendAll("Un nouvel utilisateur est arrive");
}

void Room::sendAll(std::string message)
{
  std::cout<<"size : "<<m_list.size()<<std::endl;
  for(int i=0; i<(int)m_list.size(); i++)
  {
    m_list[i].co->sendMessage(TEST, message);
  }
}

void Room::receiveMessage(Message msg)
{
  sendAll(msg);
  std::cout<<"******************"<<std::endl;
  std::cout<<"message received : "<<std::endl;
  std::cout<<"Taille du message : "<< msg[0]-'\0' <<std::endl;
  std::cout<<"Type de message : " << msg[1]-'\0' << std::endl;
  std::cout << &msg[2] << std::endl;
  std::cout<<"******************"<<std::endl;
}
