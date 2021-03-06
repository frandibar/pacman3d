#pragma once

#include <vector>

class Vertex;

/**
 * Esta clase representa las aristas no dirigidas. Esta compuesta por un id y por sus dos vértices a los que une. El vertice0 es el vértice que nace en esa arista, o sea, el que se encuentra en la posición relativa 0 de la arista. El vertice1 es el entrante de la arista, y es un vértice que se encontrará en la ultima posición de la misma (pos 63), por lo que también serí­a posición 0 de otra arista.
 * Esta clase solamente contiene setters y getters de sus atributos.
 */
class Edge 
{
    ///orientacion de los arcos
    typedef enum tEdgeOrientation{ VERTICAL = 0, HORIZONTAL = 1};

public:
  	/// vertice0 sera la posicion 0 de la arista, y vertice1 sera la ultima posicion de la misma
  	Edge( Vertex* vertice0, Vertex* vertice1, int ID, tEdgeOrientation orientation);
    /// devuelve id
    inline int getID() const {return id;}
    /// devuelve vertice naciente, posicion 0
    inline Vertex* getVertex0() const{
    	return vertex0;	
    }
    /// devuelve vertice entrante, ultima posicion
	inline Vertex* getVertex1() const{
		return vertex1;
	}
	/// devuelve el otro vertice al pasado
	inline Vertex* getOther(Vertex* vertex) const{
		return ( (vertex == vertex0)? (vertex1) : ( (vertex == vertex1)? (vertex0) : (NULL) ) )	;
	}
	/// devuelve la orientacion de la arista
	inline tEdgeOrientation getOrientation() const{
		return orientation;
	}
    
private:
    int id;//id de arco
    Vertex* vertex0;
    Vertex* vertex1;
    tEdgeOrientation orientation;
    
    //los ocultamos por seguridad
    Edge (const Edge&);
    Edge& operator=(const Edge&);
    
};

/// vector de aristas
typedef std::vector<Edge*>  EdgeList;

