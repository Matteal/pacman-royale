#include "connection.h"

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

//sleep
#include <stdio.h>
#include <time.h>
#include <unistd.h>

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

connection::connection(int fdSocket) : m_socket(fdSocket), _callback(nullptr)
{}

connection::~connection()
{
  sendMessage(create_message(CLOSE_CONNECTION));
  close(m_socket);
  m_socket = -1;
}

void connection::quit()
{
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

    int longueur = snprintf(requete, (int)message.corps.length()+3,
                        "%c%c%s",
                        (char)message.corps.length(), (char)message.type,
                        message.corps.c_str());
    send(m_socket, requete, longueur, 0);
  mtxSend.unlock();
}

void connection::startReadMessage()
{


    tWaitForMessage = new std::thread(&connection::readMessage, this);
    tWaitForMessage->detach();

    // Décriptage de la requete

}

void connection::readMessage()
{
  char tampon[TAILLE_TAMPON];
  Message msg;
  sleep(2);
  while(readOneMessage(msg))
  {
    std::thread computeMessage(_callback, msg);
    computeMessage.detach();
  }
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

  if(sd==0)
    return false;

  std::string request;
  request.reserve((int)tampon[0]-1);
  for(int i= 2; i<tampon[0]+2; i++)
  {
    request.push_back(tampon[i]);
  }
  msg = create_message((connection_type)(int)tampon[1], request);

  return true;
}
