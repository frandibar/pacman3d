//definicion de metodos de clase Pacman
#include "server/pacman.h"

//constructor
Pacman::Pacman(const int &idPlayer, const Edge * tunnel, int initPosition, tPlayerOrientation orientation, PlayerActioner & positioner, Mutex & key):Player(idPlayer, tunnel, initPosition, orientation, key),_positioner(positioner) {
	_poweredUp = false;
	_eatenElement = NULL;
}


void Pacman::move(){
	//evalua el movimiento siguiente
	std::pair< int , const Edge* > atributes = _positioner.evaluateMovement(*this);
	if ( atributes.first != -1 ){ // entonces se movio
		Thread::Sleep(getSpeed());
		
		lockPlayers();
		//modifico la posicion
		setEdge(atributes.second);
		setPosition(atributes.first);
		
	}
	
	Element* element = _positioner.evaluateElementInteraction(*this);
	//seteo el elemento que me devolvio como comido. Podria ser nulo
	_eatenElement = element;
	
	unlockPlayers();	
	
}
