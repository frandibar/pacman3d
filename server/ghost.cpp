//definicion de metodos de clase Ghost
#include "server/ghost.h"

//constructor
Ghost::Ghost(const int &idPlayer, const Edge * tunnel, int initPosition, tPlayerOrientation orientation, PlayerActioner & positioner, Mutex & key):Player(idPlayer, tunnel, initPosition, orientation, key),_positioner(positioner){
}


void Ghost::move(){
	std::pair< int , const Edge* > atributes = _positioner.evaluateMovement(*this);
	if ( atributes.first != -1 ){ // entonces se movio
		Thread::Sleep(getSpeed());
		
		lockPlayers();
		//modifico la posicion
		setEdge(atributes.second);
		setPosition(atributes.first);
		if ( !isAlive() ){//si esta muerto el fantasma, veo que llegue a la casa
			bool revive = _positioner.evaluateRevivalInteraction(*this);
			setAlive(revive);//si me dio que puedo revivir
		}
		
		unlockPlayers();
	}
	
}
