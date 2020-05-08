/**
 * @file Arco.h
 * 
 * */
#ifndef ARCO_H_
#define ARCO_H_

#include <iostream>

template < class TVertice, class TArco, class TVerticeID , class TArcoID  > class Vertice;

/**
 * el arco necesita un tipo de dato que sera el contenido de sus vertices, otro que sera el contenido de los arcos, y luego uno que sera el Id de sus vertices y otro para el Id de los arcos
 */
template < class TVertice, class TArco, class TVerticeID , class TArcoID  > class Arco {

/**
 * _Vertice
 * */	
typedef Vertice < TVertice, TArco, TVerticeID , TArcoID > _Vertice;

private:
	_Vertice* 	verticeOrigen;
	_Vertice* 	verticeDestino;
	TArco* 		cont; 
	TArcoID		Id;
	
public:
	/**
	 * 
	 */
	Arco( _Vertice* verticeO, _Vertice* verticeD, TArco* contenido, TArcoID id){
		verticeOrigen = verticeO;
		verticeDestino = verticeD;
		cont = contenido;
		Id = id;		
	}
	/**
	 * pre: arco inicializado.
	 * post: libera los recursos ocupados por el contenido. NO libera los recursos ocupados por los vertices origen y destino
	 */
	virtual ~Arco(){
		//delete cont;
	}
	/**
	 * pre: arcos inicializados.
	 * post: devuelve true si los id de los arcos coinciden y false de lo contrario.
	 */
	bool operator==(const Arco& other) const {
		return ( Id == other.Id);
	}
	/**
	 * pre: arcos inicializados.
	 * post: devuelve un puntero al vertice origen del arco
	 */
	_Vertice* getVerticeOrigen(){
		return verticeOrigen;
	}
	/**
	 * pre: arcos inicializados.
	 * post: devuelve un puntero al vertice destino del arco
	 */
	_Vertice* getVerticeDestino(){
		return verticeDestino;
	}
	
	/**
	 * 
	 */
	void setVerticeOrigen(_Vertice* vertice){
		verticeOrigen = vertice;
	}
	
	/**
	 * 
	 */	
	void setVerticeDestino(_Vertice* vertice){
		verticeDestino = vertice;
	}
	
	/**
	 * 
	 */	
	TArco* getContenido(){
		return cont;
	}
	
	/**
	 * 
	 */	
	void setContenido(TArco* contenidoNuevo){
		cont = contenidoNuevo;
	}
	
	/**
	 * 
	 */	
	TArcoID getId(){
		return Id;
	}
	
	/**
	 * 
	 */	
	void setId(TArcoID id){
		Id = id;
	}

};

#endif /*ARCO_H_*/
