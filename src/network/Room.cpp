#include "Room.h"
#include <iostream>
#include <string>
#include <functional>
#include <algorithm>

// sleep
#include <stdio.h>
#include <time.h>
#include <unistd.h>


Room::Room() : m_game(nullptr), isGameLaunched(false), limite_joueur(1)
{
  std::cout<<"une room a été crée!"<<std::endl;
}

Room::~Room()
{
  std::cout<< "ROOM> Fermeture" << std::endl;
  sendAll(create_message(CLOSE_CONNECTION, "Fermeture de la room"));
  delete m_game;
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
    s.co = co;
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

void Room::sendInstructionTo(int idJoueur, std::string message)
{
  // for(unsigned i = 0; i < m_list.size(); i++)
  // {
  //   if(m_list[i].id == idJoueur) // recherche du bon joueur
  //   {std::cout<<"#########"<<std::endl;
  //     m_list[i].co->sendMessage(create_message(INSTRUCTION, message));
  //   }
  // }
  sendAll(create_message(INSTRUCTION, message));
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
      break;
    case INSTRUCTION:
      assert(isGameLaunched); // On est pas sensé avoir d'instruction tant que la partie n'as pas commencée
      for(unsigned i = 0; i < m_list.size(); i++)
      {
        if(m_list[i].co == co)
        {
          m_game->addInstruction(msg.corps + to_string(i));
        }
      }
      break;
    case CLOSE_CONNECTION: //cherche la connection et la ferme
      mtxList.lock();
        for(unsigned i = 0; i < m_list.size(); i++)
        {
          if(m_list[i].co == co)
          {
            m_list.erase(m_list.begin()+i);
            std::cout << "l'utilisateur n°" << i << " c'est déconnecté" << std::endl;
            std::cout<<"Nombre de connexions actives: "<<m_list.size()<<std::endl;
            break;
          }
        }
      mtxList.unlock();
    break;
  }
  //if(msg.type!=CLOSE_CONNECTION)
  //  print_message(msg);
}



void Room::run()
{
  while(!isGameLaunched)
  {
    //std::cout<<"."<<st0d::flush;
    // on attend qu'il y aie assez de joueurs dans la partie
    sleep(1);
  }
  std::cout<<"ROOM> La partie va commencer!"<<std::endl;

  //création de la Game
  int tailleX = 34;
  int tailleY = 30;
  int seed = time(0); //time permet de générer une seed en fonction de l'heure
  m_game = new Game(tailleX, tailleY, seed);
  m_game->init(limite_joueur, 0 , -1);
  m_game->setCallback(std::bind(&Room::sendInstructionTo, this, std::placeholders::_1, std::placeholders::_2));

  mtxList.lock();
    for (char i = 0; i < m_list.size(); i++)
    {
      std::string msgNewGame;
      msgNewGame.push_back(i-128);  //Numéro joueur
      msgNewGame.push_back(tailleX-128); //taille_terrain_x
      msgNewGame.push_back(tailleY-128); //taille_terrain_y
      
      std::cout << msgNewGame <<std::endl;
      m_list[i].id = i;
      m_list[i].co->sendMessage(create_message(NEW_GAME, msgNewGame + to_string(seed)));
    }
  mtxList.unlock();

  m_game->mainloopServer();
}
