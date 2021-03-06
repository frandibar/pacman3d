#pragma once

#include "pacmanmap.h"
#include "utilities.h"

/**
 * Un nivel (Level) esta formado basicamente por un PacManMap y atributos adicionales
 * propios del nivel. Por ahora, la velocidad del nivel
 * 
 * @sa PacManMap
 * @sa tLevelSpeed
 */
class Level {
  
  private:
    PacManMap * _map; // Mapa del nivel
    tLevelSpeed _speed; // Velocidad del nivel
    
    //por las dudas los ocultamos
    Level(const Level&);
    Level& operator=(const Level&);
    
  public:
  	/// constuctor
    Level(PacManMap * pacManMap, tLevelSpeed speed);
    
    ///destructor
    ~Level();
    
    /// devuelve grafo asociado
    Graph * getGraph() const {return _map;}
    
    /// devuelve elementos (DESTRUIR LA LISTA RECIBIDA LUEGO DE SER USADA)
    ElementList * getElementList() const {return _map->getElementList();}
    
    /// devuelve velocidad
    tLevelSpeed getSpeed() const {return _speed;}
    
    /// devuelve el mapa asociado al nivel
    PacManMap * getPacManMap() const {return _map;}
    
};

