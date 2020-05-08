#include "namesmanager.h"

NamesManager::~NamesManager() {
  _mtx.lock();
  _nameList.clear();
  _nameMap.clear();
  _namesUpdate.clear();
  _mtx.unlock();
}

void NamesManager::addName(int playerId, const string &name, const string &IP) {
  tPlayerName playerName;
  playerName.id = playerId;
  playerName.name.assign(name);
  tName tname;
  tname.name.assign(name);
  tname.IP.assign(IP);
  _mtx.lock();
  //listas
  _nameList.push_back(playerName);
  _namesUpdate.push_back(playerName);
  //mapa
  _nameMap[playerId] = tname;
  _mtx.unlock();
}

NameList* NamesManager::getNameList() {
  NameList *list = new NameList;
  _mtx.lock();
  for(NameList::iterator it = _nameList.begin(); it != _nameList.end();it++)
    list->push_back(*it);
  _mtx.unlock();
  return list;
}

void NamesManager::removeName(unsigned int playerId) {
  _mtx.lock();
  _nameMap.erase(playerId);
  for(NameList::iterator it = _nameList.begin(); it != _nameList.end();++it)
    if (it->id == playerId) {
      _nameList.erase(it);
      break;
    }
  for(NameList::iterator it = _namesUpdate.begin(); it != _namesUpdate.end();++it)
    if (it->id == playerId) {
      _namesUpdate.erase(it);
      break;
    }
  _mtx.unlock();
}

NameList* NamesManager::getUpdates() {
  NameList *list = new NameList;
  _mtx.lock();
  for(NameList::iterator it = _namesUpdate.begin(); it != _namesUpdate.end();++it)
    list->push_back(*it);
  _namesUpdate.clear();
  _mtx.unlock();
  return list;
}

string NamesManager::findName(int playerId) {
  string name("");
  _mtx.lock();
  NameMap::iterator it = _nameMap.find(playerId);
  if (it != _nameMap.end())
    name.assign(it->second.name);
  _mtx.unlock();
  return name;
}

string NamesManager::findIP(int playerId) {
  string IP("");
  _mtx.lock();
  NameMap::iterator it = _nameMap.find(playerId);
  if (it != _nameMap.end())
    IP.assign(it->second.IP);
  _mtx.unlock();
  return IP;
}
