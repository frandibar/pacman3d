#include "elementmanager.h"

ElementManager::ElementManager() {
  reset();
}

ElementManager::~ElementManager() {
  reset();
}

void ElementManager::reset() {
  _mtx.lock();
  _elementsUpdate.clear();
  _mtx.unlock();
}

void ElementManager::addElement(const Element &elem) {
  _mtx.lock();
  _elementsUpdate.push_back(elem);
  _mtx.unlock();
}

//devuelve copia de listado de elementos y borra la lista interna
ElementList* ElementManager::getElementsAndWipe() {
  _mtx.lock();
  ElementList *elements = new ElementList(_elementsUpdate);
  _elementsUpdate.clear();
  _mtx.unlock();
  return elements;
}
