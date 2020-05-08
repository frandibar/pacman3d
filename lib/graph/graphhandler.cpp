#include "common/graphhandler.h"

GraphHandler::GraphHandler(Graph & graph): _graph(graph)
{
	_pills = 0;
	_bonus = 0;
	_pStart = false;
	_gHouse = false;	
	// inicializo las dimensiones
	_width = _graph.getWidth();
	_height = _graph.getHeight();
	// inicializo la matriz de slots donde iran los vertices, todos en FREE
	_slots.resize(_height);	
	for ( int i = 0; i < _height ; i++){
		_slots[i].resize(_width);
		for ( int j = 0; j < _width ; j++){
			_slots[i][j] = FREE;
		}		
	}
}

bool GraphHandler::isValid(int i, int j){
	if ( ( i >= 0 ) && ( i < _height ) ){
		if ( ( j >=0 ) && ( j< _width ) )
			return true;
	}
	return false;
}

bool GraphHandler::makesSquare(int i, int j){
	int k;
	int l;
	//noroeste
	k = i-1;
	l = j;
	if ( isValid(k,l) ){
		// si el del norte esta ocupado
		if ( _slots[k][l] == OCCUPIED ){
			k = i;
			l = j-1;
			if ( isValid(k,l) ){
				// si el del oeste esta ocupado
				if ( _slots[k][l] == OCCUPIED ){	
					k = i-1;
					l = j-1;
					if ( isValid(k,l) ){
						// si el del noroeste esta ocupado, se forma un cuadrado
						if ( _slots[k][l] == OCCUPIED )
							return true;
					}										
				}
			}
		}
	}
	//noreste
	k = i-1;
	l = j;
	if ( isValid(k,l) ){
		// si el del norte esta ocupado
		if ( _slots[k][l] == OCCUPIED ){
			k = i;
			l = j+1;
			if ( isValid(k,l) ){
				// si el del este esta ocupado
				if ( _slots[k][l] == OCCUPIED ){	
					k = i-1;
					l = j+1;
					if ( isValid(k,l) ){
						// si el del noreste esta ocupado, se forma un cuadrado
						if ( _slots[k][l] == OCCUPIED )
							return true;
					}										
				}
			}
		}
	}
	//sudoeste
	k = i+1;
	l = j;
	if ( isValid(k,l) ){
		// si el del sur esta ocupado
		if ( _slots[k][l] == OCCUPIED ){
			k = i;
			l = j-1;
			if ( isValid(k,l) ){
				// si el del oeste esta ocupado
				if ( _slots[k][l] == OCCUPIED ){	
					k = i+1;
					l = j-1;
					if ( isValid(k,l) ){
						// si el del sudoeste esta ocupado, se forma un cuadrado
						if ( _slots[k][l] == OCCUPIED )
							return true;
					}										
				}
			}
		}
	}
	//sudeste
	k = i+1;
	l = j;
	if ( isValid(k,l) ){
		// si el del sur esta ocupado
		if ( _slots[k][l] == OCCUPIED ){
			k = i;
			l = j+1;
			if ( isValid(k,l) ){
				// si el del este esta ocupado
				if ( _slots[k][l] == OCCUPIED ){	
					k = i+1;
					l = j+1;
					if ( isValid(k,l) ){
						// si el del sudeste esta ocupado, se forma un cuadrado
						if ( _slots[k][l] == OCCUPIED )
							return true;
					}										
				}
			}
		}
	}
	return false;	
}

int GraphHandler::getVertexID(int i, int j){
	return ( (i*_width) + j );
}

