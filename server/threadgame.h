/**
 * @file threadgame.h
 */
#ifndef __THREADGAME_H__
#define __THREADGAME_H__
#include "lib/thread.h"
#include "threaddistribution.h"
#include "lib/packetmanager.h"
#include "log.h"
#include "lib/level.h"
#include "lib/world.h"
#include "lib/utilities.h"
#include "playerhandler.h"
#include "scoremanager.h"
#include "bonusmanager.h"
#include "powerupmanager.h"
#include "elementmanager.h"
#include "namesmanager.h"
#include "clientmanager.h"
#include "statisticsmanager.h"
#include <map>
#include <vector>
using std::map;
using std::vector;

/**
 * Parametros de configuracion del juego
 */
typedef struct tGameConfig {
  int min; ///< minima cantidad de jugadores
  int max; ///< maxima cantidad de jugadores
  bool ghostComm; ///< permite GhostCommunication (si un Ghost ve al pacman, loven todos)
  int lives; ///< cantidad de vidas para el pacman
  string logFile; ///< nombre y ruta del log File (vacio es por cout)
  int waitingTime; ///< tiempo de espera hta comenzar el juego 
  int protocolVersion; ///< version de protocolo
}tGameConfig;

class PacketManager;
class PlayerHandler;
class ThreadDistribution;
class ClientManager;

/**Clase que representa la ejecucion de un solo juego en el server
 * Engloba el agregado de clientes, cumplimiento de minimo y maximo de jugadores, verificar estado de clientes y enviar los paquetes segun el protocolo
 */
class ThreadGame:public Thread {
  private:
    static const int WAIT_FIRST_PLAYER;
    //manejo del juego
    int _protocolVersion;
    const tGameConfig _config;
    ScoreManager _scoreManager;
    BonusManager _bonusManager;
    PowerUpManager _powerupManager;
    ElementManager _elementManager;
    NamesManager _namesManager;
    PlayerHandler *_playerHandler;
    StatisticsManager _statsManager;
    //hilos
    ThreadDistribution *_thdistrib;
    ClientManager *_clientManager;
    //paquetes
    PacketManager *_packetmanager;
    //estado del juego
    bool _started;
    bool _playing;
    bool _ended; 
    Log _log;
    World &_world;
    Level *_level;//nivel actual
    
    //filtra jugadores en buen estado
    void checkPlayersStatus(bool &pacmanDisconnected);
    void sendInit();
    void sendStart();
    void sendStatus(bool &pacmanDead);
    void sendStop(const tStopReason &reason);
    void sendQuit();
    bool runable() const;
    //chequea de que se cumpla con el minimo de jugadores
    bool checkMinPlayers();
    void logStartGame();
    void logEndGame();
    void waitFirstPlayer();
    bool playLevel();
    
    //los ocultamos por seguridad
    ThreadGame (const ThreadGame&);
    ThreadGame& operator=(const ThreadGame&);
    
  protected:
    typedef enum tState{OK,ERROR_SOCKET,ERROR_THREAD,ERROR_EXECUTION};
    tState _state;
    virtual void main();
    
  public:
  	/// constructor
    ThreadGame(World &world,const tGameConfig &gameConfig, int protocolVersion);
    /// destructor
    virtual ~ThreadGame();
    /// intenta agregar un jugador al juego
    void addPlayer(SocketProtocol *new_connection);
    /// devuelve si esta en buen estado
    operator bool() const;
    /// realiza movimiento de jugadores segun tecla apretada
    void pressedKey(const int &playerId,const tKeyType &key);
    /// envia mensaje de chat
    void sendMessage(const int &playerId,const string &msg);
    ///devuelve si termino el juego
    bool getEnded() const {return _ended;}
    /// agrega nombre e IP de jugador
    void addName(const int &playerId,const string &playerName,const string &IP);
    /// pacman fue comido
    void eatenPacman(int ghostId);
    /// fantasma fue comido por pacman
    void eatenGhost(int ghostId);
    /// elemento comido por pacman
    void eatenElement(const Element &elem);
    /// fantasma retorno a la casa de fantasmas para recobrar la vida
    void revivedGhost(int ghostId);
    /// metodo para cerrar el server 
    void shutDown();
};

#endif /*__THREADGAME_H__*/
