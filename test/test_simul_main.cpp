#include <iostream>
#include "server/playerhandler.h"

using namespace std;

void armarGrafo(Graph & grafo, Vertex * & pacStart, Vertex * & ghostHouse){
	pacStart = grafo.addVertex(new Element(0, PACMAN_START, NORTH) , 0);
	grafo.addVertex(new Element(1, PILL, NORTH) , 1);
	ghostHouse = grafo.addVertex(new Element(2, GHOST_HOUSE, NORTH) , 2);
	grafo.addVertex(new Element(3, PILL, NORTH) , 3);
	grafo.addVertex(new Element(5, BONUS, NORTH) , 5);
	grafo.addVertex( NULL, 6);
	grafo.addVertex( NULL , 7);
	grafo.addVertex(new Element(8, POWER_UP, NORTH) , 8);
	//agrego aristas
	cout << grafo.addEdge(grafo.getVertex(0), grafo.getVertex(1)) << endl;
	cout << grafo.addEdge(grafo.getVertex(1), grafo.getVertex(2)) << endl;
	cout << grafo.addEdge(grafo.getVertex(8), grafo.getVertex(2)) << endl;
	cout << grafo.addEdge(grafo.getVertex(0), grafo.getVertex(3)) << endl;
	cout << grafo.addEdge(grafo.getVertex(5), grafo.getVertex(2)) << endl;
	cout << grafo.addEdge(grafo.getVertex(3), grafo.getVertex(6)) << endl;
	cout << grafo.addEdge(grafo.getVertex(8), grafo.getVertex(5)) << endl;
	cout << grafo.addEdge(grafo.getVertex(6), grafo.getVertex(7)) << endl;
	cout << grafo.addEdge(grafo.getVertex(8), grafo.getVertex(7)) << endl;
	cout << "Si todos fueron unos, el grafo fue bien creado.." << endl << endl;
	
}

int main ( int argc, char** argv ){
	
	Graph grafo(3,3);
	
	Vertex * pacStart;
	Vertex * ghostHouse;
	
	armarGrafo(grafo, pacStart, ghostHouse);
	
	Game game(grafo, pacStart, ghostHouse, 3);
	
	PlayerHandler & ph = *game.getPH();
	
	//AGREGO LOS JUGADORES PRIMERO
	
	game.agregarJugador();//pacman
	game.agregarJugador();//un fantasma
	
	//AHORA LANZO EL THREAD
	
	game.run();
	
	char key = 0;
	while ( toupper(key) != 'X'){
		
		cout << endl << "Movete! (q = avanzar, s = rotar 180, d = rotar 90 der, a = rotar 90 izq): ";
		cin >> key;
		if ( !game.estaJugando() )
			break;
		switch ( key ){
			case ('q'):{
				ph.movePlayer(0, UP);	
				break;
			}
			case ('s'):{
				ph.movePlayer(0, DOWN);	
				break;
			}
			case ('a'):{
				ph.movePlayer(0, LEFT);	
				break;
			}	
			case ('d'):{
				ph.movePlayer(0, RIGHT);	
				break;
			}
			default: break;	
		}
		if ( !game.estaJugando() )
			break;
	}
	cout << "FIN..." << endl;
	
	game.terminarJuego();
	game.join();

	return 0;	
}
