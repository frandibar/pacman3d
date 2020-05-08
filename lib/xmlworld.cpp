//definicion de metodos de clase XmlWorld
#include "common/xmlworld.h"

// XmlMap (serializacion-deserializacion de PacManMap por XML):
#include "common/xmlmap.h"

// Para Conversion de numerico a String
#include "common/helper.h" 

/*
    TAGS XML
*/
#define XMLTAG_WORLD "world"
#define XMLTAG_LEVEL_COLLECTION "levels"
#define XMLTAG_LEVEL "level"
#define XMLTAG_MAP "map"

#define XMLTAG_ATTR_SPEED "speed"

#define XMLTAG_VALUE_SPEED_LOW "low"
#define XMLTAG_VALUE_SPEED_MEDIUM "medium"
#define XMLTAG_VALUE_SPEED_HIGH "high"



XmlWorld::XmlWorld() {
	// Nada por ahora...
}

void XmlWorld::Save(const string & xmlfile, const MapFileList & mapFileList) throw (string) {

	// xmlfile must be a valid filename:
	if (xmlfile == "") {
		string errorMsg = "invalid destination path.";
		throw errorMsg;
	}

	// mapFileList must have at least one element:
	// A pedido de John dejamos esta validcion sin efecto
	/*
	if (mapFileList.size() == 0) {
		string errorMsg = "mapFileList must have at least one element.";
		throw errorMsg;
	}
	*/

	// CREATE XML DOCUMENT:
	TiXmlDocument doc;  
 	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "UTF-8", "" );  
	doc.LinkEndChild( decl ); 

	// Comentarios para ROOT ELEMENT (XMLTAG_WORLD):
	TiXmlComment * comment = new TiXmlComment();
	comment->SetValue("PacMan3D World");
	doc.LinkEndChild( comment );
	// comment = new TiXmlComment();
	// comment->SetValue("...");
	// doc.LinkEndChild( comment ); 

	// ROOT ELEMENT (XMLTAG_WORLD):
	TiXmlElement * root = new TiXmlElement( XMLTAG_WORLD );
	doc.LinkEndChild(root);

	// AGREGAMOS NIVELES (estan ordenados):
	TiXmlElement * xmlLevelCollection = new TiXmlElement( XMLTAG_LEVEL_COLLECTION );
	
	int levelNbr = 1;
	for(MapFileList::const_iterator it = mapFileList.begin(); it != mapFileList.end(); ++it) {
		
		TiXmlElement * xmlLevel = new TiXmlElement( XMLTAG_LEVEL );
		
		// Atributo SPEED:
		string xmlspeed;
		if ((*it).speed == SPEED_LOW) {
			xmlspeed = XMLTAG_VALUE_SPEED_LOW;
		}
		else if ((*it).speed == SPEED_MEDIUM) {
			xmlspeed = XMLTAG_VALUE_SPEED_MEDIUM;
		}
		else if ((*it).speed == SPEED_HIGH) {
			xmlspeed = XMLTAG_VALUE_SPEED_HIGH;
		}
		else {
			// Delete all the children of the document. Does not affect 'this'.
			doc.Clear();
			string errorMsg = "Speed value not supported for level number " + StringConverter<int>(levelNbr);
			throw errorMsg;
		}
		xmlLevel->SetAttribute(XMLTAG_ATTR_SPEED, xmlspeed);

		// EL MAPA ASOCIADO AL NIVEL ES UN PATH (el path q indica el archivo *.map).
		// El path no debe ser vacio!
		if ((*it).path == "") {
			// Delete all the children of the document. Does not affect 'this'.
			doc.Clear();
			string errorMsg = "Invalid Map Path for level number " + StringConverter<int>(levelNbr);
			throw errorMsg;
		}
		
		// El map es un Attribute:
		xmlLevel->SetAttribute(XMLTAG_MAP, (*it).path);
		
		// Agregamos Level al collection de levels		
		xmlLevelCollection->LinkEndChild(xmlLevel);
		
		levelNbr ++;
	}

	root->LinkEndChild(xmlLevelCollection);

	// SAVE XML DOCUMENT:
	if (! doc.SaveFile( xmlfile )) {
		// Delete all the children of the document. Does not affect 'this'.
		doc.Clear();
		string errorMsg;
		errorMsg = "Unable to save world to XML file " + xmlfile;
		throw errorMsg;
	}
	
	// Delete all the children of the document. Does not affect 'this'.
	doc.Clear();

}



