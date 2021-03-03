#ifndef SESSION_H
#define SESSION_H

#include "connection.h"

class Session : connection
{
public:
  Session(int fdSocket);
  
  template<typename A, typename B>
  void setDestination(A func_ptr, B obj_ptr, connection* socket = nullptr)
  {
    //_callback = std::bind(func_ptr, obj_ptr, std::placeholders::_1);
    //int* socket = nullptr;
    //if(socket == nullptr) // sans suivi de la connection
      _callback = std::bind(func_ptr, obj_ptr, std::placeholders::_1, socket);
  //  else //avec suivi de la connection
      //_callback = std::bind(func_ptr, obj_ptr, std::placeholders::_1, this);
  }
protected:
}

#endif // SESSION_H
