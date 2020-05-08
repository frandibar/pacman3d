#ifndef __STATISTICS_MANAGER_H__
#define __STATISTICS_MANAGER_H__
#include "lib/mutex.h"
#include "lib/utilities.h"

/**
 * Clase para administrar las estadisticas del nivel
 * Metodos protegidos por mutex
 */
class StatisticsManager {
  private:
    tStats _stats;
    Mutex _mtx;
    
  public:
  	/// destructor
    ~StatisticsManager();
    /// inicializa manager
    void reset(int pills, int powerups);
    /// pacman come un pill
    void eatPill();
    /// pacman come bonus
    void eatBonus();
    /// pacman come power up
    void eatPowerUp();
    /// pacman come un fantasma
    void eatGhost(int ghostId);
    /// devuelve las estadisticas
    tStats getStats();
};

#endif /*__STATISTICS_MANAGER_H__*/
