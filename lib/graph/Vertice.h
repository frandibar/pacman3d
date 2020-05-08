#pragma once

#include <list>

#include "Arco.h"
#include "Wrapper.h"

/**
 * En esta clase, todos los arcos serán los que salgan de ese vertice 
 * El vertice necesita un tipo de dato que sera el contenido de los vertices, otro que sera el contenido de sus arcos, y luego uno que sera el Id de los vertices y otro para el Id de sus arcos
 */
template < class TVertice, class TArco, class TVerticeID , class TArcoID > class Vertice {
public:

/// _Arco
typedef Arco< TVertice, TArco, TVerticeID , TArcoID > _Arco;

/// _WrapArco
typedef Wrapper< _Arco > _WrapArco;

/// listWrapArcos
typedef std::list< _WrapArco >  listWrapArcos;

protected:
	listWrapArcos	lista;
	TVertice*		cont;
	TVerticeID		Id;
	
public:
	/**
	 * 
	 */
	Vertice(TVertice* contenido, TVerticeID id){
		cont = contenido;
		Id = id;
	}
	/**
	 * pre: vertice inicializado.
	 * post: se encarga de liberar los recursos ocupados por los arcos y el contenido del vertice
	 */
	virtual ~Vertice(){
		typename listWrapArcos::iterator it;
		for ( it = lista.begin(); it != lista.end() ; it++){
			_WrapArco wArco = (*it);
			delete ( wArco.getPointer() );
		}
		delete cont;
	}
	
	/**
	 * pre: vertices inicializados.
	 * post: devuelve true si los id de los vertices coinciden y false de lo contrario
	 */
	bool operator==(const Vertice& other) const {
		return ( Id == (other.Id) );
	}
	
	/**
	 * 
	 */
	bool operator<(const Vertice& other) const{
		return ( Id < (other.Id) );	
	}
	/**
	 * pre: vertice inicializado.
	 * post: agrega el arco pasado a la lista de arcos del vertice
	 */
	void agregarArco( _Arco* arco ){
		_WrapArco wArco(arco);
		lista.push_back(wArco);
	}
	/**
	 * pre: vertice inicializado.
	 * post: devuelve una copia de la lista de arcos del vertice
	 */
	listWrapArcos obtenerArcos(){
		return lista;
	}
	/**
	 * pre: vertice inicializado.
	 * post: quita el arco pasado de la lista de arcos del vertice.IMPORTANTE: liberar los recursos ocupados por ese arco luego de quitarlo del vertice
	 */
	void quitarArco( _Arco* arco ){
		_WrapArco wArco(arco);
		lista.remove(wArco);
	}
	TVertice* getContenido(){
		return cont;
	}
	void setContenido(TVertice* contenidoNuevo){
		cont = contenidoNuevo;
	}
	TVerticeID getId(){
		return Id;
	}
	void setId(TVerticeID id){
		Id = id;
	}
		
};

