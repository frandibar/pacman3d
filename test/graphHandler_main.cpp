#include <iostream>
#include <vector>

#include "common/graph.h"
#include "graphhandler.h"

using namespace std;

void mostrarGrafo(Graph & grafo){
	cout << "GRAFO: " << endl;
	VertexList* vertices = grafo.getVertices();
	VertexList::iterator it;
	for ( it = vertices->begin() ; it != vertices->end() ; ++it){
		cout << "VERTICE: " << (*it)->getID() << " TIPO: ";
		if ( (*it)->getContent() ){
			switch ((*it)->getContent()->getType()){
				case PILL:{
					cout << "pill";
					break;	
				}
				case POWER_UP:{
					cout << "power up";
					break;	
				}
				case BONUS:{
					cout << "bonus";
					break;	
				}
				case GHOST_HOUSE:{
					cout << "ghost house";
					break;	
				}
				case PACMAN_START:{
					cout << "pacman start";
					break;	
				}	
			}
		}else{
			cout << "VaCiO";
		}
		cout << " ARCOS: ";
		EdgeList edges = (*it)->getEdges();
		if ( edges[NORTH]){
			cout << "| Norte (ID=" << edges[NORTH]->getID() << ") ";
		}
		if ( edges[SOUTH]){
			cout << "| Sur (ID=" << edges[SOUTH]->getID() << ") ";
		}
		if ( edges[EAST]){
			cout << "| Este (ID=" << edges[EAST]->getID() << ") ";
		}
		if ( edges[WEST]){
			cout << "| Oeste (ID=" << edges[WEST]->getID() << ") ";
		}
		cout << "| " << endl;
	}
	delete vertices;
}

int main(int argc, char** argv){
	
	Graph grafo(10,10);
	GraphHandler gh(grafo);
	
	cout << gh.addElement(0,0,cPILL) << endl;
	
	mostrarGrafo(grafo);
	
	cout << gh.addElement(0,1,cPOWER_UP) << endl;
	
	mostrarGrafo(grafo);
	
	cout << gh.addElement(1,1, cBONUS) << endl;
	
	mostrarGrafo(grafo);
	
	cout << gh.addElement(1,0, cPILL) << endl;
	
	mostrarGrafo(grafo);
	
	cout << gh.addElement(2,1, cGHOUSE_N) << endl;
	
	mostrarGrafo(grafo);
	
	cout << gh.addElement(2,2, cPILL) << endl;
	
	mostrarGrafo(grafo);
	
	cout << gh.addElement(1,2, cPOWER_UP) << endl;
	
	mostrarGrafo(grafo);
	
	cout << gh.addElement(0,1, cPACMAN_START) << endl;
	
	mostrarGrafo(grafo);
	
	cout << gh.addElement(0,2, cPILL) << endl;
	
	mostrarGrafo(grafo);
	
	cout << gh.addElement(0,2, cGATE_V) << endl;
	
	mostrarGrafo(grafo);
	
	cout << gh.addElement(2,1, cEMPTY) << endl;
	
	mostrarGrafo(grafo);
	
	cout << gh.addElement(3,1, cEMPTY) << endl;
	
	mostrarGrafo(grafo);
	
	cout << gh.addElement(0,2, cWALL) << endl;
	
	mostrarGrafo(grafo);
	
	cout << gh.addElement(0,1, cPILL) << endl;
	
	mostrarGrafo(grafo);
	
	cout << gh.addElement(0,0, cPACMAN_START) << endl;
	
	mostrarGrafo(grafo);
	
	cout << gh.addElement(9,3, cPILL) << endl;
	
	mostrarGrafo(grafo);
	
	cout << gh.addElement(2,2, cGHOUSE_W) << endl;
	
	mostrarGrafo(grafo);
	
	cout << gh.addElement(0,4, cGATE_V) << endl;
	
	mostrarGrafo(grafo);
	
	cout << "CANTIDAD DE PILLS/POWER UPS: " << gh.numberOfPills_PowerUps() << endl;
	cout << "GHOST HOUSE: " << gh.hasGhostHouse() << endl;
	cout << "PACMAN START: " << gh.hasPacmanStart() << endl;
	cout << "BONUS: " << gh.hasBonus() << endl;
	
	cout << "Arcos: " << endl;
	EdgeList* edges = grafo.getEdges();
	
	EdgeList::iterator it;
	for ( it = edges->begin(); it != edges->end() ; ++it){
		cout << "ID: " << (*it)->getID() << " entre VertexID=" << (*it)->getVertex0()->getID() << " y VertexID=" << (*it)->getVertex1()->getID() << endl;
	}
	
	return 0;	
}
