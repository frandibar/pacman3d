#include "common/world.h"

World::World() {
  _levels = new LevelList;
}

World::~World(){
  for (LevelList::iterator it = _levels->begin();it != _levels->end();it++)
    delete *it;
  _levels->clear();
  delete _levels;
}


void World::addLevel(Level* level){
  _levels->push_back(level);
}
