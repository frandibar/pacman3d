#pragma once

#include "packetheader.h"

// A Packet is transfered between the client and server
// It consists of a header and a body.

class Packet 
{
public:
    Packet() : _body(0)
    {}

    Packet(const unsigned char& version, const PacketHeader::tPacketType& type, const unsigned char& aux, unsigned char* body, int bodylen);
    ~Packet();

    Packet(const Packet &packet);
    Packet& operator=(const Packet& packet);
    
    PacketHeader getHeader() const {return _header;}
    void setHeader(const PacketHeader &header);

    unsigned char* getBody() const {return _body;}
    void setBody(unsigned char* body, int bodylen);
    int getBodyLength() const {return _bodylen;}

private:
    PacketHeader _header;
    unsigned char* _body;
    int _bodylen;
    
};

