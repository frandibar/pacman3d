//definicion de metodos de clase Graph
#include "common/graph.h"

Graph::Graph(int width, int height){
	this->width = width;
	this->height = height;
	edges = new EdgeList();
}

Graph::~Graph(){
	//destruyo los vertices del map
	mapVertices::iterator it;
	for ( it = vertices.begin(); it != vertices.end() ; ++it){
		delete (it->second);
	}
	//destruyo los arcos de la lista
	EdgeList::iterator it2;
	for ( it2 = edges->begin(); it2 != edges->end() ; ++it2){
		delete (*it2);
	}
	//destruyo la lista
	delete edges;
}

Vertex* Graph::addVertex(Element* content, int id){
	if ( !validVertex(id) ) return NULL;
	tVertice*	vertice = graph.agregarVertice(content, id);
	if ( vertice != 0 ){
		Vertex* vertex = new Vertex(vertice);
		vertices[ vertice ] = vertex;
		return vertex;
	}else{
		return NULL;	
	}
}

Vertex* Graph::getVertex(int id){
	tVertice* vertice = graph.obtenerVertice(id);
	if ( vertice == NULL ) return NULL;
	return vertices [ vertice ];	
}

VertexList* Graph::getVertices(){
	VertexList* vertexList = new VertexList;
	VertexList::iterator itNewVertices;
	mapVertices::iterator it;
	//recorro el map con los vertices y me armo una lista con cada uno.
	for ( it = vertices.begin(); it != vertices.end() ; ++it){
		//esto lo hago para buscar uno mayor e insertarlos ordenadamente
		for ( itNewVertices = vertexList->begin() ; itNewVertices != vertexList->end(); ++itNewVertices){
			if ( it->second->getID() < (*itNewVertices)->getID() )
				break;
		}
		//inserto en mi lista, antes del que es mayor a el
		vertexList->insert(itNewVertices, it->second);
	}
	return vertexList;
}

EdgeList* Graph::getEdges(){
	EdgeList* edgeList = new EdgeList;
	EdgeList::iterator itNewEdges;
	EdgeList::iterator it;
	//recorro la lista de arcos y me armo otra lista con cada uno.
	for ( it = edges->begin(); it != edges->end() ; ++it){
		//esto lo hago para buscar uno mayor e insertarlos ordenadamente
		for ( itNewEdges = edgeList->begin() ; itNewEdges != edgeList->end(); ++itNewEdges){
			if ( (*it)->getID() < (*itNewEdges)->getID() )
				break;
		}
		//inserto en mi lista, antes del que es mayor a el
		edgeList->insert(itNewEdges, *it);
	}
	return edgeList;
}

tEdge Graph::isValidEdge0(int edge, int vertex0){
	// esta posicion me da idea de las dos aristas posibles que puede tener vertex0
	int pos = vertex0 / width;
	// este numero me da algo normalizado, que es multiplo del ancho si es valido
	int dif = edge - vertex0;
	// ahora verifico que de un multiplo con respecto a pos y pos+1
	if ( dif == ( width * pos ) )
		return north;
	// si es distinto, no es arco norte
	// veo si es este
	if ( dif == ( width * (pos + 1) ) )
		return east;
	// si es distinto, no es arco este tampoco. Por lo q no es valido
	return none;
					
}

tEdge Graph::isValidEdge1(tEdge edgeV0,  int vertex0, int vertex1){
	
	switch ( edgeV0 ){
		case (north):{// si la arista del vertice naciente es norte
			if ( (vertex0 - vertex1) != width ){// si la diferencia no es el ancho
				// puede que sea un portal
				if ( ( vertex1 - vertex0 ) != ( width * (height - 1) ) )
					return none;
			}
			// si paso las verificaciones, y el vertice naciente es el norte, es la sur del vertice entrante
			return south;
		}
		case (east):{
			if ( (vertex1 - vertex0) != 1 ){// si no es 1 es porq podria ser un portal
				if ( (vertex0 - vertex1) != (width - 1) )
					return none;				
			}
			// si paso las verificaciones y es el este del vertice naciente, entocnes es el oeste del vertice entrante
			return west;
		}
		default: return none;
	}					
}
// el edge de type0 puede ser o north o west
tEdgeOrientation Graph::edgeOrientation(tEdge edge){
	if ( edge == north )
		return VERTICAL;
	return HORIZONTAL;
}

