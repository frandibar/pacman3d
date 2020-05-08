#ifndef __THREADRECEIVER_H__
#define __THREADRECEIVER_H__
#include "lib/thread.h"
#include "lib/socketprotocol.h"
#include "lib/packetmanager.h"
#include "threadgame.h"
#include <queue>
using std::queue;

class ThreadGame;

/** Clase que maneja el hilo para recibir paquetes de un cliente
 * Puede recibvir paquetes CONNECT, KEY o MESSAGE
 */
class ThreadReceiver:public Thread {
  private:
    typedef queue<Packet> PacketQueue;
    
    static const int DELAY_TIME;
    SocketProtocol *_connection;
    int _playerId;
    PacketQueue _packets;
    PacketQueue _packetsPending;
    PacketManager _packetmanager;
    int _protocolVersion;
    ThreadGame &_thGame;
    
    //los ocultamos por seguridad
    ThreadReceiver (const ThreadReceiver&);
    ThreadReceiver& operator=(const ThreadReceiver&);
  protected:
    typedef enum tState{OK,ERROR_SOCKET,ERROR_THREAD};
    tState _state;
    
    virtual void main();
    
  public:
  	/// constructor
    ThreadReceiver(SocketProtocol *connection,const int &playerId,const int &protocolVersion,ThreadGame &thGame);
    /// destructor
    ~ThreadReceiver();
    /// devuelve si esta en buen estado
    virtual operator bool() const;
    /// devuelve el id del jugador para el cual esta escuchando
    int getPlayerId() const {return _playerId;}
};

#endif /*__THREADRECEIVER_H__*/
