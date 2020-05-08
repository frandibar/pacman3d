#include "xmlmap.h"

// Para conversion numerico-string
#include "common/helper.h"

/*
    TAGS XML
*/
#define XMLTAG_MAP "map"
#define XMLTAG_VERTICES "vertices"
#define XMLTAG_EDGES "edges"
#define XMLTAG_VERTEX "vertex"
#define XMLTAG_ELEMENT "element"
#define XMLTAG_EDGE "edge"

#define XMLTAG_ATTR_HEIGHT "height"
#define XMLTAG_ATTR_WIDTH "width"
#define XMLTAG_ATTR_VERTEX_ID "id"
#define XMLTAG_ATTR_EDGE_ID "id"
#define XMLTAG_ATTR_ELEMENT_TYPE "type"
#define XMLTAG_ATTR_DIRECTION "orientation"
#define XMLTAG_ATTR_VERTEX_FROM "vertexFrom"
#define XMLTAG_ATTR_VERTEX_TO "vertexTo"
#define XMLTAG_ATTR_EDGE_STATUS "status"

#define XMLTAG_VALUE_PILL "pill"
#define XMLTAG_VALUE_WALL "wall"
#define XMLTAG_VALUE_PACMANSTART "pacmanstart"
#define XMLTAG_VALUE_GHOSTHOUSE "ghosthouse"
#define XMLTAG_VALUE_BONUS "bonus"
#define XMLTAG_VALUE_POWERUP "powerup"
#define XMLTAG_VALUE_NONE "none"

#define XMLTAG_VALUE_DIRECTION_NORTH "N"
#define XMLTAG_VALUE_DIRECTION_SOUTH "S"
#define XMLTAG_VALUE_DIRECTION_EAST "E"
#define XMLTAG_VALUE_DIRECTION_WEST "W"

#define XMLTAG_VALUE_EDGE_STATUS_ON "on"
#define XMLTAG_VALUE_EDGE_STATUS_OFF "off"



XmlMap::XmlMap() {
	// Nada por ahora...
}

XmlMap::~XmlMap() {
	// Nada por ahora...
}


PacManMap * XmlMap::Load(const string & xmlfile) throw (string) {

	const string ERRORMSG_HEADER = "Unable to open " + xmlfile + ": ";
	
	// Ante todo validamos que el archivo exista:
	if (! fileExists(xmlfile)) {
		string errorMsg = ERRORMSG_HEADER + "File does not exist.";
		throw errorMsg;
	}
	
	// Cargamos el archivo XML a un TinyXML DOM:
	TiXmlDocument xmldoc(xmlfile.c_str());
	if (! xmldoc.LoadFile()) {
		string errorMsg = ERRORMSG_HEADER + "Invalid XML format.";
		throw errorMsg;
	}

	// Ubicamos el elemento root y en base a el construimos el Mapa
	TiXmlHandle handleDoc(& xmldoc);
	TiXmlElement * mapXmlElement = handleDoc.FirstChildElement().Element();
	
	try {
		return BuildMap(mapXmlElement);
	} catch(string e) {
		string errormsg = ERRORMSG_HEADER + e;
		throw errormsg;
	}
}


void XmlMap::Save(const string & xmlfile, PacManMap & pacManMap) throw (string) {
	
	// xmlfile must be a valid filename:
	if (xmlfile == "") {
		string errorMsg = "invalid destination path.";
		throw errorMsg;
	}
	
	// CREATE XML DOCUMENT:
	TiXmlDocument doc;  
 	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "UTF-8", "" );  
	doc.LinkEndChild( decl ); 

	// Comentarios para ROOT ELEMENT (XMLTAG_MAP):
	TiXmlComment * comment = new TiXmlComment();
	comment->SetValue("PacMan3D Map");
	doc.LinkEndChild( comment );
	// comment = new TiXmlComment();
	// comment->SetValue("...");
	// doc.LinkEndChild( comment ); 
	
	// ROOT ELEMENT (XMLTAG_MAP):
	TiXmlElement * root = BuildXmlMap(pacManMap);
	
	doc.LinkEndChild(root);

	// SAVE XML DOCUMENT:
	if (! doc.SaveFile( xmlfile )) {
		// Delete all the children of the document. Does not affect 'this'.
		doc.Clear();		
		string errorMsg;
		errorMsg = "Unable to save map to XML file " + xmlfile;
		throw errorMsg;
	}
	
	// Delete all the children of the document. Does not affect 'this'.
	doc.Clear();	
	
}


