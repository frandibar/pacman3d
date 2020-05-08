#ifndef __TIMER_H__
#define __TIMER_H__
#include <sys/time.h> 

/**Clase para medir milisegundos
 * 
 */
class Timer {
  private:
    static struct timeval _start;
    static bool _initialized;    
  public:
    ///devuelve time actual en milisegundos desde primer llamada
    static int getTime();
};
#endif /* __TIMER_H__ */
