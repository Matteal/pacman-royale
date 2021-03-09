#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#define PORT 8000
#define TAILLE_TAMPON 255
//Works using the TCP protocol

#ifdef _WIN32
    //sous windows, compiler avec l'option -lws2_32
    #include <winsock2.h>
    typedef int socklen_t;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h> //gethostbyname
#endif

#include <string>
#include <thread>
#include <mutex>

#include <functional>

//typedef std::string Message;

// [enum]
enum connection_type{
  MESSAGE = 0, // Nouveau message
  NEW_CONNECTION = 1, // demande de connection
  CLOSE_CONNECTION = 2, // Information : personne ayant quittée la room
  SERVER_LOG = 3,
  TEST = 63,}; // Information : nouvelle personne connectée à la room
// ![enum]

// [Message]
struct Message {
  connection_type type;
  std::string corps;
};

Message create_message(connection_type, std::string);

/**
  @brief affiche a la console le contenu du Message
*/
void print_message(Message msg);
// ![Message]


class connection
{
public:
  /**
    @brief constructeur de la construction
    @param fdSocket: numéro de socket renvoyé par accept()
  */
  connection(int fdSocket);
  ~connection();

  //todo
  void quit();

  /**
    @brief définis la fonction a appeller lorsqu'un message est reçu
  */
  void setCallback(std::function<void(const Message& msg)> callbackFct)
  {
    _callback = callbackFct;
  }

  /**
    @brief envoie un Message à la machine distante
  */
  void sendMessage(Message message);
  /**
    @brief lance un thread qui gère la lecture des messages reçus
  */
  void startReadMessage();

  /**
    @brief écoute l'entrée de messages sur la connection
    @todo: placer ceci en private
  */
  void readMessage();

  //todo cacher ca
  std::thread* tWaitForMessage;
protected:
  int m_socket;
  //void (*m_functionCall)(Message);
  std::function<void(const Message& msg)> _callback;


  std::mutex mtxSend;
  std::mutex mtxRecv;
};


#endif // CONNECTION_HPP
