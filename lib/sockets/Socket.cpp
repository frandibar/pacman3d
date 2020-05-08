#include "Socket.h"
#include "debug.h"

#include <iostream>
#include <cerrno>
#include <string>
#include <fcntl.h>

using std::string;

Socket::Socket() : _sock(-1)
{
    memset(&_addr, 0, sizeof(_addr));
}

Socket::~Socket()
{
    if (isValid())
        ::close(_sock);
}

bool Socket::create()
{
    _sock = socket(AF_INET, SOCK_STREAM, 0);

    if (!isValid())
        return false;

    // TIME_WAIT
    int on = 1;
    if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on)) == -1)
        return false;

    return true;
}

bool Socket::bind(int port)
{
    if (!isValid())
        return false;

    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = INADDR_ANY;
    _addr.sin_port = htons(port);

    return (::bind(_sock, (sockaddr*)&_addr, sizeof(_addr)) != -1);
}

bool Socket::listen() const
{
    if (!isValid())
        return false;

    return (::listen(_sock, SOMAXCONN) != -1);
}

bool Socket::accept(Socket* a_pSocket) const
{
    int addr_length = sizeof(_addr);
    a_pSocket->_sock = ::accept(_sock, (sockaddr*)&_addr, (socklen_t*)&addr_length);
    DEBUGME<int>(a_pSocket->_sock, "accepted socket: ");

    return (a_pSocket->_sock > 0);
}

bool Socket::send(const std::string& msg) const
{
    int status = ::send(_sock, msg.c_str(), msg.size(), MSG_NOSIGNAL);
    return (status != -1);
}

int Socket::receive(std::string& msg) const
{
    char buf[MAXRECV + 1];
    msg = "";
    memset(buf, 0, MAXRECV + 1);

    int status = recv(_sock, buf, MAXRECV, 0);

    if (status == 0)
        return 0;
    else if (status == -1) {
        std::cerr << "status: -1   errno: " << errno << "  in Socket::receive\n";
        return 0;
    }
    else {
        msg = buf;
        return status;
    }
}

bool Socket::connect(const std::string& host, int port)
{
    if (!isValid()) return false;

    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);

    int status = inet_pton(AF_INET, host.c_str(), &_addr.sin_addr);

    if (errno == EAFNOSUPPORT) return false;

    status = ::connect(_sock, (sockaddr*)&_addr, sizeof(_addr));
    return (status == 0);
}

void Socket::setNonBlocking(bool set)
{
    int opts = fcntl(_sock, F_GETFL);

    if (opts < 0)
        return;

    if (set)
        opts = (opts | O_NONBLOCK);
    else
        opts = (opts & ~O_NONBLOCK);

    fcntl(_sock, F_SETFL,opts);
}

bool Socket::getPeerName(string& name)
{
    struct sockaddr sa;
    socklen_t addlen;
    bool ret = (::getpeername(_sock, &sa, &addlen) == 0);
    name = string(sa.sa_data).substr(0, addlen);
    return ret;
}