TiXmlElement * XmlMap::BuildXmlMap(PacManMap & pacManMap) throw (string) {
	
	// ROOT ELEMENT (XMLTAG_MAP):
	TiXmlElement * root = new TiXmlElement( XMLTAG_MAP );
	root->SetAttribute(XMLTAG_ATTR_HEIGHT, pacManMap.getHeight());
	root->SetAttribute(XMLTAG_ATTR_WIDTH, pacManMap.getWidth());
	
	// SECCION Vertices:
	TiXmlElement * verticesSection = new TiXmlElement( XMLTAG_VERTICES );
	
	VertexList * vertexList = pacManMap.getVertices();
	for(VertexList::iterator it = vertexList->begin(); it != vertexList->end(); ++it) {
		// El vertex tiene el atributo ID y un elemento <element> en su interior:
		TiXmlElement * vertex = new TiXmlElement( XMLTAG_VERTEX );
		vertex->SetAttribute(XMLTAG_ATTR_VERTEX_ID, (*it)->getID());
		TiXmlElement * xmlelement = new TiXmlElement( XMLTAG_ELEMENT );
		string type;
		Element * element = (*it)->getContent();
		bool setOrientation = false;
		if(element == NULL) {
			type = XMLTAG_VALUE_NONE;
		}
		else if(element->getType() == PACMAN_START) {
			type = XMLTAG_VALUE_PACMANSTART;
			setOrientation = true;
		} 
		else if(element->getType() == GHOST_HOUSE) {
			type = XMLTAG_VALUE_GHOSTHOUSE;
			setOrientation = true;
		}
		else if(element->getType() == POWER_UP) {
			type = XMLTAG_VALUE_POWERUP;
		}
		else if(element->getType() == BONUS) {
			type = XMLTAG_VALUE_BONUS;
		}
		else if(element->getType() == PILL) {
			type = XMLTAG_VALUE_PILL;
		}
		else {
			string errorMsg = "Element type not supported for vertex ID " + StringConverter<int>((*it)->getID());
			delete vertexList;
			throw errorMsg;
		}
		xmlelement->SetAttribute(XMLTAG_ATTR_ELEMENT_TYPE, type);
		if (setOrientation) {
			string orientation;
			if (element->getOrientation() == NORTH) {
				orientation = XMLTAG_VALUE_DIRECTION_NORTH;
			}
			else if (element->getOrientation() == SOUTH) {
				orientation = XMLTAG_VALUE_DIRECTION_SOUTH;
			}
			else if (element->getOrientation() == EAST) {
				orientation = XMLTAG_VALUE_DIRECTION_EAST;
			}
			else if (element->getOrientation() == WEST) {
				orientation = XMLTAG_VALUE_DIRECTION_WEST;
			}
			else {
				string errorMsg = "Orientation value not supported for vertex ID " + StringConverter<int>((*it)->getID());
				delete vertexList;
				throw errorMsg;
			}
			xmlelement->SetAttribute(XMLTAG_ATTR_DIRECTION, orientation);
		}
		vertex->LinkEndChild(xmlelement);
		
		// Agregamos el Vertex a la seccion de vertices:
		verticesSection->LinkEndChild(vertex);
	}
	
	root->LinkEndChild(verticesSection);

	// SECCION EDGES:
	TiXmlElement * edgesSection = new TiXmlElement( XMLTAG_EDGES );
	
	EdgeList * edgeList = pacManMap.getEdges();
	for(EdgeList::iterator it = edgeList->begin(); it != edgeList->end(); ++it) {
		// Cada edge tiene un atributo ID los atributos vertexFrom y vertexTo:
		TiXmlElement * edge = new TiXmlElement( XMLTAG_EDGE );
		edge->SetAttribute(XMLTAG_ATTR_EDGE_ID, (*it)->getID());
		edge->SetAttribute(XMLTAG_ATTR_VERTEX_FROM, (*it)->getVertex0()->getID());
		edge->SetAttribute(XMLTAG_ATTR_VERTEX_TO  , (*it)->getVertex1()->getID());
		
		// Agregamos el Edge a la seccion de Edges:
		edgesSection->LinkEndChild(edge);
	}
	
	root->LinkEndChild(edgesSection);
	
	delete edgeList;
	delete vertexList;
	
	// DEVOLVEMOS root (XMLTAG_MAP):
	return root;
}

