#include "Gateway.h"

#include <unistd.h> //close
#include <iostream>
// ***************
// * CLIENT SIDE *
// ***************

Client::Client(const char* serverName) : m_co(nullptr), m_isActive(true), m_isGameLaunched(false)
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
  // std::string input="";
  //
  //
  // std::cout<<"Entrez un nom de X caractères (todo: limiter la taille)\n>";
  // std::getline(std::cin, input);
  // m_co->sendMessage(create_message(NEW_CONNECTION, input)); //vérifier la taille du pseudo
  //
  // std::cout<<"@fin de l'authentification\n"<<std::endl;

}

void Client::run()
{
  m_co->startReadAsync();
  std::string input;
  std::cout<<"entrez 'exit' pour quitter"<<std::endl;
  while(m_isActive)
  {
    input="";
    std::cout<<"> ";
    std::getline(std::cin, input); //protège des espaces

    if(isConnectionActive())
    {
      if(input[0]=='!')
      {
        input.erase(input.begin());//supprime le ! du message
        m_co->sendMessage(create_message(INSTRUCTION, input));
      }
      else if(m_isGameLaunched)
      {
        std::cout<<"CLIENT> La Game est lancée" <<std::endl;
        m_game = new Game(34, 34 , 3630);
        m_game->setCallback(std::bind(&Client::setInstructionTo, this, std::placeholders::_2));
        m_game ->Start(CONSOLE);
        //m_game->run();
        m_isGameLaunched = false;
      }

      else
        m_co->sendMessage(create_message(MESSAGE, input));
    }
    else
    {
      std::cout<<"programme terminé"<<std::endl;
      return;
    }
  }
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

void Client::setInstructionTo(std::string instruction)
{
  m_co->sendMessage(create_message(INSTRUCTION, instruction));
}

void Client::printMessage(Message msg)
{
  switch(msg.type)
  {
    case CLOSE_CONNECTION:
      std::cout<<"CLIENT> Vous avez été déconnecté pour la raison suivante : "<< msg.corps << std::endl;
      m_isActive = false;
      exit(3); // met fin au programme
      break;
    case MESSAGE:
      std::cout << "Message reçu: " << msg.corps << std::endl;
      break;
    case TEST:
      std::cout << "(LOG DU SERV) " << msg.corps << std::endl;
      break;
    case INSTRUCTION:
      assert(m_isActive); //le programme n'est pas sensé recevoir d'instruction avant que la game aie commencée
      //std::cout << "Instruction>" << msg.corps << std::endl;
      m_game->addInstruction(msg.corps);
      break;
    case NEW_GAME:
      std::cout <<"Le signal de début de partie est recu, appuie sur entrée pour débloquer" <<std::endl;
      std::cout << msg.corps << std::endl;
      m_isGameLaunched = true;
      std::cin.putback ( '\0');
      break;
    default:
      std::cout << "CLIENT> message de type " << msg.type << " non reconnu :" << msg.corps << std::endl;
      break;
  }
}
