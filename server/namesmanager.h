#ifndef __NAMESMANAGER_H__
#define __NAMESMANAGER_H__
#include "lib/mutex.h"
#include "lib/utilities.h"
#include <string>
using std::string;

/**
 * Clase para administrar los nombres y numero IP de los jugadores conectados
 * Metodos protegidos por mutex
 */
class NamesManager {
  private:
    Mutex _mtx;
    NameList _nameList;
    NameList _namesUpdate;
    NameMap _nameMap;
  public:
    ///destructor
    ~NamesManager();
    ///agrega un nuevo nombre cliente
    void addName(int playerId, const string &name, const string &IP);
    /**devuelve una copia de la lista de nombres
      * SE DEBE HACER DELETE DE LA LISTA
    */
    NameList* getNameList();
    ///elimina nombre de jugador
    void removeName(unsigned int playerId);
    /**devuelve una lista de los nombres recientemente agregados
      DEBE HACERSE DELETE
    */
    NameList* getUpdates();
    ///devuelve el nombre de un jugador
    string findName (int playerId);
    ///devuelve el IP de un jugador
    string findIP (int playerId);
};
#endif /* __NAMESMANAGER_H__ */
