
#include "ServerSocket.h"
#include "SocketException.h"


ServerSocket::ServerSocket(int port)
{
    if (!Socket::create())
        throw SocketException("Could not create server socket.");

    if (!Socket::bind(port))
        throw SocketException("Could not bind to port.");

    if (!Socket::listen())
        throw SocketException("Could not listen to socket.");
}


const ServerSocket& ServerSocket::operator<<(const std::string& buf) const
{
    if (!Socket::send(buf))
        throw SocketException("Could not write to socket.");

    return *this;
}


const ServerSocket& ServerSocket::operator>>(std::string& buf) const
{
    if (!Socket::receive(buf))
        throw SocketException("Could not read from socket.");

    return *this;
}

void ServerSocket::accept(ServerSocket* a_pSocket)
{
    if (!Socket::accept(a_pSocket))
        throw SocketException("Could not accept socket.");
}
