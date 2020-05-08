//definicion de metodos de clase Edge
#include "edge.h"

Edge::Edge(Vertex* vertice0, Vertex* vertice1, int ID, tEdgeOrientation o){
	id = ID;
	vertex0 = vertice0;
	vertex1 = vertice1;
	orientation = o;
}

