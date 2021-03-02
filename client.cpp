#include <iostream>
#include "src/Gateway.h"

#include <stdio.h>
#include <time.h>
#include <unistd.h>



int main(int argc, char *argv[]){
  //usage
  if (argc != 3) {
      printf("Usage: %s serveur message\n", argv[0]);
      perror("nombre de paramètres incorrect");
      return -1;
  }


  //L.join();
  Client cli(argv[1]);

  // connection co(1);
  // co.setDestination(&Client::printMessage, &cli);
  // co.readMessage();
  // //cli.m_co->setDestination(&Client::printMessage, &cli);
  // cli.m_co->startReadMessage();
  cli.m_co->sendMessage(TEST, argv[2]);
  sleep(1);
  cli.m_co->sendMessage(TEST, "au revoir");
  sleep(2);


  return 0;
}
