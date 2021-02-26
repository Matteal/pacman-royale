#include "Gateway.h"

#include <unistd.h> //close

// ***************
// * CLIENT SIDE *
// ***************

Client::Client(const char* serverName) : m_co(nullptr)
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

  m_co = new connection(m_socket);
}

Client::~Client()
{
  close(m_socket);
  m_socket = -1;

  //ferme la bibliothèque WinSock
  #ifdef _WIN32
    WSACleanup();
  #endif // _WIN32
}
