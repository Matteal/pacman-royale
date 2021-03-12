#include "connection.h"

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <assert.h>
//sleep
#include <stdio.h>
#include <time.h>
#include <unistd.h>

//TODO: trouver un autre moyen que KILL_LISTENING_THREAD pour terminer la lecture en asynchrone


Message create_message(connection_type typeConnection, std::string txt="")
{
  Message msg;
  msg.type = typeConnection;
  msg.corps = txt;
  return msg;
}

void print_message(Message msg)
{
  std::cout<<"******************"<<std::endl;
  std::cout<<"Taille du message : "<< msg.corps.length() <<std::endl;
  std::cout<<"Type de message : " << msg.type << std::endl;
  std::cout << msg.corps << std::endl;
  std::cout<<"******************"<<std::endl;
}

connection::connection(int fdSocket) : m_socket(fdSocket), _callback(nullptr), tWaitForMessage(nullptr)
{}

connection::~connection()
{
  sendMessage(create_message(CLOSE_CONNECTION));
  close(m_socket);
  m_socket = -1;
}


//=========================
//***** COMMUNICATION *****
//=========================

//requete format : En TETE [Taille message, type du message]
                  //Corps Message


void connection::sendMessage(Message message)
{
  mtxSend.lock();
    char requete[TAILLE_TAMPON];

    int longueur =  message.corps.length()+4;
    assert(longueur<TAILLE_TAMPON);

    char taille_message[2];
    taille_message[0] = ((int)longueur/256)-128;
    taille_message[1] = (longueur%256)-128;

    longueur = snprintf(requete, longueur,
                        "%c%c%c%s",
                        taille_message[0], taille_message[1], (char)message.type,
                        message.corps.c_str());
    send(m_socket, requete, longueur, 0);
  mtxSend.unlock();
}

void connection::startReadAsync()
{
    tWaitForMessage = new std::thread(&connection::readMessageAsync, this);
    tWaitForMessage->detach();
}

void connection::stopReadAsync()
{
  if(tWaitForMessage!=nullptr)
  {
    sendMessage(create_message(KILL_LISTENING_THREAD, " ")) ;
    m_computeMessage->join(); //attend que la dernière requete se soit exécutée
    m_computeMessage=nullptr;
  }
}

Message connection::readMessage()
{
  Message msg;

  //gestion des erreurs
  assert(tWaitForMessage==nullptr); // doit être en mode synchrone
  if(!readOneMessage(msg))
    perror("Erreur lors de la lecture du thread");

  return msg;
}

void connection::readMessageAsync()
{
  char tampon[TAILLE_TAMPON];
  Message msg;
  sleep(2);
  while(readOneMessage(msg))
  {
    // m_computeMessage contient le thread de la dernière requette en date
    m_computeMessage=new std::thread(_callback, msg);
  }
  sendMessage(create_message(KILL_LISTENING_THREAD, " "));
  std::cout<<"mort du thread d'écoute X)"<<std::endl;
}

bool connection::readOneMessage(Message& msg)
{
  char tampon[TAILLE_TAMPON];

  #ifdef _WIN32
      int sd = recv(m_socket, tampon, TAILLE_TAMPON, 0);     /* lecture par bloc */
  #else //Linux
      int sd = read(m_socket, tampon, TAILLE_TAMPON);
  #endif // _WIN32

  if(sd==0 || (connection_type)(int)tampon[2]==KILL_LISTENING_THREAD)
    return false;

  std::string request;
  int taille_requette = (tampon[1]+128) + (tampon[0]+128)*256;

  request.reserve(taille_requette-4);
  for(int i= 3; i<taille_requette-1; i++)
  {
    request.push_back(tampon[i]);
  }
  msg = create_message((connection_type)(int)tampon[2], request);

  return true;
}