PacManMap * XmlMap::BuildMap(TiXmlElement * xmlMapElement) throw (string) {

	// Antes que nada, validamos que el XMLelement sea valido:
	if (! xmlMapElement || xmlMapElement->ValueStr().compare(XMLTAG_MAP) != 0) {
		string errormsg = "Invalid format (root element must be <map>)";
		throw errormsg;
	}	

	// Determinamos altura y ancho del MAP (atributos):
	int height;
	int width;
	if (xmlMapElement->Attribute(XMLTAG_ATTR_HEIGHT, & height) == NULL) {
		string errormsg = "Invalid format (Map has no " + string(XMLTAG_ATTR_HEIGHT) + " attribute)";
		throw errormsg;
	}
	if (xmlMapElement->Attribute(XMLTAG_ATTR_WIDTH, & width) == NULL) {
		string errormsg = "Invalid format (Map has no " + string(XMLTAG_ATTR_WIDTH) + " attribute)";
		throw errormsg;
	}
	
	// Un Handle para el Map:
	TiXmlHandle mapHandle(xmlMapElement);
	
	// Determinamos la seccion VERTICES (si no existe, no podemos seguir):
	TiXmlHandle verticesXmlSection = mapHandle.FirstChild( XMLTAG_VERTICES );
	if (! verticesXmlSection.ToElement()) {
		string errormsg = "Invalid format (Map has no " + string(XMLTAG_VERTICES) + " section)";
		throw errormsg;
	}
	
	// Determinamos tmb la seccion EDGES (si no existe no podemos seguir):
	TiXmlHandle edgesXmlSection = mapHandle.FirstChild( XMLTAG_EDGES );
	if (! edgesXmlSection.ToElement()) {
		string errormsg = "Invalid format (Map has no " + string(XMLTAG_EDGES) + " section)";
		throw errormsg;
	}
	
	// Teniendo ya las dos secciones ubicadas, comenzamos a armar el Grafo !
	PacManMap * graph = new PacManMap(width, height);
	ElementList * elementList = new ElementList();
	
	
	// PRIMERO RECORREMOS LOS VERTICES, Y LOS AGREGAMOS AL GRAFO 
	// (y agregamos tmb los Elementos a ElementList)
	
	TiXmlElement * xmlVertex = verticesXmlSection.FirstChild( XMLTAG_VERTEX ).Element();
	
	// Debe haber al menos un vertice !
	if (! xmlVertex) {
		delete graph;
		delete elementList;
		string errormsg = "Invalid format (Map must have at least one vertex)";
		throw errormsg;
	}
	
	addVerticesAndElements(xmlVertex, graph, elementList);

	// LUEGO RECORREMOS LAS ARISTAS Y LAS AGREGAMOS AL GRAFO
	
	TiXmlElement * xmlEdge = edgesXmlSection.FirstChild( XMLTAG_EDGE ).Element();

	// Debe haber al menos un edge !
	if (! xmlEdge) {
		delete graph;
		delete elementList;
		string errormsg = "There must be at least one edge.";
		throw errormsg;
	}
	
	try {
		addEdges(xmlEdge, graph);
	} catch(string e) {
		delete graph;
		delete elementList;
		throw e;
	}
	
	// Por ultimo, creamos el PacManMAP y lo devolvemos:
	return graph;
	
}



