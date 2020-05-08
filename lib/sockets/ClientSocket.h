#ifndef CLIENTSOCKET_H_INCLUDED
#define CLIENTSOCKET_H_INCLUDED

#include "Socket.h"

class ClientSocket : private Socket
{
public:

     ClientSocket (std::string host, int port);
     virtual ~ClientSocket() {};

     const ClientSocket& operator<<(const std::string& buf) const;
     const ClientSocket& operator>>(std::string& buf) const;
};

#endif
