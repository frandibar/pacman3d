//definicion de metodos de clase Vertice
#include "common/vertex.h"

Vertex::Vertex( tVertice * vertex){
	vertice = vertex;
	for ( int i = 0; i < 4 ; i++){
		edges.push_back(NULL);
	}	
}

int Vertex::getActiveEdges() const{
	int active = 0;	
	for ( int i = 0; i < 4 ; i++){
		if ( edges[i] )
			++active;	
	}
	return active;
}

