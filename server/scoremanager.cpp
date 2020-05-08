#include "scoremanager.h"

int ScoreManager::getScore() {
  _mtx.lock();
  int aux = _score;
  _mtx.unlock();
  return aux;
}

void ScoreManager::add(tScore type) {
  _mtx.lock();
  if (type == SCORE_BONUS) {
    _bonusCounter++;
    _score += type*_bonusCounter;
  }
  else
    _score += type;
  _mtx.unlock();
}

void ScoreManager::reset() {
  _mtx.lock();
  _score = 0;
  _bonusCounter = 0;
  _mtx.unlock();
}
