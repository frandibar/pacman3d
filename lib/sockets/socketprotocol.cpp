#include "socketprotocol.h"
#include <string.h>     // memcpy

int SocketProtocol::send(const Packet &packet)
{
    //extrae header de paquete
    unsigned char *header = new unsigned char[1];
    header[0] = packet.getHeader().getBinaryHeader();
    Socket::send(header,1);
    delete [] header;

    //envia longitud de cuerpo
    short int len = packet.getBodyLength();
    unsigned char *bodylen = new unsigned char[2];
    memcpy(bodylen,&len,2);
    Socket::send(bodylen,2);
    delete [] bodylen;

    //envia cuerpo
    if (len == 0)
        return 0;

    return Socket::send(packet.getBody(), packet.getBodyLength());
}

int SocketProtocol::rcv(Packet &packet)
{
    unsigned char *aux = new unsigned char[1];
    //recibe header
    Socket::rcv(aux, PacketHeader::getHeaderSize());
    if (!*this) {
        delete[] aux;
        return -1;
    }
    //inserta header en paquete
    PacketHeader header(aux[0]);
    delete[] aux;
    packet.setHeader(header);
    //recibe longitud de body
    aux = new unsigned char[2];
    Socket::rcv(aux,2);
    if (!*this)  {
        delete[] aux;
        return -1;
    }
    short int len = 0;
    memcpy(&len,aux,2);
    delete[] aux;
    //recibe cuerpo
    if (len == 0)
        return 0;
    aux = new unsigned char[len];
    int ret = Socket::rcv(aux,len);
    if (!*this)  {
        delete[] aux;
        return -1;
    }
    //asigna body a cuerpo
    packet.setBody(aux,len);
    delete[] aux;
    return ret;
}

//acepta una nueva conexion
SocketProtocol* SocketProtocol::accept()
{
    return (SocketProtocol*)Socket::accept();
}

SocketProtocol::operator bool () const
{
    return Socket::operator bool();
}

void SocketProtocol::connect()
{
    Socket::connect();
}

void SocketProtocol::close()
{
    Socket::close();
}

//devuelve host del cliente
std::string SocketProtocol::getPeerName() 
{
    return Socket::getPeerName();
}

//devuelve host local
std::string SocketProtocol::getSocketName()
{
    return Socket::getSocketName();
}

//se fija si hay datos q leer/escribir
bool SocketProtocol::select(const bool &read, const int &mseg) 
{
    return Socket::select(read,mseg);
}
