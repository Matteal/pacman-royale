#include "Room.h"
#include <iostream>
#include <string>
#include <functional>
#include <algorithm>

// sleep
#include <stdio.h>
#include <time.h>
#include <unistd.h>

                              //time permet de générer une seed en fonction de l'heure
Room::Room() : m_game(35, 35, time(0)), isGameLaunched(false), limite_joueur(1)
{
  std::cout<<"une room a été crée!"<<std::endl;
}

Room::~Room()
{
  sendAll(create_message(TEST, "Fermeture de la room"));
}

void Room::addConnection(connection* co) //TODO ajouter un utilisateur (dérivé de connection)
{
  // s'assure que l'on ne dépasse pas la limite de joueur authorisé

  inscription.lock();

    if(m_list.size() == limite_joueur)
    {
      co->sendMessage(create_message(CLOSE_CONNECTION, "La room est pleine, réessayez un peu plus tard"));
      inscription.unlock();
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

  inscription.unlock();

  sendAll(create_message(TEST, "Un nouvel utilisateur est arrive"));
  std::cout << "ROOM> Nombre de connexions actives: " << m_list.size() << std::endl;

  //démare la partie quand le nombre de joueur est atteint
  if(m_list.size()==limite_joueur)
    isGameLaunched = true;


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
        assert(m_list.size()==indice+1); // la recherche doit être la bonne
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


void Room::run()
{
  while(!isGameLaunched)
  {
    std::cout<<"."<<std::flush;
    // on attend qu'il y aie assez de joueurs dans la partie
    sleep(1);

  }
  std::cout<<"ROOM> La partie va commencer!"<<std::endl;
}
