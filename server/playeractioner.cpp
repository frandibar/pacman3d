#include "playeractioner.h"

PlayerActioner::PlayerActioner(Graph & levelGraph): _levelGraph(levelGraph)
{
}

PlayerActioner::~PlayerActioner()
{
}

short int PlayerActioner::getDirectionSign(tPlayerOrientation playerOrient){
	
	if ( playerOrient == EAST_NORTH )// si viene en direccion este-norte, esta sumando
		return 1;
	// sino, esta restando posiciones
	return -1;
}

std::pair< int , const Edge* > PlayerActioner::evaluateMovement(Player & player){
	
	const Edge * actualEdge = player.getEdge();
	int position = player.getPosition();
	tPlayerOrientation playerOrient = player.getDirection();
	//tEdgeOrientation playerEdgeO = actualEdge->getOrientation();
	tOrientation realPlayerOrientation = player.getOrientation();
	
	const Edge*	nextEdge = NULL;
	const Vertex*	vertex = NULL;
	short int directionSign = getDirectionSign(playerOrient);
	int newPosition = -1;
	
	if ( playerOrient == EAST_NORTH){// si viene avanzando, sumando
		if ( position == 63 ){
			// este es el vertice en el que estoy parado si estoy en posicion 63
			vertex = actualEdge->getVertex1();
			// el proximo sera en la misma orientacion
			nextEdge = (vertex->getEdges())[realPlayerOrientation];
			
			if ( nextEdge == NULL ) return std::pair< int , const Edge* >(-1,NULL);
			
			newPosition = 0;
			
		}else{
			newPosition = position;	
		}
	}else{
		if ( position == 0 ){
			//lo mismo pero al reves
			vertex = actualEdge->getVertex0();
			nextEdge = (vertex->getEdges())[realPlayerOrientation];
			// si no tiene arista en esa orientacion, no se puede mover, choca contra una pared, por lo que devuelve false
			
			if ( nextEdge == NULL ) return std::pair< int , const Edge* >(-1,NULL);

			newPosition = 63;
		}else{
			newPosition = position;	
		}
	}
	// ahora tengo que posicionarlo en su nueva posicion, que me asegure que existe
	int finalPosition = newPosition + ( directionSign * Player::steps );	
	finalPosition = (finalPosition < 0)? (0) : ( (finalPosition > 63)? (63) : (finalPosition) );
	
	if ( nextEdge == NULL ) nextEdge = actualEdge;
		
	return std::pair< int , const Edge* >( finalPosition , nextEdge );
}

Element* PlayerActioner::evaluateElementInteraction(Player & player){
	Element* ret = NULL;
	const Edge * actualEdge = player.getEdge();
	int position = player.getPosition();
	unsigned short playerRatio = Player::ratio;
	unsigned short elementRatio = Element::ratio;
	Vertex* actualV = NULL;
	//verifico los dos posibles extremos del arco
	if ( (position - playerRatio - elementRatio) < 0 ){//estoy comiendo un elemento del extremo 0 del arco
		actualV = actualEdge->getVertex0();
		if ( actualV->getContent() ){// si el element no es nulo
			if ( (actualV->getContent()->getType() != PACMAN_START) && (actualV->getContent()->getType() != GHOST_HOUSE) ){
				ret = actualV->getContent();
				actualV->setContent(NULL);
			}
		}
	}else{
		if ( (position + playerRatio + elementRatio ) > 63 ){//estoy comiendo un elemento del extremo 63 del arco
			actualV = actualEdge->getVertex1();
			if ( actualV->getContent() ){// si el element no es nulo
				if ( (actualV->getContent()->getType() != PACMAN_START) && (actualV->getContent()->getType() != GHOST_HOUSE) ){
					ret = actualV->getContent();
					actualV->setContent(NULL);
				}
			}
		}
	}
	return ret;
}

bool PlayerActioner::evaluateRevivalInteraction(Player & player){
	const Edge * actualEdge = player.getEdge();
	int position = player.getPosition();
	Vertex* actualV = NULL;
	bool ret = false;
	
	if ( position == 0 ){//estoy en un vertice, verifico si puedo revivir
		actualV = actualEdge->getVertex0();
		if ( actualV->getContent() ){
			if ( actualV->getContent()->getType() == GHOST_HOUSE )
				ret = true;
		}
	}else{
		if ( position == 63 ){//estoy en un vertice, verifico si puedo revivir
			actualV = actualEdge->getVertex1();
			if ( actualV->getContent() ){
				if ( actualV->getContent()->getType() == GHOST_HOUSE )
					ret = true;
			}				
		}	
	}
	return ret;	
}

bool PlayerActioner::evaluatePlayerInteraction(Player & player1, Player & player2){
	const Edge * actualEdgeP1 = player1.getEdge();
	int positionP1 = player1.getPosition();
	const Edge * actualEdgeP2 = player2.getEdge();
	int positionP2 = player2.getPosition();
	unsigned short playerRatio = Player::ratio;
	
	//si alguno no esta vivo listo..
	if ( !player1.isAlive() || !player2.isAlive() ) return false;
	
	if ( actualEdgeP1 == actualEdgeP2 ){//estan en el mismo arco, verifico si se tocan
		if ( abs(positionP1 - positionP2) < 2*playerRatio )
			return true;			
	}else{//si no estan en el mismo, es mas complicado
		if ( actualEdgeP1->getOrientation() == actualEdgeP1->getOrientation() ){//si estan igualmente orientados
			if ( (positionP1 - playerRatio ) < (positionP2 + playerRatio -63) ){// player uno esta del lago decreciente y player dos del lado creciente
				if ( actualEdgeP1->getVertex0() == actualEdgeP2->getVertex1() )	
					return true;
			}else{
				if ( (positionP2 - playerRatio ) < (positionP1 + playerRatio -63) ){// caso contrario
					if ( actualEdgeP2->getVertex0() == actualEdgeP1->getVertex1() )	
						return true;
				}	
			}
		}
	}
		
	return false;
}
