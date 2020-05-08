/*implementacion de clase Thread*/
#include "common/thread.h"
#include <unistd.h>

//constructor
Thread::Thread(){
  _state = OK;
  _running = false;
  _mustStop = false;
  //inicializa los atributos del thread
  pthread_attr_init(&_attr);
  //setea al thread como "esperable"
  pthread_attr_setdetachstate(&_attr,PTHREAD_CREATE_JOINABLE);
}

void* Thread::static_run(void* arg) {
  ((Thread *)arg)->main();
  ((Thread *)arg)->_running = false;  
  return 0;
}

//lanza el thread
void Thread::run() {
  _running = true;
  if (pthread_create(&_thread,&_attr,static_run,(void*)this) != 0)
    _state = ERROR;  
}

//detiene el thread
void Thread::stop() {
  _mustStop = true;
}

//espera a que termine el thread
void Thread::join() {
  if (_running)
    if (pthread_join(_thread,0) != 0)
      _state = ERROR;
}

//espera que el thread termine y luego lo libera
Thread::~Thread() {
  if (_running) {
    stop();
    join();
  }
}

//duerme el thread
void Thread::Sleep(unsigned long msecs) {
  usleep(msecs*1000);
}

//operator bool
Thread::operator bool() const {
  return (_state == OK);
}
