/**
 * @file scoremanager.h
 * */
#ifndef __SCOREMANAGER_H__
#define __SCOREMANAGER_H__
#include "lib/mutex.h"

/// Tabla de puntuaciones
typedef enum tScore {SCORE_GHOST = 10,SCORE_BONUS = 50,SCORE_POWERUP = -5,SCORE_PILL = 1};

/**
 * Clase para administrar puntuacion
 * Lleva la cuenta del puntaje del Pacman
 * Conserva un tabla de puntajes para cada item que aporte puntos
 * Metodos protegidos por mutex
 */
class ScoreManager{
  private:
    int _score;
    int _bonusCounter;
    Mutex _mtx;
  public:
  	/// constructor
    ScoreManager():_score(0),_bonusCounter(0){}
    /// devuelve puntaje actual
    int getScore();
    /// agrega puntaje segun item
    void add(tScore type);
    /// inicializa manager
    void reset();
};

#endif /*__SCOREMANAGER_H__*/
