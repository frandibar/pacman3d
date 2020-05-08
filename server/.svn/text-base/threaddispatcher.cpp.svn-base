#include "threaddispatcher.h"
 
const string ThreadDispatcher::LOCALHOST = "127.0.0.1";

ThreadDispatcher::ThreadDispatcher(const string & host, const port_type & puerto, const int & MaxClients, ThreadGame & thgame, const int & protocolVersion): _listener (host, puerto, MaxClients),_client(LOCALHOST,puerto), _thgame(thgame) {
  _state = OK;
  if (!_listener)
    _state = ERROR_SOCKET;
  if (!Thread::operator bool())
    _state = ERROR_THREAD;
}

ThreadDispatcher::operator bool() const {
  return (_state == OK);
}

void ThreadDispatcher::stop() {
  Thread::stop();
  _client.connect();
}

void ThreadDispatcher::main() {
  if (!*this)
    return;
  //atiende pedidos de clientes
  while ((!_mustStop) && (*this)) {
    // se acepta a un nuevo cliente
    SocketProtocol *new_connection = _listener.accept();
    if (_mustStop) {
      if (new_connection)
        delete new_connection;
      return;
    }
    //verifica estado de conexion
    if (!new_connection) 
      continue;
    if (!*new_connection) {
      delete new_connection;
      continue;
    }
    //agrego jugador
    _thgame.addPlayer(new_connection);
  }  
}
