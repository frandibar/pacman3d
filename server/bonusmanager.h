#ifndef __BONUSMANAGER_H__
#define __BONUSMANAGER_H__
#include "lib/mutex.h"

/**
 * Clase para administrar el elemento bonus
 * El bonus se crea, tiene un tiempo de vida, y luego expira
 * Metodos protegidos por mutex
 */
class BonusManager {
  private:
    static const int MAX_NUM;//maxima cantidad de bonus
    static const int MIN_TIME;
    static const int MAX_TIME;
    static const int EXPIRE_TIME;
    
    int _bonusCounter;
    bool _triggered;
    bool _created;
    int _timeToWait;
    int _timeTriggered;
    int _timeCreated;
    Mutex _mtx;
    
  public:
  	
  	///constructor
    BonusManager();
    
    ///devuelve true si deberia aparecer un bonus según un numero al azar, en tal caso se le asigna un tiempo de expiracion
    bool createBonus();
    
    ///inicializa el manager
    void reset();
    
    ///el pacman comio el bonus, por lo que se actualiza el contador
    void eatBonus();
    
    ///devuelve a cantidad de bonus comidos por el pacman en el nivel actual
    int getBonusCounter();
    
    ///devuelve true si el bonus esta creado y debe expirar
    bool bonusExpired();
};

#endif /* __BONUSMANAGER_H__ */
