#include "common/timer.h"

struct timeval Timer::_start;
bool Timer::_initialized = false;

//devuelve time actual en milisegundos desde primer llamada
int Timer::getTime() {
  //setea timer la primera vez q se la invoca
  if (!_initialized) {
    gettimeofday(&_start,0);
    _initialized = true;
    return 0;
  }
  struct timeval end;
  gettimeofday(&end,0);
  //calcula segundos transcurridos
  int i = (end.tv_sec - _start.tv_sec)*1000;
  //suma milisegundos pasados
  i += ((end.tv_usec - _start.tv_usec)/1000);
  return i;
}