// Private use only:
void XmlMap::getOrientationAttribute(TiXmlElement * element, tOrientation & orientation) throw (string) {
	
	if (! element) {
		string errormsg = "null element.";
		throw errormsg;
	}
	
	if (element->Attribute(XMLTAG_ATTR_DIRECTION) == NULL) {
		string errormsg = "Element has no " + string(XMLTAG_ATTR_DIRECTION) + " attribute.";
		throw errormsg;
	}	
	string orientationAttr = element->Attribute(XMLTAG_ATTR_DIRECTION);
	if (orientationAttr.compare(XMLTAG_VALUE_DIRECTION_NORTH) == 0) {
		orientation = NORTH;
	}
	else if (orientationAttr.compare(XMLTAG_VALUE_DIRECTION_SOUTH) == 0) {
		orientation = SOUTH;
	}
	else if (orientationAttr.compare(XMLTAG_VALUE_DIRECTION_EAST) == 0) {
		orientation = EAST;
	}
	else if (orientationAttr.compare(XMLTAG_VALUE_DIRECTION_WEST) == 0) {
		orientation = WEST;
	}
	else {
		// Valor invalido para Orientation:
		string errormsg = "Invalid value for " + string(XMLTAG_ATTR_DIRECTION) + " attribute.";
		throw errormsg;
	}
	
}


// Private use only:
void XmlMap::addVerticesAndElements(TiXmlElement * xmlVertex, Graph * graph, ElementList * elementList) throw (string) {

	for( ; xmlVertex; xmlVertex = xmlVertex->NextSiblingElement()) {
		
		// Determinamos el ID del vertice
		int vertexId;
		if (xmlVertex->Attribute(XMLTAG_ATTR_VERTEX_ID, & vertexId) == NULL) {
			string errormsg = "Some vertex has no " + string(XMLTAG_ATTR_VERTEX_ID) + " Attribute.";
			throw errormsg;
		}
		// Determinamos el ELEMENTO PARA EL VERTICE:
		TiXmlElement * vertexElement = xmlVertex->FirstChildElement(XMLTAG_ELEMENT);
		if (! vertexElement) {
			string errormsg = "Vertex " + StringConverter<int>(vertexId) + " has no Element.";
			throw errormsg;
		}

		// DIFERENTES ACCIONES SEGUN EL TIPO DE ELEMENTO CONTENIDO EN EL VERTICE:
		if (vertexElement->Attribute(XMLTAG_ATTR_ELEMENT_TYPE) == NULL) {
			string errormsg = "Vertex " + StringConverter<int>(vertexId) + " Element has no " + XMLTAG_ATTR_ELEMENT_TYPE + " Attribute.";
			throw errormsg;
		}	
		string elementType = vertexElement->Attribute(XMLTAG_ATTR_ELEMENT_TYPE);
		
		bool addToGraph = false;
		bool addToElementList = false;
		Element * element = 0;
		
		if (elementType.compare(XMLTAG_VALUE_WALL) == 0) {
			// Si es wall no lo agregamos al Grafo...
			// Pues el grafo espera solo vertices q formen caminos
		}
		else if (elementType.compare(XMLTAG_VALUE_PILL) == 0) {
			// Si es pill lo agregamos solo al grafo
			// La orientacion no tiene sentido aca (ponemos NORTH por poner algo)
			element = new Element(vertexId, PILL, NORTH);
			addToGraph = true;
		}
		else if (elementType.compare(XMLTAG_VALUE_BONUS) == 0) {
			// Si es Bonus, lo agregamos al grafo y al elementList
			// La orientacion no tiene sentido aca (ponemos NORTH por poner algo)
			element = new Element(vertexId, BONUS, NORTH);
			addToGraph = true;
			addToElementList = true;
		}
		else if (elementType.compare(XMLTAG_VALUE_GHOSTHOUSE) == 0) {
			// Si es GhostHouse, lo agregamos al grafo y al elementList
			// Pero previamente debemos determinar la orientacion:
			tOrientation orientation;
			try {
				getOrientationAttribute(vertexElement, orientation);
			} catch(string e) {
				string errormsg = "Error getting orientation attribute for vertex " + StringConverter<int>(vertexId) + " element:\n" + e;
				throw errormsg;
			}
			element = new Element(vertexId, GHOST_HOUSE, orientation);
			addToGraph = true;
			addToElementList = true;
		}
		else if (elementType.compare(XMLTAG_VALUE_PACMANSTART) == 0) {
			// Si es PacManStart, lo agregamos al grafo y al elementList
			// Pero previamente debemos determinar la orientacion:
			tOrientation orientation;
			try {
				getOrientationAttribute(vertexElement, orientation);
			} catch(string e) {
				string errormsg = "Error getting orientation attribute for vertex " + StringConverter<int>(vertexId) + " element:\n" + e;
				throw errormsg;
			}
			element = new Element(vertexId, PACMAN_START, orientation);
			addToGraph = true;
			addToElementList = true;
		}
		else if (elementType.compare(XMLTAG_VALUE_POWERUP) == 0) {
			// Si es PowerUp, lo agregamos al grafo y al elementList
			// La orientacion no tiene sentido aca (ponemos NORTH por poner algo)
			element = new Element(vertexId, POWER_UP, NORTH);
			addToGraph = true;
			addToElementList = true;
		}
		else if (elementType.compare(XMLTAG_VALUE_NONE) == 0) {
			// Si es none NO CREAMOS Element. 
			// solo agregamos al grafo un vertice con elemento Nulo
			addToGraph = true;
		}
		else {
			// Cualquier otro valor es invalido:
			string errormsg = "Invalid Element Type for vertex " + StringConverter<int>(vertexId) + " Element.";
			throw errormsg;
		}
		
		if(addToGraph) {
			graph->addVertex(element, vertexId);
		}
		if(addToElementList) {
			elementList->push_back(*element);
		}
		
	} // end for(...)

}


