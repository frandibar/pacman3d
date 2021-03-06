#ifndef __THREADTRANSMITTER_H__
#define __THREADTRANSMITTER_H__
#include "lib/thread.h"
#include "lib/mutex.h"
#include "lib/socketprotocol.h"
#include "lib/packet.h"
#include <queue>
using std::queue;

/**Clase que emite paquetes al cliente
 */
class ThreadTransmitter : public Thread {
	
  private:
    typedef queue<Packet> PacketQueue;
    
    static const int DELAY_TIME;
    SocketProtocol *_connection;
    int _playerId;
    bool _pacman;//define si es pacman o no
    PacketQueue _packets;
    PacketQueue _packetsPending;
    Mutex _mtx;
    
    //envia mensaje por socket
    int send(const Packet &packet);
    
    //los ocultamos por seguridad
    ThreadTransmitter (const ThreadTransmitter&);
    ThreadTransmitter& operator=(const ThreadTransmitter&);
    
  protected:
    typedef enum tState{OK,ERROR_SOCKET,ERROR_THREAD};
    tState _state;
    
    virtual void main();
    
  public:
  	/// constructor
    ThreadTransmitter(SocketProtocol *connection,const int &playerId,const bool &pacman);
    /// destructor
    ~ThreadTransmitter();
    /// devuelve si esta en buen estado
    virtual operator bool() const;
    ///envia paquete al cliente, encolandolo
    void sendPacket(const Packet &packet);
    /// devuelve el id del jugador al que le envia paquetes
    int getPlayerId() const {return _playerId;}
    /// devuelve si es pacman
    bool isPacman() const {return _pacman;}
    ///devuelve IP de host del socket
    string getHostName() const {return _connection->getPeerName();}
};

#endif /*__THREADTRANSMITTER_H__*/
