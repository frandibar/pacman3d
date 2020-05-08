#ifndef SOCKET_H_INCLUDED
#define SOCKET_H_INCLUDED

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>

const int MAXRECV = 500;  // max length of buffer to receive

// socket wrapper class
class Socket
{
public:
    Socket();
    Socket(const Socket& s) : _sock(s._sock), _addr(s._addr) {}  // copy constructor
    virtual ~Socket();

    // server initialization
    bool create();
    bool bind(int port);
    bool listen() const;
    bool accept(Socket*) const;

    // client initialization
    bool connect(const std::string& host, int port);

    // data Transmission
    bool send(const std::string& buf) const;
    int receive(std::string& buf) const;

    void setNonBlocking(bool);

    bool isValid() const { return _sock != -1; }
    bool getPeerName(std::string& name);

private:
    int _sock;
    sockaddr_in _addr;
};

#endif
