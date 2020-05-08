#pragma once

#include <pthread.h>
#include <boost/utility.hpp>

// class wrapper for posix threads
class Thread : public boost::noncopyable
{
public:
    Thread();
    virtual ~Thread();
    
    static void Sleep(unsigned long msecs);

    virtual void run();
    virtual void stop();
    virtual bool running() const {return _running;};
    virtual void join();
    //devuelve si esta en buen estado
    operator bool() const;

protected:
    enum tState { OK, ERROR };
    bool _running;
    bool _mustStop;
    tState _state;
    
    virtual void main() = 0;
    
private:    
    pthread_t _thread;
    pthread_attr_t _attr;
    //funcion que realmente ejecuta el thread tomando this como argumento
    static void* static_run(void* arg);
};
