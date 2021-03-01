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

void redirectMessage(Message msg)
{
  std::cout<<"******************"<<std::endl;
  std::cout<<"Taille du message : "<< msg[0]-'\0' <<std::endl;
  std::cout<<"Type de message : " << msg[1]-'\0' << std::endl;
  msg[msg[0]-'\0'+2] = '\0';
  std::cout << &msg[2] << std::endl;
  std::cout<<"******************"<<std::endl;
}
using std::placeholders::_1;
void Room::addConnection(connection* co) //TODO ajouter un utilisateur (dérivé de connection)
{
  //std::cout<<"Add connection"<<std::endl;
  //co->setMessageDestination(std::bind(&Room::receiveMessage) this);
  //co->setDestination(&Room::receiveMessage, this);
  co->startReadMessage();

  Session s;
  s.co= co;
  s.id = m_list.size();
  m_list.push_back(s);

  sendAll("Un nouvel utilisateur est arrivé");
}

void Room::sendAll(std::string message)
{
  std::cout<<"size : "<<m_list.size()<<std::endl;
  for(int i=0; i<m_list.size(); i++)
  {
    m_list[i].co->sendMessage(MESSAGE, message);
  }
}

void Room::receiveMessage(Message msg)
{
  std::cout<<"******************"<<std::endl;
  std::cout<<"message received : "<<std::endl;
  std::cout<<"Taille du message : "<< msg[0]-'\0' <<std::endl;
  std::cout<<"Type de message : " << msg[1]-'\0' << std::endl;
  msg[msg[0]-'\0'+2] = '\0';
  std::cout << &msg[2] << std::endl;
  std::cout<<"******************"<<std::endl;
}
