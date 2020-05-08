#ifndef __THREADDISTRIBUTION_H__
#define __THREADDISTRIBUTION_H__
#include "lib/thread.h"
#include "lib/mutex.h"
#include "threadtransmitter.h"
#include <list>
#include <queue>
using std::queue;
using std::list;

/**Clase para distribuir paquetes de forma masiva
 * En este hilo se encolan paquetes a enviar a todos los clientes
 */
class ThreadDistribution:public Thread {
  private:
    static const int DELAY_TIME;
    class SuscriptorList: public list<ThreadTransmitter*>{};
    typedef queue<Packet> PacketQueue;

    PacketQueue _packets;
    PacketQueue _packetsPending;
    Mutex _mtxPacket;
    SuscriptorList _clients;
    Mutex _mtxClients;
    SuscriptorList _clientsPending;
    Mutex _mtxClientPending;
  protected:
    typedef enum tState {OK,ERROR_SOCKET,ERROR_THREAD,ERROR_EXECUTION};
    tState _state;
    
    virtual void main();
    
  public:
  	/// constuctor
    ThreadDistribution();
    /// destructor
    ~ThreadDistribution();
    /// devuelve si esta en buen estado
    operator bool() const;
    /// envio de paquete a todos los clientes
    void broadcast(const Packet &packet);
    /// suscribe jugador al sistema de distribucion
    void suscribe(ThreadTransmitter *thTransmitter);
    /// elimina jugador (por desconeccion del cliente)
    void unsuscribe(int transmitterId);
};

#endif /*__THREADDISTRIBUTION_H__*/
