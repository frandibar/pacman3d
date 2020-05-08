#pragma once

class PacketHeader 
{
public:
    enum tPacketType { INIT, START, STATUS, STOP, QUIT, KEY, MESSAGE, CONNECT };

    PacketHeader()
    {}

    PacketHeader(const unsigned char& version, const tPacketType& type, const unsigned char& aux);
    PacketHeader(const unsigned char& header);
    
    unsigned char getBinaryHeader() const {return _header;}
    static int getHeaderSize() {return sizeof(char);}
    
    unsigned char getVersion() const {return _version;}
    tPacketType getType() const {return _type;}
    
    ///devuelve bits auxiliares
    unsigned char getAux() const {return _aux;}

private:
    unsigned char _header;
    unsigned char _version;
    tPacketType _type;
    unsigned char _aux;
};
