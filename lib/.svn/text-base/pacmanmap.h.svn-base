/*
 * PacManMap.h
 */
#ifndef __PACMANMAP_H__
#define __PACMANMAP_H__

#include "common/graph.h"
#include "common/element.h"
#include "common/graphhandler.h"
#include "common/grapherrors.h"

/**
 * PacManMap representa un Mapa de PacMan
 * En esencia un Mapa de PacMan (PacManMap) es un Grafo
 * Por tanto, PacManMap extiende la clase Graph
 * */
class PacManMap: public Graph {
	
private:
	PacManMap (const PacManMap&);
	PacManMap& operator=(const PacManMap&);
	
public:
	///
  	PacManMap(int width, int height): Graph(width, height) {}
  	
  	///
  	virtual ~PacManMap();
  	
  	///
  	bool isValid();
  	
  	///
  	bool isValid(string & errormsg);
};

#endif /*__PACMANMAP_H__*/
