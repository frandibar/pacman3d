#include "pacmanmap.h"

PacManMap::~PacManMap() {
	// Nada por ahora...
}

bool PacManMap::isValid() {
	string errormsg;
	return isValid(errormsg);
}

bool PacManMap::isValid(string & errormsg) {
	
	// GraphHandler actua como un Helper temporal para validar que 
	// la estructura de nuestro mapa sea correcta.
	PacManMap auxMap(width, height);
	GraphHandler gh(auxMap);
	
	Vertex * vertex = NULL;
	VertexList* vertices = this->getVertices();
	VertexList::iterator it;
	for ( it = vertices->begin() ; it != vertices->end() ; ++it){
		vertex = (*it);
		tCell cellType;
		if ( vertex->getContent() ){
			switch (vertex->getContent()->getType()){
				case PACMAN_START:{
					cellType = cPACMAN_START;
					break;
				}
				case PILL:{
					cellType = cPILL;
					break;
				}
				case POWER_UP:{
					cellType = cPOWER_UP;
					break;
				}
				case BONUS:{
					cellType = cBONUS;
					break;
				}
				case GHOST_HOUSE:{
					switch ( vertex->getContent()->getOrientation() ){
						case NORTH:{
							cellType = cGHOUSE_N;
							break;
						}
						case EAST:{
							cellType = cGHOUSE_E;
							break;
						}
						case SOUTH:{
							cellType = cGHOUSE_S;
							break;
						}
						case WEST:{
							cellType = cGHOUSE_W;
							break;
						}
					}					
					break;
				}
			}
		}else {//si es null entonces esta vacio el vertice. Puede ser o no un portal
			switch( gh.portalType(vertex) ){
				case HORIZONTAL_p:{
					cellType = cGATE_H;
					break;	
				}
				case VERTICAL_p:{
					cellType = cGATE_V;
					break;	
				}
				case NONE_p:{
					cellType = cEMPTY;
					break;
				}
			}			
		}
		
		// intento subir el elemento via GraphHandler
        int row = vertex->getID() / this->width;//me da la fila
        int column = vertex->getID() - (this->width * row );//me da la columna		
		int error = gh.addElement(row, column, cellType);
		//si me devuelve 0, todo salio bien y pudo incluirlo
		if ( error != 0 ){
			errormsg = GraphErrors::getErrorText(error);
			delete vertices;
			return false;
		}
		
	} // end FOR	
	
	delete vertices;
	
	// Ahora validamos que el grafo sea ciclico y otras yerbas (que el grafo pueda cerrarse):
	int errorcode = gh.validateClosedGraph();
	if (errorcode != 0) {
		errormsg = GraphErrors::getErrorText(errorcode);
		return false;
	}
	
	// Si llegamos aca, el mapa ES VALIDO =)
	return true;
}
