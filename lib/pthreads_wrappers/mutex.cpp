//implementacion de clase Mutex
#include "common/mutex.h"

Mutex::Mutex(){
  _state = OK;
  if (pthread_mutex_init(&_mutex,0) != 0)
    _state = ERROR;
}

Mutex::~Mutex(){
  pthread_mutex_destroy(&_mutex);
}

int Mutex::lock(){
  pthread_mutex_lock(&_mutex);
  return 0;
}

int Mutex::unlock(){
  pthread_mutex_unlock(&_mutex);
  return 0;
}

Mutex::operator bool() const {
  return (_state == OK);
}
