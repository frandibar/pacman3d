#include "clientmanager.h"

ClientManager::ClientManager(ThreadDistribution &thDistrib): _thDistrib(thDistrib) {
  _number = 0;
}

int ClientManager::getNumberClients() {
  int ret = 0;
  _mtx.lock();
  ret = _number;
  _mtx.unlock();
  return ret;
}

void ClientManager::addClient(int playerId,ThreadTransmitter *thTransmitter,ThreadReceiver *thReceiver) {
  _mtx.lock();
  _number++;
  _transmitters[playerId] = thTransmitter;
  _receivers[playerId] = thReceiver;
  _mtx.unlock();
}

void ClientManager::deleteClients() {
  _mtx.lock();
  //elimina request
  for (TransmitterMap::iterator it = _transmitters.begin();it != _transmitters.end();it++)
    delete it->second;
  _transmitters.clear();
  //elimina detectores
  for (ReceiverMap::iterator it = _receivers.begin();it != _receivers.end();it++) {
    it->second->stop();
    it->second->join();
    delete it->second;
  }
  _receivers.clear();
  _mtx.unlock();
}

//filtra jugadores en buen estado
IdList ClientManager::checkPlayersStatus(bool &pacmanDisconnected) {
  pacmanDisconnected = false;
  IdList idlist;
  _mtx.lock();
  //se fija si hay requests con estado de error y los saca
  TransmitterMap::iterator iter = _transmitters.begin();
  while (iter != _transmitters.end()) {
    if (!*iter->second) {
      //cliente con error
      if (iter->second->isPacman())
        pacmanDisconnected = true;
      int id = iter->second->getPlayerId();
      idlist.push_back(id);
      //detiene detector
      ReceiverMap::iterator itReceiver = _receivers.find(id);
      if (itReceiver != _receivers.end()) {
        itReceiver->second->stop();
        itReceiver->second->join();
        delete itReceiver->second;
        _receivers.erase(itReceiver);
      }
      //detiene request
      TransmitterMap::iterator iterAnt = iter;
      iter++;
      _thDistrib.unsuscribe(iterAnt->second->getPlayerId());
      iterAnt->second->stop();
      iterAnt->second->join();
      delete iterAnt->second;
      _transmitters.erase(iterAnt);
    }
    else
      iter++;
  }
  _mtx.unlock();
  return idlist;
}

TransmitterMap* ClientManager::getRequests() {
  TransmitterMap* requests;
  _mtx.lock();
  requests = new TransmitterMap(_transmitters);
  _mtx.unlock();
  return requests;
}
