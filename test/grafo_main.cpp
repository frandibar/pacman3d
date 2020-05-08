#include <iostream>
#include "common/graph.h"
#include "common/utilities.h"

using namespace std;

int main(int argc, char** argv){

	Graph grafo(5, 4);
	
	Vertex* v0 = grafo.addVertex(0, 0);
	Vertex* v1 = grafo.addVertex(0, 1);
	
	/*-*/
	
	Vertex* v = grafo.addVertex(0, 16);
	grafo.addEdge(1, v1, v);
	
	/*-*/
	
	
	bool exito = grafo.addEdge(v1, v0);
	
	if ( exito )
		cout << "Se ha generado el arco!!" << endl;
	
	v1 = grafo.addVertex(0, 15);
	
	exito = grafo.addEdge(v1, v0);
	
	/*-*/
	
	grafo.addEdge(35, v1, v);
	
	/*-*/
	
	if ( !exito ) cout << "No se pudo agregar el arco" << endl;
	else cout << "Se ha agregado el arco con exito!" << endl;
	
	cout << "La cantidad de aristas del vertice " << v0->getID() << " es: " << v0->getActiveEdges() << endl;
	
	cout << "La cantidad de aristas del vertice " << v1->getID() << " es: " << v1->getActiveEdges() << endl;
	
	EdgeList aristas = v0->getEdges();
	
	cout << "La arista este del vertice " << v0->getID() << " es la arista: " << aristas[EAST]->getID() << endl; 
	
	Vertex* myVertex = grafo.getVertex(15);
	
	cout << "La arista sur del vertex 15 es: "<< myVertex->getSouthEdge()->getID() << endl;
	
	myVertex = grafo.getVertex(1);
	
	cout << "La arista norte del vertex 1 es: "<< myVertex->getNorthEdge()->getID() << endl;
	
	if ( grafo.isConnected() ) cout << "El grafo es conexo!" << endl;
	else cout << "El grafo No es conexo" << endl;
	
	if ( grafo.isCycleIfConnected() ) cout << "El grafo es ciclico tambien!" << endl;
	else cout << "El grafo No es ciclico" << endl;	
	
	return 0;	
}