std::vector< std::pair<int,int> > GraphHandler::getGHCoord(int i, int j, tOrientation orientation){
	std::vector< std::pair<int,int> > coord(5);	
	switch ( orientation ) {
		//devuelvo el oeste, este, sudoeste, sur, sudeste
		case NORTH:{
			coord[0].first = i;
			coord[0].second = j-1;
			coord[1].first = i;
			coord[1].second = j+1;
			coord[2].first = i+1;
			coord[2].second = j-1;
			coord[3].first = i+1;
			coord[3].second = j;
			coord[4].first = i+1;
			coord[4].second = j+1;
			break;
		}
		case SOUTH:{
			coord[0].first = i;
			coord[0].second = j-1;
			coord[1].first = i;
			coord[1].second = j+1;
			coord[2].first = i-1;
			coord[2].second = j-1;
			coord[3].first = i-1;
			coord[3].second = j;
			coord[4].first = i-1;
			coord[4].second = j+1;
			break;
		}
		case WEST:{
			coord[0].first = i;
			coord[0].second = j+1;
			coord[1].first = i-1;
			coord[1].second = j;
			coord[2].first = i-1;
			coord[2].second = j+1;
			coord[3].first = i+1;
			coord[3].second = j;
			coord[4].first = i+1;
			coord[4].second = j+1;
			break;
		}
		case EAST:{
			coord[0].first = i;
			coord[0].second = j-1;
			coord[1].first = i-1;
			coord[1].second = j;
			coord[2].first = i-1;
			coord[2].second = j-1;
			coord[3].first = i+1;
			coord[3].second = j;
			coord[4].first = i+1;
			coord[4].second = j-1;
			break;
		}
		
	}	
	
	return coord;	
}


bool GraphHandler::removeVertex(int i, int j){
	if ( _slots[i][j] == FREE )
		return true;
	
	if ( _slots[i][j] == PROHIBITED )
		return false;
			
	int vertexID = getVertexID(i,j);
	
	Vertex* vertex = _graph.getVertex( vertexID );
	
	Element* element = vertex->getContent();
	// si tiene algo, tengo que hacer cosas antes de eliminar
	if ( element ){
		
		switch ( element->getType() ){
			case PILL:
			case POWER_UP:{
				--_pills;
				break;
			}
			case BONUS:{
				_bonus = false;
				break;
			}
			case PACMAN_START:{
				_pStart = false;
				break;
			}
			case GHOST_HOUSE:{
				// pongo en empty los que corresponden a la casa
				std::vector< std::pair<int,int> > coord(getGHCoord(i,j,element->getOrientation()));
				for ( unsigned int k = 0; k < coord.size() ; ++k){
					_slots[ coord[k].first ][ coord[k].second ] = FREE;	
				}		
				_gHouse = false;
				break;
			}			
		}
		
	}else{// no tiene nada.. por lo que podria ser un portal
		Vertex* vertexPortal = NULL;
		// podria ser un portal
		if ( i == 0 ){//entonces es de la primera fila (borde de arriba)
			if ( vertex->getNorthEdge() != NULL ){// tiene portal
				// libero los costados
				_slots[i][j-1] = FREE;
				_slots[i][j+1] = FREE;
				//obtengo el correspondiente al otro lado
				vertexPortal = _graph.getVertex( getVertexID(_height-1, j) );
				// tambien lo quito
				_graph.removeVertex(getVertexID(_height-1, j));
				// lo libero
				_slots[_height-1][j] = FREE;
				// libero los costados
				_slots[_height-1][j-1] = FREE;
				_slots[_height-1][j+1] = FREE;
			}
		}else{
			if ( j == 0 ){// entonces es la primer columna (borde izquierdo)
				if ( vertex->getWestEdge() != NULL ){// tiene portal
					// libero los costados
					_slots[i+1][j] = FREE;
					_slots[i-1][j] = FREE;
					//obtengo el correspondiente al otro lado
					vertexPortal = _graph.getVertex( getVertexID(i, _width-1) );
					// tambien lo quito
					_graph.removeVertex(getVertexID(i, _width-1));
					_slots[i][_width-1] = FREE;
					// libero los costados
					_slots[i+1][_width-1] = FREE;
					_slots[i-1][_width-1] = FREE;
				}					
			}else{
				if ( i == (_height-1) ){// entonces es la ultima fila (borde de abajo)
					return removeVertex(0,j);
				}else{
					if ( j == (_width-1) )// entonces es la ultima columna (derecha)
						return removeVertex(i,0);
				}
			}
		}
	}
	//sea como sea, elimino el vertice que me piden
	_graph.removeVertex(vertexID);
	_slots[i][j] = FREE;
	return true;	
}

