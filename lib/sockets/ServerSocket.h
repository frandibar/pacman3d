#ifndef SERVERSOCKET_H_INCLUDED
#define SERVERSOCKET_H_INCLUDED

#include "Socket.h"
#include <string>

class ServerSocket : private Socket
{
public:
    ServerSocket() {};
    ServerSocket(int port);
    ServerSocket(const ServerSocket& rhs) : Socket(rhs) {}   // copy constructor
    virtual ~ServerSocket() {}

    const ServerSocket& operator<<(const std::string& buf) const;
    const ServerSocket& operator>>(std::string& buf) const;

    void accept(ServerSocket*);
    bool isAlive() { 
        std::string name;
        return (Socket::getPeerName(name) == 0);
    }
    std::string getPeerName() { 
        std::string name;
        Socket::getPeerName(name);
        return name;
    }
};


#endif
