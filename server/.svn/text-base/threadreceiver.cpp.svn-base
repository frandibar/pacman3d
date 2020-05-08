#include "threadreceiver.h"

const int ThreadReceiver::DELAY_TIME = 100;

ThreadReceiver::ThreadReceiver(SocketProtocol *connection,const int &playerId,const int &protocolVersion,ThreadGame &thGame):_packetmanager(protocolVersion), _thGame(thGame) {
  _state = OK;
  _connection = connection;
  _playerId = playerId;
  _protocolVersion = protocolVersion;
  if (!*_connection)
    _state = ERROR_SOCKET;
  if (!Thread::operator bool())
    _state = ERROR_THREAD;
}

ThreadReceiver::~ThreadReceiver() {
  while (!_packets.empty())
    _packets.pop();
  while (!_packetsPending.empty())
    _packetsPending.pop();
}

ThreadReceiver::operator bool() const {
  return ((_state == OK) && (*_connection));
}

void ThreadReceiver::main() {
  if (!*this)
    return;
  bool inactive;
  bool read = true;
  while (*this) {
    inactive = true;
    //se fija de leer un paquete
    if (_connection->select(read,0)) {
      if (!*_connection) {
        _state = ERROR_SOCKET;
        return;
      }
      inactive = false;
      //hay datos a leer
      Packet packet;
      //recibe paquete
      _connection->rcv(packet);
      if (!*_connection) {
        _state = ERROR_SOCKET;
        return;
      }
      //se fija que el protocolo sea "aceptable"
      if (packet.getHeader().getVersion() > _protocolVersion)
        continue;//evita paquetes con version mayor a la estipulada
      //se fija que el paquete sea KEY O MESSAGE
      switch (packet.getHeader().getType()) {
        case KEY: {
          tKeyType key;
          _packetmanager.parseKey(packet,key);
          _thGame.pressedKey(_playerId,key);
          break;}
        case MESSAGE: {
          string msg("");
          _packetmanager.parseMessage(packet,msg);
          _thGame.sendMessage(_playerId,msg);
          break;}
        case CONNECT: {
          string name("");
          _packetmanager.parseConnect(packet,name);
          _thGame.addName(_playerId,name,_connection->getPeerName());
          break;}
        default: break;
      }
    }
    if (inactive)
      Sleep(DELAY_TIME);
  }
}
