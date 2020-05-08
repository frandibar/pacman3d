/**
 * @file world.h
 * */
#ifndef __WORLD_H__
#define __WORLD_H__
#include "common/level.h"
#include <vector>

/// Lista de niveles
typedef std::vector<Level*> LevelList;

/**Clase para agrupar intancias de Level en un mundo
 * No es más que una lista de niveles (Levels), con la que se pueden agregar niveles, y pedir la lista de los mismos.
 * */
class World {
  private:
    LevelList *_levels;
    //los ocultamos por las dudas
    World(const World& world);
    World& operator=(const World& world);
  public:
  	/// constructor
    World();
    /// destructor
    ~World();
    /// agrega un nivel al mundo
    void addLevel(Level* level);
    /// devuelve la lista de niveles
    LevelList* getLevels() const {return _levels;}
};
#endif /* __WORLD_H__ */
