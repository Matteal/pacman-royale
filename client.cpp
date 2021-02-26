#include <iostream>
#include "src/Gateway.h"

#include <stdio.h>
#include <time.h>
#include <unistd.h>


//#include <thread>
void Log()
{
  std::cout<<"log\n";
}

void callFunction(void  (*fct)(void))
{
  std::cout<<"LOGEUR";
  fct();
}

void printMessage(Message msg)
{
  std::cout<<"print message : "<<std::endl;
  std::cout<<"Taille du message : "<< msg[0]-'\0' <<std::endl;
  std::cout<<"Type de message : " << msg[1]-'\0' << std::endl;
  msg[msg[0]-'\0'+2] = '\0';
  std::cout << &msg[2] << std::endl;
}

int main(int argc, char *argv[]){
  //usage
  if (argc != 3) {
      printf("Usage: %s serveur message\n", argv[0]);
      perror("nombre de paramètres incorrect");
      return -1;
  }

  void (*ptf)(void);
  ptf = &Log;
  //std::thread L(callFunction);
  callFunction(ptf);
  //L.join();
  Client cli(argv[1]);
  cli.m_co->setMessageDestination(&printMessage);
  cli.m_co->startReadMessage();
  cli.m_co->sendMessage(TEST, argv[2]);
  //sleep(1);
  cli.m_co->sendMessage(TEST, "au revoir");

  sleep(2);

  return 0;
}
