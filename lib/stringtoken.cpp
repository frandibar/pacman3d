#include "common/stringtoken.h"

//recive buffer y delimitadores, devuelve el token y un bool si se pudo sacar algo
bool StringToken::extractToken(const string &buffer,const string &delims,string &token) {
  _lastBuffer.assign(buffer);
  return extractToken(delims,token);
}

//recive buffer y delimitadores, devuelve el token de ultimo buffer ingresado y un bool si se pudo sacar algo
bool StringToken::extractToken(const string &delims,string &token) {
  token.assign("");
  if (_lastBuffer.length() == 0)
    return false;
  unsigned int i;
  for(i = 0; i < _lastBuffer.length(); i++) {
    if (delims.find_first_of(_lastBuffer[i]) != string::npos)
      break; //se encontro un delimitador
    else //actualiza token
      token.append(1,_lastBuffer[i]);
  }
  _lastBuffer.erase(0,i+1);
  return true;
}
