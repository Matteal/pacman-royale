#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#define PORT 8000
#define TAILLE_TAMPON 256
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


// [enum]
enum connection_type{
  MESSAGE = 0, // Nouveau message
  NEW_CONNECTION = 1, // demande de connection
  CLOSE_CONNECTION = 2, // Information : personne ayant quittée la room
  SERVER_LOG = 3,
  MANUAL = 4, // A utiliser uniquement pour des échanges en dur
  INSTRUCTION = 5, // instructions de déplacement utilisées par Game
  KILL_LISTENING_THREAD = 62, // tue le thread /!\ a l'utilisation
  TEST = 63,}; // Information : nouvelle personne connectée à la room
// ![enum]

// [Message]
struct Message {
  connection_type type;
  std::string corps;
};

/*
  @brief crée et initialise une structure Message
*/
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
    @brief renvoie la première requette reçue
  */
  Message readMessage();

  /**
    @brief lance un thread qui gère la lecture des messages reçus
  */
  void startReadAsync();

  /**
    @brief arrète la lecture en asynchrone
    termine le thread lancé par connection::startReadAsync
  */
  void stopReadAsync();


protected:
  bool isAsync;
  int m_socket;
  std::thread* m_computeMessage;
  std::thread* tWaitForMessage;
  std::function<void(const Message msg)> _callback;

  /**
    @brief écoute l'entrée de messages sur la connection en asynchrone
  */
  void readMessageAsync();

  /**
    @brief écoute l'entrée et renvoie la première requette reçue
    retourne si tout c'est bien passé
    @param msg [out]: message récupéré
  */
  bool readOneMessage(Message& msg);

  std::mutex mtxSend;
  std::mutex mtxRecv;
};


#endif // CONNECTION_HPP