int GraphHandler::addGhostHouse(int i, int j, tOrientation orientation){
	Element* element = NULL;
	if ( _slots[i][j] == PROHIBITED )
		return 1;
		
	// verifico que no la quieran poner en un borde
	if ( (i == 0) || (j == 0) || (i == (_height-1)) || (j == (_width-1)) )
		return 8;
	
	if ( _gHouse ){
		Vertex* vertex = _graph.getVertex( getVertexID(i,j) );
		if ( vertex ){
			element = vertex->getContent();
			if ( element ){
				if ( element->getType() != GHOST_HOUSE )//entonces quiero pisar otra cosa, y el ghostHouse existe!
					return 5;
			}else{//si no tiene element es porq es un vertex vacio.. entonces ya existe el ghost house
				return 5;				
			}
		}else{//si no tiene el vertice es porq tiene un wall.. entonces ya existe el ghost house
			return 5;	
		}
	}
		
	if ( element ){//si llegue hasta aca y element tiene algo, es que quiero pisar un ghost house
		//lo piso directamente y si despues no puedo poner el nuevo, recupero el anterior
		removeVertex(i,j);
	}
	// voy a verificar que los que tenga que poner esten libres
	std::vector< std::pair<int,int> > coord( getGHCoord( i,j,orientation ) );
	for ( int k = 0; k < 5 ; ++k){
		if ( isValid(coord[k].first, coord[k].second) ){
			// si alguno esta ocupado o prohibido, no tengo suficinete espacio 
			if ( _slots[ coord[k].first ][ coord[k].second ] == PROHIBITED ){
				//entonces tengo que volver porq no pude poner la casa.. pero tengo que ver si recupero la que estaba!
				if ( element ){// si habia una..
					addGhostHouse(i,j, element->getOrientation());	
				}
				return 7;
			}	
		}else{
			return 7;	
		}
	}
	
	//AHORA LOS ELIMINO TODOS LOS QUE ME MOLESTEN
	if ( _slots[i][j] == OCCUPIED )
		removeVertex(i,j);
	for ( int k = 0; k < 5 ; ++k){
		removeVertex( coord[k].first , coord[k].second );
	}	
	
	// los pongo todos en prohibido
	for ( int k = 0; k < 5 ; ++k){
		_slots[ coord[k].first ][ coord[k].second ] = PROHIBITED ;
	}
	// ahora puedo colocarla
	_gHouse = true;
	addVerticesAndEdges(i, j, new Element( getVertexID(i,j) , GHOST_HOUSE, orientation));
	
	return 0;
}

