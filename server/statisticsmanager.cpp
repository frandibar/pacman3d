#include "statisticsmanager.h"


StatisticsManager::~StatisticsManager() {
  _stats.eatenGhosts.clear();
}

void StatisticsManager::reset(int pills, int powerups) {
  _mtx.lock();
  _stats.pills = pills;
  _stats.powerups = powerups;
  _stats.bonusCounter = 0;
  _stats.ghostsCounter = 0;
  _stats.eatenGhosts.clear();
  _stats.pillsEaten = 0;
  _stats.powerupsEaten = 0;
  _mtx.unlock();
}

void StatisticsManager::eatPill() {
  _mtx.lock();
  _stats.pillsEaten++;
  _mtx.unlock();
}

void StatisticsManager::eatBonus() {
  _mtx.lock();
  _stats.bonusCounter++;
  _mtx.unlock();
}

void StatisticsManager::eatPowerUp() {
  _mtx.lock();
  _stats.powerupsEaten++;
  _mtx.unlock();
}

void StatisticsManager::eatGhost(int ghostId) {
  _mtx.lock();
  _stats.ghostsCounter++;
  EatenGhostsMap::iterator it = _stats.eatenGhosts.find(ghostId);
  if (it == _stats.eatenGhosts.end())
    _stats.eatenGhosts[ghostId] = 1;
  else
    _stats.eatenGhosts[ghostId]++;
  _mtx.unlock();
}

tStats StatisticsManager::getStats() {
  tStats ret;
  _mtx.lock();
  ret = _stats;
  _mtx.unlock();
  return ret;
}