void Graph::addEdgeFinally(int edgeID, Vertex* vertex0, Vertex* vertex1, tEdgeOrientation orientation){
	// ahora puedo agregar la arista al verdadero grafo
	tVertice* vertice0 = vertex0->vertice;
	tVertice* vertice1 = vertex1->vertice;
	// agrego la arista en ambas direcciones. Y no me importa su contenido o ID
	graph.agregarArco(0,edgeID, vertice0, vertice1);
	graph.agregarArco(0,edgeID, vertice1, vertice0);
	
	// ahora tengo que agregar el edge a este grafo y a sus vertex
	Edge* edge = new Edge(vertex0, vertex1, edgeID, orientation );
	edges->push_back( edge );
	// a los vertices tambien
	switch ( orientation ){
		case ( VERTICAL ):{
			vertex0->setNorthEdge( edge );
			vertex1->setSouthEdge( edge );
			break;
		}
		case ( HORIZONTAL ):{
			vertex0->setEastEdge( edge );
			vertex1->setWestEdge( edge );
			break;	
		}
		default: break;
	}
}

bool Graph::addEdge(int edgeID, Vertex* vertex0, Vertex* vertex1){
	tEdge type0 = isValidEdge0(edgeID, vertex0->getID() );
	// verifico que sea valido
	if ( type0 == none ) return false;
	
	tEdge type1 = isValidEdge1(type0, vertex0->getID(), vertex1->getID());
	// verifico que sea valido
	if ( type1 == none ) return false;
	
	addEdgeFinally(edgeID, vertex0, vertex1, edgeOrientation(type0));
	
	return true;	
}

void Graph::pushChildren( std::stack< tWrappedVertex > & vertStack, tWrappedVertex vertice){
	tVertice::listWrapArcos edgeList = vertice.getPointer()->obtenerArcos();
	tVertice::listWrapArcos::iterator it;
	for ( it = edgeList.begin() ; it != edgeList.end() ; ++it){
		vertStack.push( tWrappedVertex(it->getPointer()->getVerticeDestino()) );
	}
}

bool Graph::isConnected(){
	std::stack< tWrappedVertex > vertStack;//pila de vertices
	std::set< tWrappedVertex > vertSet;//set de vertices
	//si esta vacio de vertices ya devuelvo false
	if ( vertices.empty() )
		return false;
	
	tWrappedVertex source(vertices.begin()->first);	
	
	vertStack.push(source);
	while ( ! vertStack.empty() ){
		tWrappedVertex actual(vertStack.top());
		vertStack.pop();
		/*
		 * El algoritmo es, si debe marcarse (o sea, no esta en el set), entonces marcar y agregar hijos
		 * El insert devuelve un pair con true en su segundo miembro si pudo insertarlo, entonces, no estaba en el set, y por lo tanto ya fue insertado
		 */
		if ( ( vertSet.insert(actual) ).second ){//si debe marcarse, marcar
			pushChildren(vertStack, actual);//apilar hijos
		}
	}
	// ahora tengo todos "marcados", los tengo en el set, y si no hay la misma cantidad que el total de vertices, alguno esta suelto
	return ( vertSet.size() == vertices.size() );	
}
    
bool Graph::isCycleIfConnected(){
	//aca solamente veo que tenga dos arcos (salvo para la casa de los fantasmas). Con eso es suficiente para que sea ciclico (si es que es conexo)
	mapVertices::iterator it;
	for ( it = vertices.begin(); it != vertices.end() ; ++it){
		if ( (it->second)->getActiveEdges() < 2 ){
			if ( (it->second)->getContent() ){				
				if ( (it->second)->getContent()->getType() != GHOST_HOUSE ){
					return false;
				}
			}else{
				return false;	
			}
		}			
	}
	return true;
}

bool Graph::validVertex(int id){
	if ( (id < 0) || ( id > ( width*height - 1) ) )
		return false;
	return true;	
}

int Graph::getStartingEdgeId(tEdgeOrientation orientation, int vertexID){
	int pos = vertexID / width;
	if ( orientation == VERTICAL ){// tonces tengo que darle la arista norte
		return ( ( width * pos ) + vertexID );
	}else{// sino, es la arista este
		return ( ( width * ( pos + 1 ) ) + vertexID );
	}
}

bool Graph::isNorth(int v1, int v2){
	if ( v1 < v2 ){// entonces podria ser un portal
		if ( ( v2 - v1 ) == ( width * (height - 1) ) )
			return true;
		return false;
	}else{// no es portal
		if ( ( v1 - v2 ) == width )
			return true;
		return false;
	}
}

bool Graph::isSouth(int v1, int v2){
	if ( v2 < v1 ){// entonces podria ser un portal
		if ( ( v1 - v2 ) == ( width * (height - 1) ) )
			return true;
		return false;
	}else{// no es portal
		if ( ( v2 - v1 ) == width )
			return true;
		return false;
	}
}

