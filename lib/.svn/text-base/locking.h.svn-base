//header de clase Locking
#ifndef __LOCKING_H__
#define __LOCKING_H__
#include "common/mutex.h"

/**
 * Wrapper para Mutex
 * 
 * @sa Mutex
 */
class Locking {
  private:
    Mutex &_m;
    //constructor de copia y operator= ocultos
    Locking(const Locking&);
    Locking& operator=(const Locking&);
  public:
    /// constructor con mutex => hace lock
    Locking(Mutex &m):_m(m){_m.lock();};
    
    /// destructor que libera mutex
    virtual ~Locking(){_m.unlock();};
};
#endif
