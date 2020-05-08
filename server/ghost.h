/*cabecera de la clase Ghost*/
#ifndef __GHOST_H__
#define __GHOST_H__
#include "server/player.h"
#include "server/playeractioner.h"

/**
 * Esta clase hereda de Player, y solamente redefine el método move() para que lo que haga sea, primeramente activar el mutex, evaluar si el movimiento que va a hacer es valido, desactivar el mutex, dormir la cantidad de tiempo que deba, y luego volverlo a activar, para setear su nueva posición y arco. Para luego evaluar si se encuentra en la casa de los fantasmas, así en caso de estar muerto pueda revivir. Terminando por la desactivación del mutex.
 * Como este mutex será el mismo que utilicen todos los jugadores, no podrán estar evaluando choques si es que el fantasma cambia su posición, por lo que no habrá problemas de concurrencia.
 */
class Ghost:public Player {
  private:
  	PlayerActioner & _positioner;
	
  protected:
    ///lo q ejecuta el thread
   /// virtual void main();
   virtual void move();
  public:
  	/// 
    Ghost(const int &idPlayer, const Edge * tunnel, int initPosition, tPlayerOrientation orientation, PlayerActioner & positioner, Mutex & key);
};

#endif /* __GHOST_H__ */
