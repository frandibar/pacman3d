/**
 * @file vertex.h 
 * Esta clase representa los vértices dentro del grafo. Esta compuesto por un vértice del tipo Vertice (template utilizado en el TP3 y TP4), el cual contiene un contenido del tipo Elemento* y un id numérico entero.
 * Contiene 4 aristas del tipo Edge, una norte, una este, una sur y una oeste, que son los pasillos por los que puede recorrerse hasta otro vértice en el mapa. Por convención, las aristas norte y este son nacientes en ese vértice, lo que significa que ese vértice es posición 0 de sus aristas, tanto norte como este, y por lo tanto, también es posición 63 de las aristas entrantes a el, o sea, sus aristas sur y este.
 * Esta clase solamente tiene setters y getters de sus atributos.
 * */
#ifndef __VERTEX_H__
#define __VERTEX_H__

#include <list>
#include "common/Vertice.h"
#include "common/element.h"
#include "common/edge.h"
#include "common/utilities.h"


class Graph;

/**
 * POR CONVENCION, EL ORDEN EN EL VECTOR DE ARISTAS ES: Norte, Este, Sur, Oeste, correspondiente a 0, 1, 2, 3 respectivamente
 */
class Vertex {

  public:
	
	///
	typedef Vertice< Element, void, int, int > tVertice;
	
  private:
  	friend class Graph;
  	tVertice*	vertice;
  	EdgeList	edges;
  	
  	//los ocultamos por seguridad
  	Vertex (const Vertex&);
  	Vertex& operator=(const Vertex&);
    
  public:
  	///constructor
  	Vertex( tVertice * vertex);
  	///setters y getters de edges
  	inline void setNorthEdge( Edge* edge){
		edges[NORTH] = edge;		
	}
	///setters y getters de edges
  	inline void setEastEdge( Edge* edge){
		edges[EAST] = edge;	
	}
	///setters y getters de edges
  	inline void setSouthEdge( Edge* edge){
		edges[SOUTH] = edge;	
	}
	///setters y getters de edges
  	inline void setWestEdge( Edge* edge){
		edges[WEST] = edge;	
	}
	///setters y getters de edges
  	inline Edge* getNorthEdge() const{
		return edges[NORTH];	
	}
	///setters y getters de edges
  	inline Edge* getEastEdge() const{
		return edges[EAST];	
	}
	///setters y getters de edges
  	inline Edge* getSouthEdge() const{
		return edges[SOUTH];	
	}
	///setters y getters de edges
  	inline Edge* getWestEdge() const{
		return edges[WEST];	
	}
	/// dado que son cuatro, la copia no va a ser un costo tan importante
	/// devuelve el vector de edges
	inline EdgeList getEdges() const{
		return edges;
	}
	/// setea el elemento contenido en el vertice
  	inline void setContent( Element* content){
		vertice->setContenido(content);	
	}
	/// devuevle el elemento contenido en el vertice
  	inline Element* getContent() const{
		return vertice->getContenido();
	}
	/// devuelve el id del vertice
  	inline int getID() const{
		return vertice->getId();	
	}
	/// devuelte la cantidad de edges no nulos del vertice	
	int getActiveEdges() const;
    
};

/** \typedef
 * lista de vertices
 */
typedef std::list< Vertex* >  VertexList;

#endif
