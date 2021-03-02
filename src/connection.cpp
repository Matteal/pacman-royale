#include "connection.h"

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

connection::connection(int fdSocket) : m_socket(fdSocket), _callback(nullptr)
{}

connection::~connection()
{
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


void connection::sendMessage(connection_type type, std::string message)
{
  mtxSend.lock();
    char requete[TAILLE_TAMPON];

    int longueur = snprintf(requete, (int)message.length()+3,
                        "%c%c%s\0",
                        (int)message.length()+'\0', type+'\0',
                        message.c_str());
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

  #ifdef _WIN32
      while(recv(m_socket, tampon, TAILLE_TAMPON, 0)!=0)     /* lecture par bloc */
  #else //Linux
      while(read(m_socket, tampon, TAILLE_TAMPON)!=0)
  #endif // _WIN32
  {
  std::cout<<"Message recu!"<<std::endl;

    // std::cout<<"Taille du message : "<< tampon[0]-'\0' <<std::endl;
    // std::cout<<"Type de message : " << tampon[1]-'\0' << std::endl;
    // tampon[tampon[0]-'\0'+2] = '\0';
    // std::cout << &tampon[2] << std::endl;

    Message t = tampon;
    _callback(tampon);
    // std::thread computeMessage(_callback, t);
    // computeMessage.detach();
  }
  std::cout<<"mort X)"<<std::endl;
  //mort du thread
}

void connection::printMessage(Message msg)
{
  std::cout<<"******************"<<std::endl;
  std::cout<<"Taille du message : "<< msg[0]-'\0' <<std::endl;
  std::cout<<"Type de message : " << msg[1]-'\0' << std::endl;
  msg[msg[0]-'\0'+2] = '\0';
  std::cout << &msg[2] << std::endl;
  std::cout<<"******************"<<std::endl;
}
