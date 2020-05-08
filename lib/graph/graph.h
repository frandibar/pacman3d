/*cabecera de la clase Graph*/
#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <map>
#include <list>
#include <set>
#include <stack>
#include "common/vertex.h"
#include "common/edge.h"
#include "common/Grafo.h"

/**
 * POR CONVENCION, las aristas norte y este de un vertice son las nacientes (posicion 0)
 */
typedef enum edge_e{
	north = 0,	
	south,
	east,
	west,
	none
}tEdge;

/**
 * Esta clase representa al mapa-grafo de un nivel. Esta compuesto primeramente de los atributos ancho y altura del mapa, y por un grafo del tipo Grafo (template utilizado en el TP3 y TP4), el cual contiene una lista de Vértices utilizados en el TP3-4, los cuales contienen aristas utilizadas en los mismos TPs, y encapsula el comportamiento de grafo que debe tener el mapa. Gracias a esto, Graph permite validar si es conexo, y si es cíclico.
 * Además, Graph contiene una colección de vértices del tipo Vertex, que puede ser devuelta, y para la interfaz solamente maneja a estos tipos de vértices, los cuales manejan solamente el tipo de arista Edge. Esto lo logra gracias a un map que tiene como atributo, con el cual relaciona un tipo Vértice con su tipo Vertex que lo contiene, y dado un id, crea un elemento del tipo Vértice para el grafo template, y con ese vértice, un elemento del tipo Vertex que agrega al map. Luego, para buscar ese Vertex, ingresado el id, se busca el Vertice en el grafo template y mediante este, se busca el Vertex en el map.
 * Al momento de agregar vértices, valida que el id este en el rango permitido dado por las dimensiones (altura y ancho) del mapa. Y al momento de agregar una arista entre dos vértices, validad que el id de la arista sea coherente con los vértices que esta une, dado el formato de mapa que nos fue dado en el enunciado. Por ejemplo, la arista 0 (portal) del mapa debe unir el vértice 0 con el vértice en su misma columna y en la ultima fila, sino no serí­a coherente.
 * Esta clase permite agregar vértices, agregar aristas entre vértices, validar que sea conexo y cíclico, obtener una lista de vértices y una lista de aristas.
 * @sa Grafo
 */
class Graph {
	
typedef Grafo < Element, void, int, int > 	tGrafo;
typedef Vertex::tVertice					tVertice;
typedef std::map< tVertice* , Vertex* >		mapVertices;
typedef tGrafo::_WrapVert					tWrappedVertex;
	
  private:
    EdgeList 	*edges;//aristas en grafo
    tGrafo		graph;// estructura de grafo
    mapVertices	vertices;// vertices del grafo
    
    tEdge isValidEdge0(int edge, int vertex0);
    
    tEdge isValidEdge1(tEdge edge, int vertex0, int vertex1);
    
    tEdgeOrientation edgeOrientation(tEdge edge);
    
    bool validVertex(int id);
    
    bool isNorth(int v1, int v2);
    
    bool isEast(int v1, int v2);
    
    bool isSouth(int v1, int v2);
    
    bool isWest(int v1, int v2);
    
    int getStartingEdgeId(tEdgeOrientation orientation, int vertexID);
    
    void pushChildren(  std::stack< tWrappedVertex > & vertStack, tWrappedVertex vertice);
    
    void addEdgeFinally(int edgeID, Vertex* vertex0, Vertex* vertex1, tEdgeOrientation orientation);
    
    //los ocultamos por seguridad
    Graph (const Graph&);
    Graph& operator=(const Graph&);

  protected:
    int 		width; //< ancho
    int 		height;//< altura
    
  public:
  	
  	///
  	Graph(int width, int height);
  	
  	///
  	virtual ~Graph();
    
    /// devuelve altura
    int getWidth() const {return width;}
    
    /// devuelve altura
    int getHeight() const {return height;}
    
    /// devuelve una lista con todas las aristas. IMPORTANTE: Destruir la lista una vez terminada de utilizar, pero NO DESTRUIR LAS ARISTAS
    EdgeList* getEdges();
    /**
     * agrega un vertice al grafo y devuelve un puntero al mismo
     * si el vertice ya existe en el grafo, devuelve NULL
     */
    Vertex* addVertex(Element* content, int id);
    
    /// devuelve una lista de todos los elementos dentro del grafo
    ElementList* getElementList();
    
    /// quita el vertice y todos los arcos que a el apunten. Si no existe devuelve false
    bool removeVertex(int id);
    
    /// devuelve el vertice cuyo id es llamado
    Vertex* getVertex(int id);
    
    /// devuelve una lista de todos los vertices del grafo. IMPORTANTE: Destruir la lista una vez terminada de utilizar, pero NO DESTRUIR LOS VERTICES
    VertexList * getVertices();
    
    /** agrega una arista al grafo, entre los vertices: vertice0 ( posicion 0 de la arista) y vertice1 ( ultima posicion de la arista)
     *  devuelve true si la arista pudo ser ingresada, o sea, si su id es concordante con el formato de grafo dado
     *  NOTA: las aristas son no dirigidas.
     */
    bool addEdge(int edgeID, Vertex* vertex0, Vertex* vertex1);
    
    /// agrega una arista al grafo, entre los vertices pasados. Se encarga automaticamente de verificar si esos vertices son adyacentes y devuelve true si pudo agregarla
    bool addEdge(Vertex* vertex, Vertex* OtherVertex);
    
    /// devuelve true si el grafo es conexo
    bool isConnected();    
    
    /// devuelve true si el grafo es ciclico, dado que se sabe que es conexo
    bool isCycleIfConnected();    
    
};
#endif
