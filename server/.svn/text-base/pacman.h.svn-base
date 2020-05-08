/*cabecera de la clase Pacman*/
#ifndef __PACMAN_H__
#define __PACMAN_H__
#include "server/player.h"
#include "server/playeractioner.h"

/**
 * Esta clase hereda de Player e incorpora algunos atributos, como por ejemplo un bool para decir si está en modo power up, y una referencia a un elemento comido.
 * Implementa el método move() haciendo primeramente lo mismo que ghost, pero luego al momento de terminar el movimiento, evalúa si en esa posición no hay un elemento para comer. Si lo hay, lo setea como elemento comido, y termina.
 */
class Pacman:public Player {
  private:
  	PlayerActioner & _positioner;
  	bool	_poweredUp;
  	Element* _eatenElement;
  protected:
    ///lo q ejecuta el thread
    virtual void move();
    
  public:
  	/// 
    Pacman(const int &idPlayer, const Edge * tunnel, int initPosition, tPlayerOrientation orientation, PlayerActioner & positioner, Mutex & key);
    ///devuelve si pacman esta en estado power up
    inline bool isPoweredUp() const{
    	return _poweredUp;
    }
    
    ///setea al pacman en powerUp
    inline void setPowerUp(bool powerUp){
    	_poweredUp = powerUp;
    }
    ///devuelve el elemento comido
    inline Element* getEatenElement() const{
    	return _eatenElement;	
    }
    
    ///setea elemento comido en nulo
    inline void resetEatenElement(){
    	_eatenElement = NULL;	
    }
};

#endif /* __PACMAN_H__ */