int GraphHandler::addPortal(int i, int j, tEdgeOrientation orientation){
	if ( _slots[i][j] == PROHIBITED )
		return 1;// slot prohibido
	if ( i == 0 ){
		if ( orientation == HORIZONTAL )
			return 6;
		if ( !isValid(i, j+1) || !isValid(i, j-1) )
			return 7;
		
		// evaluo la otra punta
		if ( _slots[ _height-1 ][ j ]  == PROHIBITED ){
			return 1;	
		}
		//AHORA SI ESTAN OCUPADOS, LOS REMUEVO
		// primeor este lado
		if ( _slots[i][j] == OCCUPIED )
			removeVertex(i,j);
		if ( _slots[ i][ j+1 ]  == OCCUPIED )
			removeVertex(i,j+1);
		if ( _slots[ i][ j-1 ]  == OCCUPIED )
			removeVertex(i,j-1);
		//ahora los del otro lado
		if ( _slots[ _height-1 ][ j ]  == OCCUPIED )
			removeVertex(_height-1,j);
		if ( _slots[ _height-1][ j+1 ]  == OCCUPIED)
			removeVertex(_height-1,j+1);
		if ( _slots[ _height-1][ j-1 ]  == OCCUPIED )
			removeVertex(_height-1,j-1);
		
		// prohibo sus costados
		_slots[ _height-1 ][ j-1 ]  = PROHIBITED;
		_slots[ _height-1 ][ j+1 ]  = PROHIBITED;
		// LO AGREGO!
		addVerticesAndEdges(_height-1, j, NULL);
		
		// prohibo sus costados
		_slots[ i ][ j-1 ]  = PROHIBITED;
		_slots[ i ][ j+1 ]  = PROHIBITED;
		// LO AGREGO!
		addVerticesAndEdges(i, j, NULL);
		
		// SOLAMENTE ME QUEDA UNIR AMBOS!
		_graph.addEdge( _graph.getVertex( getVertexID(i, j) ) , _graph.getVertex( getVertexID(_height-1, j) ) );		
		
		return 0;			
		
	}else{
		if ( j == 0 ){
			if ( orientation == VERTICAL )
				return 6;
			if ( !isValid(i+1, j) || !isValid(i-1, j) )
				return 7;
				
			// si el analogo esta no esta libre, salgo
			if ( _slots[ i ][ _width-1 ]  == PROHIBITED ){
				return 1;	
			}
			
			//AHORA SI ESTAN OCUPADOS, LOS REMUEVO
			// primeor este lado
			if ( _slots[i][j] == OCCUPIED )
				removeVertex(i,j);
			if ( _slots[i+1][ j]  == OCCUPIED )
				removeVertex(i+1,j);
			if ( _slots[ i-1][ j ]  == OCCUPIED )
				removeVertex(i-1,j);
			//ahora los del otro lado
			if ( _slots[ i ][ _width-1 ]  == OCCUPIED )
				removeVertex(i,_width-1);
			if ( _slots[ i+1][ _width-1]  == OCCUPIED)
				removeVertex(i+1,_width-1);
			if ( _slots[ i-1][ _width-1]  == OCCUPIED )
				removeVertex(i-1,_width-1);
			
			
			// prohibo sus costados
			_slots[ i+1][ _width-1 ]  = PROHIBITED;
			_slots[ i-1 ][ _width-1 ]  = PROHIBITED;
			
			// LO AGREGO!
			addVerticesAndEdges(i, _width-1, NULL);
						
			// prohibo sus costados
			_slots[ i+1 ][ j ]  = PROHIBITED;
			_slots[ i-1 ][ j ]  = PROHIBITED;
			
			// LO AGREGO!
			addVerticesAndEdges(i, j, NULL);
			
			
			// SOLO ME QUEDA UNIR AMBOS
			_graph.addEdge( _graph.getVertex( getVertexID(i, _width-1) ) , _graph.getVertex( getVertexID(i, j ) ) );
			
			return 0;			
			
		}else{// podria ser otro borde
			if ( i == (_height-1) ){//entonces es el del borde de abajo
				//aprovecho y coloco el de arriba, total el de abajo tambien se coloca
				return addPortal(0,j, orientation);	
			}else{
				if ( j == (_width-1) ){//entonces es el del borde derecho
					// aprovecho nuevamente
					return addPortal(i,0, orientation);
				}	
			}
					
		}
	}
	return 6;// portal invalido
}

tPortalType GraphHandler::portalType(Vertex * vertex) {
	
	//calculo fila y columna a partir del numero de vertice
	int i = vertex->getID() / _width;//me da la fila
	int j = vertex->getID() - (_width * i );//me da la columna
	//ahora veo que tipo de portal podria ser
	if ( i == 0 ){//podria ser portal norte
		if ( vertex->getNorthEdge() )//si el arco norte no es nulo
			return VERTICAL_p;
	}else{
		if ( j == 0 ){//podria ser un portal oeste
			if ( vertex->getWestEdge() )//si el arco oeste no es nulo
				return HORIZONTAL_p;
		}else{
			if ( i == (_height - 1) ){//podria ser un portal sur
				if ( vertex->getSouthEdge() )//si el arco sur no es nulo
					return VERTICAL_p;
			}else{
				if ( j == (_width - 1) ){//podria ser un portal este
					if ( vertex->getEastEdge() )//si el arco este no es nulo
						return HORIZONTAL_p;
				}
			}
		}
	}
	//si no es ninguno de estos, no es ninguno
	return NONE_p;
	
}


