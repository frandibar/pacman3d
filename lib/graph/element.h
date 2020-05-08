#pragma once
#include "utilities.h"

#include <vector>

/**
 * Esta clase representa el elemento que puede contener un vértice. Es un conjunto de atributos, entre los cuales podemos destacar el id del vértice donde se encuentra, el tipo de elemento (puede ser pill, powerup, bonus, etc), la orientación en el mapa (por si es casa de fantasmas), el estado del elemento (comido o no). Todos sus métodos son setters y getters.
 */
class Element 
{
public:
  	/// Todo elemento es esferico, y este es su radio
    static const unsigned short ratio;
    /// constructor con parametros
    Element (const int &vertexId,const tElementType &type,const tOrientation &orientation):_vertexId(vertexId), _type(type), _orientation(orientation){}
 	/// setters y getters
    int getIdVertex() const {return _vertexId;}
    /// setters y getters
    tElementType getType() const {return _type;}
    /// setters y getters
    tOrientation getOrientation() const {return _orientation;}
    /// setters y getters
    tElementState getState() const {return _state;}
    /// setters y getters
    void setState(const tElementState &state) {_state = state;}

private:
    int _vertexId;
    tElementType _type;
    tOrientation _orientation;
    tElementState _state;
};

/// lista de elementos
typedef std::vector<Element> ElementList;

