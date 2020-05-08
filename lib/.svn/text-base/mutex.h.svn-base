#ifndef __MUTEX_H__
#define __MUTEX_H__
#include <pthread.h>

/** Clase para utilizar los mutex de POSIX
 *
 */
class Mutex {
  private:
    typedef enum MutexState{OK,ERROR};
    pthread_mutex_t _mutex;
    MutexState _state;
    //constructor de copia y operator= ocultos
    Mutex(const Mutex&);
    Mutex& operator=(const Mutex&);
  
  public:
  	///constructor
    Mutex();
    ///destructor
    virtual ~Mutex();
    ///bloquea mutex
    int lock();
    ///desbloqea mutex
    int unlock();    
    ///devuelve estado de mutex
    operator bool() const;
};


class Lock
{
public:
    Lock(Mutex& m) : _mutex(m) {
        _mutex.lock(); 
    }
    ~Lock() { _mutex.unlock(); }

private:
    Mutex& _mutex;
};

// shared variable
// include a variable and its mutex
// warning: Don't use safe functions (set, get) within a {lock(); ... unlock();} block. Use unsafe function instead
template <class T>
class ThreadSharedVar
{       
public:
    ThreadSharedVar(const T& t) : _t(t) {}
    ThreadSharedVar() {}

    T get() {
        lock();
        T res = unsafeGet();
        unlock();
        return res;
    }

    void set(const T& newVal) {
        lock();
        unsafeSet(newVal);
        unlock();
    }

    void lock() { _mutex.lock(); }
    void unlock() { _mutex.unlock(); }

    const T& unsafeGet() { return _t; }
    void unsafeSet(const T& new_val) { _t = new_val; }

private:
    T _t;
    Mutex _mutex;
};

#endif