void GraphHandler::addVerticesAndEdges(int i, int j, Element* element){
	int vertexID = getVertexID(i, j);
	Vertex* vertex = _graph.addVertex(element, vertexID);
	// vertice norte
	if ( isValid(i-1, j) ){
		if ( _slots[i-1][j] == OCCUPIED ){
			_graph.addEdge(vertex, _graph.getVertex( getVertexID(i-1, j) ) );	
		}
	}
	// vertice sur
	if ( isValid(i+1, j) ){
		if ( _slots[i+1][j] == OCCUPIED ){
			_graph.addEdge(vertex, _graph.getVertex( getVertexID(i+1, j) ) );	
		}
	}
	// vertice este
	if ( isValid(i, j+1) ){
		if ( _slots[i][j+1] == OCCUPIED ){
			_graph.addEdge(vertex, _graph.getVertex( getVertexID(i, j+1) ) );	
		}
	}
	// vertice norte
	if ( isValid(i, j-1) ){
		if ( _slots[i][j-1] == OCCUPIED ){
			_graph.addEdge(vertex, _graph.getVertex( getVertexID(i, j-1) ) );	
		}
	}
	// lo pongo como ocupado
	_slots[i][j] = OCCUPIED;
}

int GraphHandler::addEmptyVertex(int i, int j){
	if ( _slots[i][j] == PROHIBITED )
		return 1;// slot prohibido	
	
	if ( makesSquare(i,j) )
		return 3;// forma un cuadrado
		
	//si no pasa esto, puedo colocarlo.. primero, si esta ocupado, desalojo
	if ( _slots[i][j] == OCCUPIED )
		removeVertex(i,j);
		
	addVerticesAndEdges(i,j,NULL);
	
	return 0;
}

int GraphHandler::addVertex(int i, int j, tElementType type){
	
	if ( _slots[i][j] == PROHIBITED )
		return 1;// slot prohibido
			
	if ( makesSquare(i,j) )
		return 3;// forma un cuadrado
	
	if ( type == PACMAN_START)
		if ( _pStart )
			return 4;// ya existe una pacman start
			
	if ( type == BONUS)
		if ( _bonus )
			return 2;// ya existe una bonus
			
	//si paso todo esto, puede ser agregado.. asi que veo si esta ocupado
	if ( _slots[i][j] == OCCUPIED )
		removeVertex(i,j);
			
	switch ( type ){
		case PILL:
		case POWER_UP:{
			++_pills;
			break;	
		}
		case BONUS:{
			_bonus = true;
			break;	
		}
		case PACMAN_START:{
			_pStart = true;
			break;
		}	
		default: break;
	}
	
	int vertexID = getVertexID(i, j);	
	Element* element = new Element(vertexID, type, NORTH);
	
	addVerticesAndEdges(i,j,element);

	return 0;
}

int GraphHandler::addElement (int row, int column, tCell type){
	switch (type){
		case cPILL:{
			return addVertex(row, column, PILL );	
		}
		case cPOWER_UP:{
			return addVertex(row, column, POWER_UP );	
		}
		case cBONUS:{
			return addVertex(row, column, BONUS );	
		}
		case cPACMAN_START:{
			return addVertex(row, column, PACMAN_START );	
		}
		case cWALL:{
			if (removeVertex(row, column))
				return 0;
			else return 1;
		}
		case cEMPTY:{
			return addEmptyVertex(row, column);	
		}
		case cGATE_V:{
			return addPortal(row, column, VERTICAL );	
		}
		case cGATE_H:{
			return addPortal(row, column, HORIZONTAL );	
		}
		case cGHOUSE_N:{
			return addGhostHouse(row, column, NORTH );	
		}
		case cGHOUSE_S:{
			return addGhostHouse(row, column, SOUTH );	
		}
		case cGHOUSE_E:{
			return addGhostHouse(row, column, EAST );	
		}
		case cGHOUSE_W:{
			return addGhostHouse(row, column, WEST );	
		}
		default: return 0;
	}
}

int GraphHandler::validateClosedGraph() {
	
  int errorCode = 0;
  if ( numberOfPills_PowerUps() == 0 ) {
  	errorCode = 9;
  } else if ( ! hasBonus() ) {
  	errorCode = 10;	
  } else if ( ! hasPacmanStart() ) {
  	errorCode = 11;	
  } else if ( ! hasGhostHouse() ) {
  	errorCode = 12;	
  } else if ( ! _graph.isConnected() ) {
  	errorCode = 13;	
  } else if ( ! _graph.isCycleIfConnected() ) {
  	errorCode = 14;	
  }	

  return errorCode;

}

GraphHandler::~GraphHandler() {
	// Nada por ahora...
}
