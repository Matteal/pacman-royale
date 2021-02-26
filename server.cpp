#include <iostream>
#include "src/Gateway.h"

#include "thread"

#include <stdio.h>
#include <time.h>
#include <unistd.h>

// void receptFunction(int socket)
// {
//   connection co(socket);
//   std::cout<<"Start of the thread"<<std::endl;
//   co.send_message(MESSAGE, "hello world ! :3");
//   co.read_message();
//   sleep(5);
//   std::cout<<"End of the thread"<<std::endl;
// }
void printMessage(Message msg)
{
  std::cout<<"print message : "<<std::endl;
  std::cout<<"Taille du message : "<< msg[0]-'\0' <<std::endl;
  std::cout<<"Type de message : " << msg[1]-'\0' << std::endl;
  msg[msg[0]-'\0'+2] = '\0';
  std::cout << &msg[2] << std::endl;
}

void receptFunction(int fdSocket)
{
  std::cout<<"numero thread: "<<fdSocket<<std::endl;

  connection* co = new connection(fdSocket);
  co->setMessageDestination(&printMessage);
  co->startReadMessage();
  co->sendMessage(TEST, "bonjour");

  sleep(5);
  co->quit();
}

int main(int argc, char *argv[])
{
  Server srv;
  srv.startListening(&receptFunction);//wait_for_connection();

  srv.connectionListener->join();
  return 0;
}