bool Graph::isWest(int v1, int v2){
	if ( v1 < v2 ){// entonces podria ser un portal
		if ( ( v2 - v1 ) == ( width - 1 )  )
			return true;
		return false;
	}else{// no es portal
		if ( ( v1 - v2 ) == 1 )
			return true;
		return false;
	}
}

bool Graph::isEast(int v1, int v2){
	if ( v2 < v1 ){// entonces podria ser un portal
		if ( ( v1 - v2 ) == ( width - 1 )  )
			return true;
		return false;
	}else{// no es portal
		if ( ( v2 - v1 ) == 1 )
			return true;
		return false;
	}
}

bool Graph::addEdge(Vertex* vertex, Vertex* OtherVertex){
	int vID = vertex->getID();
	int oVID = OtherVertex->getID();
	int edgeID = -1;
	// si son iguales, no son adyacentes..
	if ( vID == oVID )
		return false;
	// veo que sean vertices validos
	if ( !validVertex( vID ) || !validVertex( oVID ) )
		return false;
		
	// ahora veo que tipo de relacion hay entre ellos
	if ( isNorth(vID, oVID) ){// si OtherVertex esta al norte de vertex
		edgeID = getStartingEdgeId(VERTICAL, vID);
		addEdgeFinally(edgeID, vertex, OtherVertex, VERTICAL);
		return true;
	}else{
		if ( isEast(vID, oVID) ){// si OtherVertex esta al este de vertex
			edgeID = getStartingEdgeId(HORIZONTAL, vID);
			addEdgeFinally(edgeID, vertex, OtherVertex, HORIZONTAL);
			return true;
		}else{
			if ( isWest(vID, oVID) ) {//si OtherVertex esta al oeste de vertex, entonces vertex esta al este de Other
				edgeID = getStartingEdgeId(HORIZONTAL, oVID);
				addEdgeFinally(edgeID, OtherVertex, vertex, HORIZONTAL);
				return true;
			}else{
				if ( isSouth(vID, oVID) ) {// si Other esta al sur de vertex, entonces vertes esta al norte de Other
					edgeID = getStartingEdgeId(VERTICAL, oVID);
					addEdgeFinally(edgeID, OtherVertex, vertex, VERTICAL);
					return true;
				}
			}
		}
	}
	// si no es ningun caso, devuelvo falso
	return false;
	
}

bool Graph::removeVertex(int id){
	Vertex* vertex = getVertex(id);
	if ( vertex == NULL ) return false;
	//priemro quito el vertice del grafo propiamente dicho
	graph.quitarVertice(id);
	//ahora lo quito del map y quito sus arcos, de la lista y de los vertices adyacentes
	Edge* edge = NULL;
	//con todas las aristas hago lo mismo
	edge = vertex->getNorthEdge();
	if ( edge ){//si tiene arista norte
		Vertex* otherVertex = edge->getOther(vertex);//obtengo el vertice destino
		otherVertex->setSouthEdge(NULL);//le saco la arista sur
		edges->erase( std::find(edges->begin(), edges->end(), edge) );		
	}
	
	edge = vertex->getSouthEdge();
	if ( edge ){//si tiene arista sur
		Vertex* otherVertex = edge->getOther(vertex);//obtengo el vertice destino
		otherVertex->setNorthEdge(NULL);//le saco la arista norte
		edges->erase( std::find(edges->begin(), edges->end(), edge) );// saco la arista de la lista de aristas
	}
	
	edge = vertex->getWestEdge();
	if ( edge ){//si tiene arista oeste
		Vertex* otherVertex = edge->getOther(vertex);//obtengo el vertice destino
		otherVertex->setEastEdge(NULL);//le saco la arista este
		edges->erase( std::find(edges->begin(), edges->end(), edge) );		
	}
	
	edge = vertex->getEastEdge();
	if ( edge ){//si tiene arista este
		Vertex* otherVertex = edge->getOther(vertex);//obtengo el vertice destino
		otherVertex->setWestEdge(NULL);//le saco la arista oeste
		edges->erase( std::find(edges->begin(), edges->end(), edge) );		
	}
	//elimino el vertice, y lo saco del map
	vertices.erase( vertices.find( vertex->vertice ) );
	
	delete vertex;
	
	return true;	
}

ElementList* Graph::getElementList(){
	Element* element = NULL;;
	ElementList* elements = new ElementList();
	mapVertices::iterator it;
	for ( it = vertices.begin() ; it != vertices.end() ; ++it){
		element = (it->second)->getContent();
		if ( element ){
			elements->push_back(*element);	
		}
	}
	return elements;
}


