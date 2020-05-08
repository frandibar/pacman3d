#pragma once

#include <string>

#include "socket.h"
#include "packet.h"


/**Clase que utiliza el socket pero agregando la capa de protocolo de transmision
 * 
 */
class SocketProtocol : private Socket 
{
public:
    // contruye un socket para cliente
    SocketProtocol(const std::string& host, const port_type &port) : Socket(host, port)
    {}
    // constructor de socket para server
    explicit SocketProtocol(const std::string & host, const port_type & port, const int & cant_clientes) : Socket(host, port, cant_clientes)
    {}
    
    int send(const Packet &packet);
    int rcv(Packet &packet);
    
    //acepta un nuevo cliente
    SocketProtocol* accept();
    
    //devuelve si esta en buen estado
    operator bool () const;
    
    void connect();
    void close();
    
    /// se fija si hay datos q leer/escribir
    bool select(const bool& read, const int& mseg);
    
    /// devuelve host del cliente
    std::string getPeerName();
    
    /// devuelve host local
    std::string getSocketName();
};

