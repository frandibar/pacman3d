#include "packet.h"
#include <string.h>     // memcpy

Packet::Packet(const unsigned char &version,const PacketHeader::tPacketType &type,const unsigned char &aux,unsigned char *body,int bodylen): _header(version,type,aux), _bodylen(bodylen) 
{
    if (body) {
        _body = new unsigned char[bodylen];
        memcpy(_body,body,bodylen);
    }
    else
        _body = 0;
}

// destructor
Packet::~Packet() 
{
    if (_body)
        delete[] _body;
}

Packet::Packet(const Packet &packet): _header(packet._header), _bodylen(packet._bodylen) 
{
    if (packet._body) {
        _body = new unsigned char[_bodylen];
        memcpy(_body,packet._body,_bodylen);
    }
    else 
        _body = 0;
}

Packet& Packet::operator=(const Packet& packet) 
{
    if (this != &packet) {
        if (_body) {
            delete[] _body;
            _body = 0;
        }
        _bodylen = packet._bodylen;
        _header = packet._header;
        if (packet._body) {
            _body = new unsigned char[_bodylen];
            memcpy(_body,packet._body,_bodylen);
        }
    }
    return *this;
}

void Packet::setHeader(const PacketHeader &header) 
{
    _header = header;
}

void Packet::setBody(unsigned char *body,int bodylen) 
{
    _bodylen = bodylen;
    if (_body)
        delete[] _body;
    _body = new unsigned char[bodylen];
    memcpy(_body,body,bodylen);
}
