/**
 * @file Grafo.h
 * */
#ifndef GRAFO_H_
#define GRAFO_H_

#include <algorithm>
#include "Vertice.h"
#include "Arco.h"

/**
 * el grafo necesita un tipo de dato que sera el contenido de los vertices, otro que sera el contenido de los arcos, y luego uno que sera el Id de los vertices y otro para el Id de los arcos
 */
template < class TVertice, class TArco, class TVerticeID , class TArcoID > class Grafo
{
public:
/// _Arco
typedef Arco< TVertice, TArco, TVerticeID , TArcoID > _Arco;
/// _WrapArco
typedef Wrapper< _Arco > _WrapArco;
/// listWrapArcos
typedef std::list< _WrapArco >  listWrapArcos;
/// _Vertice
typedef Vertice< TVertice, TArco, TVerticeID , TArcoID > _Vertice;
/// _WrapVert
typedef Wrapper< _Vertice > _WrapVert;
/// listWrapVertices
typedef std::list < _WrapVert > listWrapVertices;

private:

	listWrapVertices 	lista;
	_Vertice*			source;
	
public:
	/**
	 * Constructor
	 */
	Grafo(){
		source = 0;
	}
	/**
	 * pre: grafo inicializado
	 * post: se encarga de liberar los recursos ocupados por los vertices
	 */
	virtual ~Grafo(){
		typename listWrapVertices::iterator it;
		for ( it = lista.begin(); it != lista.end() ; it++){
			_WrapVert wVert = (*it);
			delete ( wVert.getPointer() );
		}
	}
	/**
	 * Pre: grafo inicializado.
	 * post: si el vertice existe devuelve un puntero al mismo. De lo contrario devuelve 0
	 */
	_Vertice* obtenerVertice( TVerticeID Id ){
		_Vertice* vertice = 0;
		_WrapVert wVert( new _Vertice( 0 , Id) );
		typename listWrapVertices::iterator it;
		it = find (lista.begin(), lista.end(), wVert);
		if ( it != lista.end() ) vertice = it->getPointer() ;
		delete ( wVert.getPointer() );
		return vertice;
	}
	/**
	 * Pre: grafo inicializado.
	 * post: si el vertice ya existe devuelve 0. De lo contrario agrega al grafo un vertice con el contenido y el id pasados y devuelve un puntero al mismo
	 */
	_Vertice* agregarVertice( TVertice* cont, TVerticeID verticeID){
		if ( obtenerVertice(verticeID) == 0 ){
			_Vertice* nuevo = new _Vertice(cont, verticeID);
			_WrapVert wVert( nuevo );
			lista.push_back( wVert );
			if ( source == 0 ) source = nuevo;
			return nuevo;
		}else return 0;
	}
	/**
	 * pre: grafo inicializado.
	 * post: si el vertice no existe en el grafo devuelve false. De lo contrario elimina el vertice (y libera sus recursos) y elimina todos los arcos de los vertices que apunten a el.
	 */
	bool quitarVertice ( TVerticeID Id ){
		_Vertice* vertBorrar = obtenerVertice( Id );
		_Vertice* verticeActual = 0;
		_Arco* arcoActual = 0;
		if ( vertBorrar == 0 ) return false;
		//si existe, busco que arcos apuntan a el
		typename listWrapVertices::iterator it;
		for ( it = lista.begin(); it != lista.end() ; it++){
			verticeActual = it->getPointer() ;//obtengo un vertice de la lista
			listWrapArcos listaArcos = verticeActual->obtenerArcos();//obtengo sus arcos
			typename listWrapArcos::iterator itArcos; // voy a iterar en todos
			for ( itArcos = listaArcos.begin(); itArcos != listaArcos.end() ; itArcos++){
				arcoActual = itArcos->getPointer(); //obtengo el arco del wrapper
				if ( _WrapVert(vertBorrar) == _WrapVert(arcoActual->getVerticeDestino()) ){// si tiene un arco cuyo destino es el que tengo q borrar.. la borro del vertice					
					verticeActual->quitarArco(arcoActual);// se lo quito al vertice
					delete arcoActual;// libero la memoria
					arcoActual = 0;
				}
			}			
		}
		//ahora lo borro
		_WrapVert wVert( vertBorrar );
		lista.remove(wVert);// lo remuevo de la lista
		delete vertBorrar;// libero sus recursos
		vertBorrar = 0;
		return true;
	}
	/**
	 * pre: grafo inicializado
	 * post: devuelve una copia de la lista de vertices que contiene
	 */
	listWrapVertices obtenerVertices(){
		return lista;
	}
	
	/**
	 * pre: grafo inicializado
	 * post: si el vertice existe, cambia al source por el vertice indicado, y devuelve ese vertice. Sino no cambia y devuelve 0.
	 */
	_Vertice* setSource( TVerticeID Id ){
		_Vertice* vert = obtenerVertice(Id);
		if ( vert != 0 ) source = vert;
		return vert;
	}
	
	/**
	 * pre: grafo inicializado
	 * post: devuelve un puntero al vertice source. Devuelve 0 si no hay vertices en el grafo
	 */
	_Vertice* getSource() {
		return source;
	}
	
	/**
	 * pre: los vertices deben haber sido creados previamente
	 * post: crea un arco nuevo en el grafo con el contenido pasado entre vertice origen y destino
	 */
	void agregarArco( TArco* cont, TArcoID arcoID, _Vertice* verticeOrigen, _Vertice* verticeDestino) {
		_Arco * nuevoArco = new _Arco(verticeOrigen, verticeDestino, cont, arcoID);
		verticeOrigen->agregarArco(nuevoArco);
	}
};

#endif /*GRAFO_H_*/
