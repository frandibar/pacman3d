#pragma once

#include "packet.h"
#include "lib/level.h"
#include "lib/element.h"
#include "lib/utilities.h"

/**Clase para crear y parsear paquetes segun sea el tipo
 * 
 * @sa Packet
 * */
class PacketManager 
{
public:
  	///constructor con numero de version de protocolo
    PacketManager(const short int &version):_version(version){};

    ///devuelve version de protocolo
    short int getProtocolVersion() const {return _version;}
    
	  ///crea un paquete INIT
    void createInit(Packet &packet,const bool &pacman,const Level &level);
    
    ///crea un paquete INIT segun otro paquete init, para acelerar la creacion para muchos clientes
    void createInit(Packet &packet,const bool &pacman,unsigned char* body,int bodylen);
    
    ///crea un paquete START
    void createStart(Packet &packet,const short int &idClient);
    
    ///crea un paquete STATUS
    void createStatus(Packet &packet,const int &score,const PlayerStatusList &players,const ElementList &elements,const NameList &namesChanged);
    
    ///crea un paquete STOP
    void createStop(Packet &packet,const tStopReason &stopReason,const int &score,const tStats &stats);
    
    ///crea un paquete QUIT
    void createQuit(Packet &packet);
    
    ///crea un paquete KEY
    void createKey(Packet &packet,const tKeyType &key);
    
    ///crea un paquete MESSAGE
    void createMessage(Packet &packet,const string &msg);
    
    ///crea un paquete CONNECT
    void createConnect(Packet &packet,const string &name);

	  ///parsea un paquete INIT
    void parseInit(const Packet& init,bool &isPacman,int &width,int &height,EdgeSet &edges,ElementList &elements);
    
    ///parsea un paquete START
    void parseStart(const Packet& packet,int &id);
    
    ///parsea un paquete STATUS
    void parseStatus(const Packet& packet,int &score,PlayerStatusList &positionList,ElementList &elements,NameList &namesChanged);
    
    ///parsea un paquete STOP
    void parseStop(const Packet& packet,tStopReason &reason,int &score,tStats &stats);
    
    ///parsea un paquete QUIT
    void parseQuit(const Packet& packet);
    
    ///parsea un paquete KEY
    void parseKey(const Packet& packet,tKeyType &key);
    
    ///parsea un paquete MESSAGE
    void parseMessage(const Packet& packet,string &msg);
    
    ///parsea un paquete CONNECT
    void parseConnect(const Packet& packet,string &name);

private:
    short int _version;
    
    void parsePlayerNames(const string& buffer,NameList &namesChanged);
    void buildNames(const NameList &names,string &s);
    
};

