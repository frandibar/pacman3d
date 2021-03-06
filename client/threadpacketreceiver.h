#ifndef __THREADPACKETRECEIVER_H__
#define __THREADPACKETRECEIVER_H__
#include "lib/thread.h"
#include "lib/packetmanager.h"
#include "lib/socketprotocol.h"
#include "graphicupdater.h"

/**Clase para utilizar el hilo para recibir paquetes del servidor
  Al recibir cada paquete, lo parsea y notifica a GraphicUpdater
*/
class ThreadPacketReceiver:public Thread {
  private:
    static const int DELAY_TIME;
    PacketManager _packetManager;
    int _protocolVersion;
    SocketProtocol &_connection;
    GraphicUpdater &_graphicUpdater;
    
    void initPacket(const Packet& packet);
    void startPacket(const Packet& packet);
    void statusPacket(const Packet& packet);
    void stopPacket(const Packet& packet);
    void quitPacket(const Packet& packet);
    void messagePacket(const Packet& packet);
  protected:
    typedef enum tPhase{WAITING,INITIALIZED,STARTED};
    typedef enum tState{OK,ERROR_SOCKET,ERROR_THREAD};
    tPhase _phase;
    tState _state;
    //lo q ejecuta el thread
    virtual void main();
  public:
    ///constructor
    ThreadPacketReceiver(const int &protocolVersion,SocketProtocol &connection,GraphicUpdater &graphicUpdater);
    ///devuelve si esta en buen estado
    virtual operator bool() const;
};
#endif
