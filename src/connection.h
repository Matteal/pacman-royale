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
void print_message(Message msg);
// ![Message]


class connection
{
public:
  connection(int fdSocket);
  ~connection();

  void quit();

  template<typename A, typename B>
  void setDestination(A func_ptr, B obj_ptr)
  {
    _callback = std::bind(func_ptr, obj_ptr, std::placeholders::_1);
  }

  void sendMessage(Message message);
  void startReadMessage();
  //TODO void StopReadMessage()
  void readMessage();

  //void printMessage(Message msg);
  std::thread* tWaitForMessage;
protected:
  int m_socket;
  //void (*m_functionCall)(Message);
  std::function<void(const Message& msg)> _callback;


  std::mutex mtxSend;
  std::mutex mtxRecv;
};


#endif // CONNECTION_HPP
