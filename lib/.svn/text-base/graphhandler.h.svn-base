#ifndef GRAPHHANDLER_H_
#define GRAPHHANDLER_H_

#include <iostream>
#include <vector>
#include "common/graph.h"

typedef enum tSlot{ FREE = 0, OCCUPIED = 1, PROHIBITED = 2 };
typedef enum tPortalType{VERTICAL_p, HORIZONTAL_p, NONE_p};

/**
 * Es la clase encargada de wrappear el grafo, y hacer que todas las inclusiones de elementos (vértices) que quieran hacerse, pasen por el, para poder así­ verificar que estas inclusiones son posibles, y sino, devolver un valor entero informando que tipo de error se produce. Básicamente se utiliza al momento de armar un mapa en el editor, o tambien como validador de mapas al momento de cargar uno en el servidor.
 * Básicamente valida cosas como que no se agreguen más de un PacmanStart, GhostHouse y Bonus. Además informa la cantidad de pills y powerUps que hay en el mapa (así no se pueda guardar un mapa sin al menos 1 de ellos). Valida que no se formen cuadrados de elementos en el mapa, que los portales sean colocados en los bordes, y que los GhostHouse sean colocados donde tengan suficiente espacio y donde pueda agregarse un vértice para que los fantasmas puedan salir de la casa.
 * Para esto, aparte del grafo, contiene una matriz que le informa el estado de cada uno de los cells del mapa, para que esto sea más rápido que preguntando al grafo. Lo que tiene son tres estados posibles para los cells: libre (no hay vértice en esa posición), ocupado (hay un vértice con algún elemento en esa posición), prohibido (no hay un vértice, pero limita con un portal o ghostHouse y por lo tanto no pueden colocarse vértices en esas posiciones).
 * Lo que hace este GraphHandler, es primeramente validar todas estas cuestiones, y luego agrega el vértice correspondiente al grafo. Además, evalúa si en los alrededores del vértice recientemente agregado existen otros vértices, y une los mismos mediante aristas. Si se quiere agregar un elemento tipo Wall, lo que hace es borrar el vértice seleccionado, y esto implica también borrar todos los arcos que apuntan a el.
 * Otra funcionalidad de este GraphHandler, es validar si un grafo ya armado es válido para el juego. Esto se hace mediante el armado total de una copia de ese grafo con todos los métodos de inserción validados que contiene esta clase, para asegurarse que ese grafo no es inválido dado que pudo haber sido manipulado el archivo XML donde estuvo guardado.
 */
class GraphHandler
{
private:
	Graph & _graph;
	int _width;
	int _height;
	unsigned int _pills;
	bool _bonus;
	bool _pStart;
	bool _gHouse;	
	std::vector< std::vector< tSlot > > _slots;
		
	bool makesSquare(int i, int j);
	int getVertexID(int i, int j);
	void addVerticesAndEdges(int i, int j, Element* element);
	
	/// agrega un vertice con un tipo pill, power up, bonus, o pacman start. Devuelve 0 si fue con exito, y sino los errores correspondientes
	int addVertex(int i, int j, tElementType type);
	/// agrega un vertice con elemento NULL. Devuelve 0 si fue con exito, y sino los errores correspondientes
	int addEmptyVertex(int i, int j);
	/// agrega un vertice con un tipo ghost house. Devuelve 0 si fue con exito, y sino los errores correspondientes
	int addGhostHouse(int i, int j, tOrientation orientation);
	/// agrega DOS vertices con conectados por un portal, con elemento NULL. Devuelve 0 si fue con exito, y sino los errores correspondientes
	int addPortal(int i, int j, tEdgeOrientation orientation);
	/// remueve el vertice. Devuelve true si pudo, false si había un vertice prohibido
	bool removeVertex(int i, int j);

public:
	///
	GraphHandler(Graph & graph);
	
	///
	virtual ~GraphHandler();
	
	/// devuelve cantidad de pills y powerups en el mapa
	inline unsigned int numberOfPills_PowerUps() const{
		return _pills;	
	}
	
	/// devuelve si el mapa tiene bonus
	inline bool hasBonus() const{
		return _bonus;	
	}
	
	/// devuelve si el mapa tiene pacman start
	inline bool hasPacmanStart() const{
		return _pStart;
	}	
	
	/// devuelve si el mapa tiene ghost house
	inline bool hasGhostHouse() const{
		return _gHouse;	
	}
	
	/// agrega un vertice con el elemento del tipo pasado al grafo, y devuelve 0. Si ocurriera algun error, devuelve el entero correspondiente al mismo.
	int addElement (int row, int column, tCell type);
	
	/// devuelve un par(fila,columna) de los vertices que rodean al vertice ghostHouse, dependiendo la orientacion indicada
	static std::vector< std::pair<int,int> > getGHCoord(int i, int j, tOrientation orientation);
	
	/// devuelve el tipo de portal que sería el vertice. Si no es porta, devuelve NONE_p
	tPortalType portalType(Vertex * vertex);
	
	/**
	// valida que el grafo pueda cerrarse, es decir, que cumpla con todos los requisitos 
	// para ser un mapa de pacman (contar con los elementos obligatorios, que sea un circuito
	// cerrado, que sea conexo y ciclico, etc)
	// Devuelve 0 si esta OK. Caso contrario devuelve codigo de error (buscar el texto 
	// descriptivo por este codigo en GraphErrors::getErrorText(errorcode) )
	 */
	int validateClosedGraph();
	
	/// devuelve si las coordenadas son validas para un vertice del mapa(grafo)
	bool isValid(int i, int j);

	
};

#endif /*GRAPHHANDLER_H_*/
