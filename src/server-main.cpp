#include <iostream>
#include "network/Gateway.h"

// sleep
#include <stdio.h>
#include <time.h>
#include <unistd.h>


void receptFunction(int fdSocket)
{
  std::cout<<"numero thread: "<<fdSocket<<std::endl;

  connection* co = new connection(fdSocket);
  //co->setDestination(&printMessage);
  co->startReadAsync();
  co->sendMessage(create_message(TEST, "bonjour"));

  sleep(5);
}

int main(int argc, char *argv[])
{
  Server srv;
  srv.startListening();//wait_for_connection();
  srv.run();
  std::cout<<"un pti thread est fermé on dirais"<<std::endl;
  return 0;
}
