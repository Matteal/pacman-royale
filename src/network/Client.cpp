#include "Gateway.h"

#include <unistd.h> //close
#include <iostream>
// ***************
// * CLIENT SIDE *
// ***************

Client::Client(const char* serverName) : m_co(nullptr), m_isActive(true)
{
  //initialise WinSocks sous windows
  #ifdef _WIN32
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2,2), &WSAData);
  #endif // _WIN32

  struct sockaddr_in addr_serveur;
  struct hostent *serveur;

  m_socket = socket(AF_INET, SOCK_STREAM, 0);       /* création prise */
  if (m_socket < 0)
    {perror("socket"); exit(-1);}

  serveur = gethostbyname(serverName); /* recherche adresse serveur */
  if (serveur == NULL)
    {perror("gethostbyname"); exit(-1);}

  addr_serveur.sin_family = AF_INET;
  addr_serveur.sin_port = htons(PORT);
  addr_serveur.sin_addr = *(struct in_addr *) serveur->h_addr;

  if (connect(m_socket, /* connexion au serveur */
              (struct sockaddr *) &addr_serveur, sizeof addr_serveur) < 0)
    {perror("connect"); exit(-1);}

  // création de la connection
  m_co = new connection(m_socket);
  authentification();
  m_co->setCallback(std::bind(&Client::printMessage, this, std::placeholders::_1));
}

void Client::authentification()
{
  std::string input="";


  std::cout<<"Entrez un nom de X caractères (todo: limiter la taille)\n>";
  std::getline(std::cin, input);
  m_co->sendMessage(create_message(NEW_CONNECTION, input)); //vérifier la taille du pseudo

  std::cout<<"@fin de l'authentification\n"<<std::endl;

}

Client::~Client()
{
  close(m_socket);
  m_socket = -1;

  delete(m_co);
  m_co = nullptr;

  //ferme la bibliothèque WinSock
  #ifdef _WIN32
    WSACleanup();
  #endif // _WIN32
}

bool Client::isConnectionActive()
{
  return m_isActive;
}

void Client::printMessage(Message msg)
{
  if(msg.type == CLOSE_CONNECTION)
  {
    std::cout<<"Vous avez été déconnecté par le serveur"<<std::endl;
    m_isActive = false;
  }
  std::cout << "MSG: " << msg.corps << std::endl;
}
