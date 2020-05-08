#pragma once

#include <string>
#include <sstream>

/// toma instancia de cualquier tipo y lo convierte a string
template <typename T>
std::string StringConverter(const T &value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

/// convierte de string a T
template<typename T> 
T fromString(const std::string &s)
{
    std::istringstream is(s);
    T r;
    is >> r;
    return r;
}

/// Determina si el contenido del string es numerico o no
bool stringIsNumeric(const std::string &s);

/**
 * fileExists()
 * Determina si el archivo recibido como parametro existe.
 * (el parametro debe ser la ruta completa del archivo a validar)
 */
bool fileExists( const std::string & s );

///devuelve true si la cadena source termina con la cadena patron pasada. De lo contrario devuelve false.
bool endsIn(const std::string & source, const std::string & pattern);

