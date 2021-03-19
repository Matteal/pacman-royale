#include <iostream>
#include "network/Gateway.h"

// sleep
#include <stdio.h>
#include <time.h>
#include <unistd.h>



int main(int argc, char *argv[])
{
  Server srv;
  srv.startListening();//wait_for_connection();
  srv.run();
  std::cout<<"un pti thread est fermé on dirais"<<std::endl;
  return 0;
}
