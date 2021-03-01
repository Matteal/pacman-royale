#include <iostream>
#include "src/Gateway.h"

#include <stdio.h>
#include <time.h>
#include <unistd.h>


// void printMessage(Message msg)
// {
//   std::cout<<"******************"<<std::endl;
//   std::cout<<"Taille du message : "<< msg[0]-'\0' <<std::endl;
//   std::cout<<"Type de message : " << msg[1]-'\0' << std::endl;
//   msg[msg[0]-'\0'+2] = '\0';
//   std::cout << &msg[2] << std::endl;
//   std::cout<<"******************"<<std::endl;
// }

int main(int argc, char *argv[]){
  //usage
  if (argc != 3) {
      printf("Usage: %s serveur message\n", argv[0]);
      perror("nombre de paramètres incorrect");
      return -1;
  }


  //L.join();
  Client cli(argv[1]);

  //connection co(1);
  co.setDestination(&Client::printMessage, &cli);
  // cli.printMessage("bonjour toi");
  // //cli.m_co->setDestination(&Client::printMessage, &cli);
  // cli.m_co->startReadMessage();
  // cli.m_co->sendMessage(TEST, argv[2]);
  // sleep(1);
  // cli.m_co->sendMessage(TEST, "au revoir");


  return 0;
}
