#include "threadtransmitter.h"

const int ThreadTransmitter::DELAY_TIME = 100;

ThreadTransmitter::ThreadTransmitter(SocketProtocol *connection,const int &playerId,const bool &pacman) {
  _state = OK;
  _connection = connection;
  _playerId = playerId;
  _pacman = pacman;
  if (!*_connection)
    _state = ERROR_SOCKET;
  if (!Thread::operator bool())
    _state = ERROR_THREAD;
}

ThreadTransmitter::~ThreadTransmitter() {
  delete _connection;
  _mtx.lock();
  while (!_packets.empty())
    _packets.pop();
  while (!_packetsPending.empty())
    _packetsPending.pop();
  _mtx.unlock();
}

ThreadTransmitter::operator bool() const {
  return ((_state == OK) && (*_connection));
}

//encola paquete a enviar
void ThreadTransmitter::sendPacket(const Packet &packet) {
  _mtx.lock();
  _packetsPending.push(packet);
  _mtx.unlock();
}

//envia paquete por socket
int ThreadTransmitter::send(const Packet &packet){
  if (!_connection) {
    _state = ERROR_SOCKET;
    return -1;
  }
  return _connection->send(packet);
}

void ThreadTransmitter::main() {
  if (!*this)
    return;
  bool inactive;
  while (*this) {
    inactive = true;
    //se fija si hay mensajes pendientes y los pasa a la cola para enviar
    _mtx.lock();
    while (!_packetsPending.empty()) {
      _packets.push(_packetsPending.front());
      _packetsPending.pop();
    }
    _mtx.unlock();
    
    //envia todos los packets
    while (!_packets.empty()) {
      inactive = false;
      if (send(_packets.front()) < 0) {
        _state = ERROR_SOCKET;
        return;
      }
      _packets.pop();
    }
    
    if ((_mustStop) && (_packets.empty()) && (_packetsPending.empty()))
      break; //cierra cuando termina de mandar todo
    if (inactive)
      Sleep(DELAY_TIME);
  }
}
