#include "connection.h"

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <cassert>
//sleep
#include <stdio.h>
#include <time.h>
#include <unistd.h>

//TODO: trouver un autre moyen que KILL_LISTENING_THREAD pour terminer la lecture en asynchrone


Message create_message(connection_type typeConnection, std::string txt)
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

connection::connection(int fdSocket) : m_socket(fdSocket), tWaitForMessage(nullptr), _callback(nullptr)
{}

connection::~connection()
{
  stopReadAsync();
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


    int longueur =  message.corps.length();
    assert(longueur>0); //Le message doit faire au moins un caractère
    assert(longueur<=TAILLE_TAMPON); // Le message doit faire au plus 256 caractères

    char taille_message = ((int)longueur)-128-1;
    longueur = snprintf(requete, longueur+3,
                        "%c%c%s",
                        taille_message, (char)message.type,
                        message.corps.c_str());
    if(send(m_socket, requete, longueur, 0)==-1)
      std::cerr<<"Tu viens d'envoyer un message dans le vent : "<<errno<<std::endl;

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
    tWaitForMessage->join(); //attend que la dernière requete se soit exécutée
    tWaitForMessage=nullptr;
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
  Message msg;
  sleep(2);
  while(readOneMessage(msg))
  {
    // m_computeMessage contient le thread de la dernière requette en date
    m_computeMessage=new std::thread(_callback, msg);
  }
  sendMessage(create_message(KILL_LISTENING_THREAD, " "));
  std::cout<<"mort du thread d'écoute X)"<<std::endl;
  m_computeMessage->join(); // on attend que la dernière requete aie finie
  if(_callback!=nullptr)
    _callback(create_message(CLOSE_CONNECTION, ""));
}

bool connection::readOneMessage(Message& msg)
{
  char tampon[TAILLE_TAMPON];

  #ifdef _WIN32
      int sd = recv(m_socket, tampon, TAILLE_TAMPON, 0);     /* lecture par bloc */
  #else //Linux
      int sd = read(m_socket, tampon, TAILLE_TAMPON);
  #endif // _WIN32

  if(sd==0 || (connection_type)(int)tampon[1]==KILL_LISTENING_THREAD)
    return false;

  std::string request;
  int taille_requette = tampon[0]+128+1;

  request.reserve(taille_requette);
  for(int i= 2; i<taille_requette+2; i++)
  {
    request.push_back(tampon[i]);
  }
  msg = create_message((connection_type)(int)tampon[1], request);

  return true;
}
