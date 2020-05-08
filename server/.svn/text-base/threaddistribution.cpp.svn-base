#include "threaddistribution.h"

const int ThreadDistribution::DELAY_TIME = 25;

ThreadDistribution::ThreadDistribution() {
  _state = OK;
  if (!Thread::operator bool())
    _state = ERROR_THREAD;
}

ThreadDistribution::~ThreadDistribution() {
  //borra clientes
  _clients.clear();
  _clientsPending.clear();
  while (!_packets.empty())
    _packets.pop();
  while (!_packetsPending.empty())
    _packetsPending.pop();
}

ThreadDistribution::operator bool() const {
  return (_state == OK);
}

void ThreadDistribution::suscribe(ThreadTransmitter *thTransmitter) {
  //agrega cliente
  _mtxClientPending.lock();
  _clientsPending.push_back(thTransmitter);
  _mtxClientPending.unlock();
}

void ThreadDistribution::unsuscribe(int transmitterId) {
  //agrega cliente
  _mtxClients.lock();
  SuscriptorList::iterator it = _clients.begin();
  while (it != _clients.end()) {
    if ((*it)->getPlayerId() == transmitterId)
      break;
    it++;
  }
  if (it != _clients.end())
    _clients.erase(it);
  _mtxClients.unlock();
}

//Agrega mensaje a enviar a clientes
void ThreadDistribution::broadcast(const Packet &packet) {
  _mtxPacket.lock();
  _packetsPending.push(packet);
  _mtxPacket.unlock();
}

void ThreadDistribution::main() {
  if (!*this)
    return;
  bool busy;
  while (*this) {
    busy = false;
    //inserta clientes a dar de alta
    _mtxClientPending.lock();
    for (SuscriptorList::iterator it = _clientsPending.begin(); it != _clientsPending.end();++it)
      _clients.push_back(*it);
    _clientsPending.clear();
    _mtxClientPending.unlock();
    
    //agrega mensajes dados de alta
    _mtxPacket.lock();
    while (!_packetsPending.empty()) {
      _packets.push(_packetsPending.front());
      _packetsPending.pop();
    }
    _mtxPacket.unlock();
    
    //distribuye el primer mensaje a todos los suscriptores
    if (!_packets.empty()) {
      busy = true;
      _mtxClients.lock();
      SuscriptorList::iterator iter = _clients.begin();
      while (iter != _clients.end()) {
        (*iter)->sendPacket(_packets.front());
        SuscriptorList::iterator iterAnt = iter;
        iter++;
        if (!**iterAnt) {
          //cliente en mal estado => lo saca
          _clients.erase(iterAnt);
        }
      }
      _mtxClients.unlock();
      _packets.pop();
    }
    
    if ((_mustStop) && (_packets.empty()) && (_packetsPending.empty()))
      break; //cierra cuando termina de mandar todo
    if (!busy)
      Sleep(DELAY_TIME);
  }
}
