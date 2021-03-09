#include "Gateway.h"

#include <iostream>
#include <unistd.h> //close
#include <thread>


// ***************
// * SERVER SIDE *
// ***************

Server::Server() : connectionListener(nullptr)
{
	//initialise WinSocks sous windows
  #ifdef _WIN32
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,2), &WSAData);
  #endif // _WIN32

  struct sockaddr_in adresse_serveur;
  size_t taille_adresse_serveur;

  m_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(m_socket < 0)
    {perror("socket"); exit(-1);}

  /* 3.2 Remplissage de l'adresse de réception
     (protocole Internet TCP-IP, réception acceptée sur toutes
     les adresses IP du serveur, numéro de port indiqué)*/

  adresse_serveur.sin_family = AF_INET;
  adresse_serveur.sin_port = htons(PORT);
  adresse_serveur.sin_addr.s_addr = INADDR_ANY;
  /* 3.3 Association du socket au port de réception */
  taille_adresse_serveur = sizeof adresse_serveur;

  if (bind(m_socket,
    (struct sockaddr *) &adresse_serveur, taille_adresse_serveur) < 0)
    {perror("Binding socket failed");exit(-1);}

  // Ouverture du service
  listen(m_socket, 10);//4 for max connections in wait
}

Server::~Server()
{
  close(m_fdSocket);
  m_fdSocket = -1;

  //ferme la bibliothèque WinSock
  #ifdef _WIN32
    WSACleanup();
  #endif // _WIN32
}


void Server::startListening(void (*function)(int  ))
{
  m_function = function;
  connectionListener = new std::thread(&Server::wait_for_connection, this);

  m_room = new Room();
  //m_function = m_room->
}

void Server::wait_for_connection()
{
  while(true)
  {
    printf("SERVEUR> Le serveur écoute le port %d\n", PORT);
    while ((m_fdSocket = accept(m_socket, NULL, NULL)) < 0)
    {
      if (errno != EINTR)
      {
        perror("newcoming connection failed");
        exit(3);
      }
    }
    std::clog<<"connection entrante"<<std::endl;

    //create a new thread for the incomming connection
    std::thread a(&Server::authentification, this, m_fdSocket);
    a.detach();
  }
}

void Server::authentification(int socket) const
{
  connection* co = new connection(socket);
  m_room->addConnection(co);
  //co.
  //co->setMessageDestination(&Room::addConnection, m_room, co);
}
