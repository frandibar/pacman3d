#pragma once
#include <string>
using std::string;

// TODO: usar boost tokenizer

/**Clase para parsear strings con campos separados con un delimitador
 */
class StringToken {
  private:
    //ultimo string con que se invoco
    string _lastBuffer;
  public:
  	///constructor
    StringToken():_lastBuffer(""){};
    ///recive buffer y delimitadores, devuelve el token y un bool si se pudo sacar algo
    bool extractToken(const string &buffer,const string &delims,string &token);
    ///recive buffer y delimitadores, devuelve el token de ultimo buffer ingresado y un bool si se pudo sacar algo
    bool extractToken(const string &delims,string &token);
};
