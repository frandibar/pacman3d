/*
 * @file xmlmap.h
 * 
 * XmlMap es una clase de tipo servicio (no se instancia, solo brinda metodos static)
 * 
 * El servicio brindado es de serializacion-deserializacion de un PacManMap 
 * en formato XML
 * 
 * */
#pragma once


/// PacManMap (includes Graph, Element and ElementList):
#include "pacmanmap.h"

#include <string>
using std::string;

/// TINYXML:
#include "lib/tinyxml/tinyxml.h"

/**
 * XmlMap es una clase de tipo servicio (no se instancia, solo brinda metodos static)
 * 
 * El servicio brindado es de serializacion-deserializacion de un PacManMap 
 * en formato XML
 * 
 * @sa XmlWorld
 */
class XmlMap {

private:

	// XmlMap NO necesita instanciarse.
	XmlMap();
	
	// MOdularizacion interna
	static void getOrientationAttribute(TiXmlElement * element, tOrientation & orientation) throw (string);
	static void addVerticesAndElements(TiXmlElement * xmlVertex, Graph * graph, ElementList * elementList) throw (string);
	static void addEdges(TiXmlElement * xmlEdge, Graph * graph) throw (string);

public:

	virtual ~XmlMap();
    /// carga xml a memoria
    static PacManMap * Load(const string & xmlfile) throw (string);
    /// Construye un PacManMap a partir de un TiXMLElement que es la representacion del pacmanMap en XML:
    static PacManMap * BuildMap(TiXmlElement * xmlMapElement) throw (string);
	/// Guarda pacmanMap en formato xml al archivo especificado en el destino especificado 
	static void Save(const string & xmlfile, PacManMap & pacManMap) throw (string);	
	/// Construye un TiXMLElement a partir de un PacManMap:
	static TiXmlElement * BuildXmlMap(PacManMap & pacManMap) throw (string);
	
	
};

