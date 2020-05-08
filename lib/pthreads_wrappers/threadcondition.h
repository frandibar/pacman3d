#ifndef THREADCONDITION_H_
#define THREADCONDITION_H_

#include <pthread.h>

/**
 * C++ wrapper class for pthread ThreadCondition
 */
template <class T>
class ThreadCondition
{
public:
    ThreadCondition(const T& t) : _t(t) {
        pthread_mutex_init(&_mutex, NULL);
        pthread_cond_init(&_cond, NULL);
    }

    ~ThreadCondition() {
        pthread_cond_destroy(&_cond);
        pthread_mutex_destroy(&_mutex);
    }

    /// Change the ThreadCondition's value
    void set(const T& newval) {
        pthread_mutex_lock(&_mutex);
        _t = newval;
        if (_t)
            pthread_cond_broadcast(&_cond);
        pthread_mutex_unlock(&_mutex);
    }

    T get() {
        T val;
        pthread_mutex_lock(&_mutex);
        val = _t;
        pthread_mutex_unlock(&_mutex);
        return val;
    }
    
    /// Waits for the ThreadCondition to be true
    void waitFor() {
        pthread_mutex_lock(&_mutex);
        while (!_t)
            pthread_cond_wait(&_cond, &_mutex);
        pthread_mutex_unlock(&_mutex);
    }

private:    
    T _t;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
};

#endif /*THREADCONDITION_H_*/
