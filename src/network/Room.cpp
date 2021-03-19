#include "Room.h"
#include <iostream>
#include <string>
#include <functional>
#include <algorithm>

Room::Room() : m_game()
{

  std::cout<<"une room a été crée!"<<std::endl;
}

Room::~Room()
{
  sendAll(create_message(TEST, "Fermeture de la room"));
}

void Room::addConnection(connection* co) //TODO ajouter un utilisateur (dérivé de connection)
{
  // récupère le nom du joueur
  std::string input="";

  Message msg = co->readMessage();
  if(msg.type == CLOSE_CONNECTION)
  {
    std::cout<<"Un utilisateur à tenté de se connecter sans succès"<<std::endl;
    return;
  }

  // attache la connexion entrante à la liste
  Session s;
  s.co= co;
  s.id = -1;

  mtxList.lock();
    m_list.push_back(s);
  mtxList.unlock();

  co->setCallback(std::bind(&Room::receiveMessage, this, std::placeholders::_1, co));
  co->startReadAsync();

  sendAll(create_message(TEST, "Un nouvel utilisateur est arrive"));
  std::cout<<"Nombre de connexions actives: "<<m_list.size()<<std::endl;
}

void Room::sendAll(Message message)
{
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
    case CLOSE_CONNECTION: //cherche la connection et la ferme
      int indice = 0;
      while(m_list[indice].co != co)
      {
        indice ++;
      }


      //delete(m_list[indice].co);
      mtxList.lock();
        m_list.erase(m_list.begin()+indice);
      mtxList.unlock();

      std::cout << "l'utilisateur n°" << indice << " c'est déconnecté" << std::endl;
      std::cout<<"Nombre de connexions actives: "<<m_list.size()<<std::endl;
  }
  if(msg.type!=CLOSE_CONNECTION)
    print_message(msg);
}
