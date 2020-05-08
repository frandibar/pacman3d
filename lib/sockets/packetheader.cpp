#include "packetheader.h"

PacketHeader::PacketHeader(const unsigned char& version, const tPacketType& type, const unsigned char& aux) 
{
    _header = 0;
    _version = version;
    _type = type;
    _aux = aux;
    _header = _header | (version << 6);
    _header = _header | (type << 3);
    _header = _header | (aux);
}

PacketHeader::PacketHeader(const unsigned char &header) 
{
    _header = header;
    // parse version bits 6-7
    _version = (header & (3 << 6)) >> 6;
    // parse type bits 3-5
    _type = (tPacketType)((header & (7 << 3)) >> 3);
    // parse aux bits 0-2
    _aux = header & 7;
}