// Private use only:
void XmlMap::addEdges(TiXmlElement * xmlEdge, Graph * graph) throw (string) {
	
	for( ; xmlEdge; xmlEdge = xmlEdge->NextSiblingElement()) {
		
		// Cada arista tiene los sig atributos:
		// a) id (excluyente)
		// b) vertexFrom (excluyente)
		// c) vertexTo (excluyente)
		// d) status ON/OFF (opcional): default ON
		//	  Si el status es OFF, la arista NO SE AGREGA

		// EDGE ID:
		int id;
		if (xmlEdge->Attribute(XMLTAG_ATTR_EDGE_ID, & id) == NULL) {
			string errormsg = "Some edge has no " + string(XMLTAG_ATTR_EDGE_ID) + " attribute.";
			throw errormsg;
		}
		
		// Vertex From and To:
		int vertexFrom;
		int vertexTo;
		if (xmlEdge->Attribute(XMLTAG_ATTR_VERTEX_FROM, & vertexFrom) == NULL) {
			string errormsg = "Edge " + StringConverter<int>(id) + " has no " + XMLTAG_ATTR_VERTEX_FROM + " attribute.";
			throw errormsg;
		}
		if (xmlEdge->Attribute(XMLTAG_ATTR_VERTEX_TO, & vertexTo) == NULL) {
			string errormsg = "Edge " + StringConverter<int>(id) + " has no " + XMLTAG_ATTR_VERTEX_TO + " attribute.";
			throw errormsg;
		}

		// Status ON/OFF (opcional: si no existe se toma status ON)
		// PERO SI EL STATUS ES OFF, SE OMITE LA ARISTA (no se la agrega)
		if (xmlEdge->Attribute(XMLTAG_ATTR_EDGE_STATUS) != NULL) {
			string status = xmlEdge->Attribute(XMLTAG_ATTR_EDGE_STATUS);
			if (status.compare(XMLTAG_VALUE_EDGE_STATUS_OFF) == 0) {
				continue;
			} 
			else if (status.compare(XMLTAG_VALUE_EDGE_STATUS_ON) == 0) {
				// nada...
			}
			else {
				string errormsg = "Invalid value for Edge " + string(XMLTAG_ATTR_EDGE_STATUS) + " attribute (must be ON/OFF).";
				throw errormsg;
			}
			
		}	
		
		// Si llegamos aca, la Arista SE AGREGA AL GRAFO:
		Vertex * vertex0 = graph->getVertex(vertexFrom);
		if (vertex0 == NULL) {
			string errormsg = "Error adding edge " + StringConverter<int>(id) + " to Graph: Vertex " + StringConverter<int>(vertexFrom) + " does not exist.";
			throw errormsg;
		}
		Vertex * vertex1 = graph->getVertex(vertexTo);
		if (vertex1 == NULL) {
			string errormsg = "Error adding edge " + StringConverter<int>(id) + " to Graph: Vertex " + StringConverter<int>(vertexTo) + " does not exist.";
			throw errormsg;
		}
		if (! graph->addEdge(id, vertex0, vertex1) ) {
			string errormsg = "Error adding edge " + StringConverter<int>(id) + " to Graph.";
			throw errormsg;
		}
		
	} // end for(...)
	
}
