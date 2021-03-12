#include <iostream>
#include "network/Gateway.h"

//sleep
#include <stdio.h>
#include <time.h>
#include <unistd.h>



int main(int argc, char *argv[]){
  //usage
  if (argc < 2) {
      printf("Usage: %s serveur (message)\n", argv[0]);
      perror("nombre de paramètres incorrect");
      return -1;
  }

  Client cli(argv[1]);

  cli.m_co->startReadAsync();
  sleep(1);

  std::string input;
  std::cout<<"entrez 'exit' pour quitter"<<std::endl;
  while(input != "exit")
  {
    input="";
    std::cout<<"> ";
    std::getline(std::cin, input); //protège des espaces
    cli.m_co->sendMessage(create_message(MESSAGE, input));
  }

  return 0;
}
