/**
 * @file xmlworld.h
 * 
 * XmlWorld es una clase de tipo servicio (no se instancia, solo brinda metodos static)
 * 
 * El servicio brindado es de serializacion-deserializacion de un World 
 * en formato XML
 * 
 * */
#ifndef __XMLWORLD_H__
#define __XMLWORLD_H__

/// Level (includes Graph, Element and ElementList):
#include "common/level.h"
#include "common/world.h"

#include <string>
using std::string;

/// TINYXML:
#include "common/tinyxml/tinyxml.h"

/**
 * XmlWorld es una clase de tipo servicio (no se instancia, solo brinda metodos static)
 * 
 * El servicio brindado es de serializacion-deserializacion de un World 
 * en formato XML
 * 
 * @sa XmlMap
 */
class XmlWorld {
  
  private:
  	// XmlWorld NO necesita instanciarse.
  	XmlWorld();
  	    
  public:
  	static const tLevelSpeed SPEED_BY_DEFAULT = SPEED_LOW; 
    /// levanta los datos del xml (ruta completa) hacia una instancia de World
    static void Load(const string & xmlfile, World & world) throw (string);
    /// levanta los datos del xml (ruta completa) hacia una instancia de MapFileList
    static void Load(const string & xmlfile, MapFileList & mapFileList) throw (string);
    /// Guarda mapFileList en formato xml al archivo especificado (ruta completa):
    static void Save(const string & xmlfile, const MapFileList & mapFileList) throw (string); 
};

#endif /* __XMLWORLD_H__ */
