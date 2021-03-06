#pragma once

#include "../common/thread.h"
#include "../common/socketprotocol.h"
#include "threadgame.h"

/**Clase para manejar el hilo de atencion de clientes
 * Los clientes se conectan al puerto del server
 * Desde aqui se agrega el cliente al juego
 */
class ThreadDispatcher: public Thread 
{
public:
  	// constructor
    ThreadDispatcher(const string & host, const port_type & puerto, const int & MaxClients, ThreadGame & game,const int & protocolVersion);
    // devuelve si esta en buen estado
    operator bool() const;
    // sobrecarga el metodo para detener el hilo
    virtual void stop();

protected:
    typedef enum tState{OK,ERROR_SOCKET,ERROR_THREAD,ERROR_EXECUTION};
    tState _state;
    
    virtual void main();
private:
    static const string LOCALHOST;
    SocketProtocol _listener;
    SocketProtocol _client;//cliente para poder cerrar el hilo
    ThreadGame& _thgame;
};


