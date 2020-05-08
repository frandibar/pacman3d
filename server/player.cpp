/*implementacion de clase Player*/
#include "server/player.h"
#include "lib/header.h"

const unsigned short Player::steps = 16;
const unsigned short Player::ratio = 16;

//constructor
Player::Player(const int &idPlayer, const Edge * tunnel, int initPosition, tPlayerOrientation orientation, Mutex & key):_key(key){
  _tunnel = tunnel;
  _edgeID = tunnel->getID();
  _idPlayer = idPlayer;
  _orientation = orientation;
  _speed = PLAYER_SPEED;
  _position = initPosition;
  _alive = true;
}

//jugador toco tecla
void Player::keyPressed(const tKeyType &key) {
	switch (key) {
		case (UP):{
			this->move();
			break;
		}
		case (DOWN):{
			// giro de 180 grados
			switch (_orientation){
				case (EAST_NORTH):{
					setOrientation(WEST_SOUTH);
					break;
				}
				case (WEST_SOUTH):{
					setOrientation(EAST_NORTH);
					break;
				}
			}
			break;
		}
		case (LEFT):{
			// giro de 90 grados hacia izquierda
			if ( (_position == 63) || (_position == 0) ){
				 tOrientation newOrient;
				 tOrientation playerOrientation = getOrientation();
				switch (playerOrientation){
					case (NORTH):{
						newOrient = WEST;
						break;
					}
					case (EAST):{
						newOrient = NORTH;
						break;
					}	
					case (SOUTH):{
						newOrient = EAST;
						break;
					}	
					case (WEST):{
						newOrient = SOUTH;
						break;
					}	
				}
				// ahora busco si puedo obtener un arco en esa orientacion
				Vertex* vertex = NULL;
				if ( _position == 63 ){
					vertex = _tunnel->getVertex1();
				}else{
					vertex = _tunnel->getVertex0();
				}
				const Edge* next = vertex->getEdges()[newOrient];
				if ( next ){
					if ( ( newOrient == NORTH ) || ( newOrient == EAST ) ){						
						_position = 0;
						setOrientation(EAST_NORTH);	
					}else{
						_position = 63;
						setOrientation(WEST_SOUTH);
					}
					setEdge(next);
				}			
			}		
			break;
		}
		case (RIGHT):{
			// giro de 90 grados hacia derecha
			if ( (_position == 63) || (_position == 0) ){
				 tOrientation newOrient;
				 tOrientation playerOrientation = getOrientation();
				switch (playerOrientation){
					case (NORTH):{
						newOrient = EAST;
						break;
					}
					case (EAST):{
						newOrient = SOUTH;
						break;
					}	
					case (SOUTH):{
						newOrient = WEST;
						break;
					}	
					case (WEST):{
						newOrient = NORTH;
						break;
					}	
				}
				// ahora busco si puedo obtener un arco en esa orientacion
				Vertex* vertex = NULL;
				if ( _position == 63 ){
					vertex = _tunnel->getVertex1();
				}else{
					vertex = _tunnel->getVertex0();
				}
				const Edge* next = vertex->getEdges()[newOrient];
				if ( next ){
					if ( ( newOrient == NORTH ) || ( newOrient == EAST ) ){						
						_position = 0;
						setOrientation(EAST_NORTH);	
					}else{
						_position = 63;
						setOrientation(WEST_SOUTH);
					}
					setEdge(next);
				}				
			}		
			break;
		}
	}
}

void Player::setEdge(const Edge* edge){	
	_edgeID = edge->getID();	
	_tunnel = edge;
}

tOrientation Player::getOrientation(){
	// segun la orientacion del jugador y la de la arista, saco la orientacion verdadera
	if ( _orientation == EAST_NORTH ){
			if ( _tunnel->getOrientation() == VERTICAL ){
				return NORTH;
			}
			return EAST;
	}else{
			if ( _tunnel->getOrientation() == VERTICAL ){
				return SOUTH;
			}
			return WEST;
		
	}
}