void XmlWorld::Load(const string & xmlfile, MapFileList & mapFileList) throw (string) {
	
	const string ERRORMSG_HEADER = "Unable to open " + xmlfile + ": ";
	
	// Ante todo validamos que el archivo exista:
	if (! fileExists(xmlfile)) {
		string errorMsg = ERRORMSG_HEADER + "File does not exist.";
		throw errorMsg;
	}	
	
	// Cargamos el archivo XML a un TinyXML DOM:
	TiXmlDocument xmldoc(xmlfile.c_str());
	if (! xmldoc.LoadFile()) {
		string errorMsg = ERRORMSG_HEADER + "Invalid format.";
		throw errorMsg;
	}
	
	// Ubicamos el elemento root: debe ser world!
	TiXmlHandle handleDoc(& xmldoc);
	TiXmlElement * worldXmlElement = handleDoc.FirstChildElement().Element();
	if (! worldXmlElement || worldXmlElement->ValueStr().compare(XMLTAG_WORLD) != 0) {
		string errorMsg = ERRORMSG_HEADER + "Invalid format (root element must be <" + XMLTAG_WORLD + ">)";
		throw errorMsg;
	}
	TiXmlHandle worldHandle(worldXmlElement);
	
	// Ubicamos el levels Collection:
	TiXmlElement * xmlLevelCollection = worldHandle.FirstChild( XMLTAG_LEVEL_COLLECTION ).Element();
	if (! xmlLevelCollection) {
		string errorMsg = ERRORMSG_HEADER + "Invalid format (there is no <" + XMLTAG_LEVEL_COLLECTION + "> element!)";
		throw errorMsg;
	}
	TiXmlHandle levelCollectionHandle(xmlLevelCollection);
	
	// Luego recorremos los levels de la collection (debe haber al menos un level!)
	// Para cada level del XML creamos un tMapFile y lo agregamos a mapFileList
	// Tener en cuenta que cada Level debe proporcionar
	// a) un atributo MAP PATH (excluyente)
	// b) un atributo speed (opcional) DEFAULT ver SPEED_BY_DEFAULT
	
	TiXmlElement * levelXmlElement = levelCollectionHandle.FirstChild( XMLTAG_LEVEL ).Element();
	
	// En realidad si no hay levels no importa.
	/*
	if (! levelXmlElement) {
		string errorMsg = ERRORMSG_HEADER + "Invalid format (there are no <" + XMLTAG_LEVEL + "> elements!)";
		throw errorMsg;
	}
	*/
	
	int levelnumber = 1;
	
	for( ; levelXmlElement; levelXmlElement = levelXmlElement->NextSiblingElement()) {

		// Determinamos el atributo PATH del Map asociado al Level (debe existir !)
		if (levelXmlElement->Attribute(XMLTAG_MAP) == NULL) {
			string errorMsg = ERRORMSG_HEADER + "Invalid format (Level element no. " + StringConverter<int>(levelnumber) + " has no " + XMLTAG_MAP + " attribute)";
			throw errorMsg;
		}
		string mapPathAttr = levelXmlElement->Attribute(XMLTAG_MAP);
		if (mapPathAttr == "") {
			string errorMsg = ERRORMSG_HEADER + "Invalid format (At Level element no. " + StringConverter<int>(levelnumber) + ": " + XMLTAG_MAP + " attribute is blank)";
			throw errorMsg;
		}
		
		// Determinamos el atrbuto speed del nivel 
		// (podria no existir, en ese caso damos un default):
		tLevelSpeed speed;
		
		if (levelXmlElement->Attribute(XMLTAG_ATTR_SPEED) == NULL) {
			speed = SPEED_BY_DEFAULT;
		} else {
			string speedAttr = levelXmlElement->Attribute(XMLTAG_ATTR_SPEED);
			if (speedAttr.compare(XMLTAG_VALUE_SPEED_LOW) == 0) {
				speed = SPEED_LOW;
			}
			else if (speedAttr.compare(XMLTAG_VALUE_SPEED_MEDIUM) == 0) {
				speed = SPEED_MEDIUM;
			}
			else if (speedAttr.compare(XMLTAG_VALUE_SPEED_HIGH) == 0) {
				speed = SPEED_HIGH;
			}
			else {
				// Valor invalido para el atributo speed:
				string errorMsg = ERRORMSG_HEADER + "Invalid format (At Level element no. " + StringConverter<int>(levelnumber) + ": invalid value for " + XMLTAG_ATTR_SPEED + " attribute)";
				throw errorMsg;
			}			
		}

		
		// Y por ultimo, construimos el tMapFile y lo agregamos al list:
		tMapFile tmf;
		tmf.path = mapPathAttr;
		tmf.speed = speed;
		mapFileList.push_back(tmf);
		
		levelnumber++;
		
	} // end for(...)
	
	
	// LISTO !
	
}


//carga xml a memoria
void XmlWorld::Load(const string & xmlfile, World & world) throw (string) {
	
	const string ERRORMSG_HEADER = "Unable to load World from " + xmlfile + ":\n";
	
	// Hacemos el Load del archivo para obtener la lista de MapFile
	// (ante error, tira exception: string)
	MapFileList mapFileList;
	XmlWorld::Load(xmlfile, mapFileList);
	
	// Para cada elemento de la lista, creamos el mapa y el nivel !
	// Debe haber al menos un elemento !
	if (mapFileList.size() == 0) {
		string errormsg = ERRORMSG_HEADER + "There must be at least one level.";
		throw errormsg;
	}
	int levelNumber = 1;
	for(MapFileList::iterator it = mapFileList.begin(); it != mapFileList.end(); ++it) {
		try {
			PacManMap * pacManMap = XmlMap::Load((*it).path);
			// Antes de agregar el nivel, validamos que el mapa sea valido !
			string err;
			if (! pacManMap->isValid(err))
				throw err; // esto es medio feo! tirar una exception para luego atraparla abajo =(
			world.addLevel(new Level(pacManMap, (*it).speed));
		} catch(string e) {
			string errormsg = ERRORMSG_HEADER + "Error trying to load map for level no. " + StringConverter<int>(levelNumber) + " (" + e + ")";
			throw errormsg;
		}
		++ levelNumber;
	}
	
	// LISTO !
}
