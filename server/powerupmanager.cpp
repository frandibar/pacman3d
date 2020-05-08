#include "powerupmanager.h"
#include "lib/timer.h"

const int PowerUpManager::POWERUP_DURATION = 5000;

PowerUpManager::PowerUpManager() {
  reset();
}

void PowerUpManager::reset() {
  _mtx.lock();
  _powerUpEnabledTime = 0;
  _powerupMode = false;
  _mtx.unlock();
}

bool PowerUpManager::getPowerUpMode() {
  _mtx.lock();
  bool ret = _powerupMode;
  _mtx.unlock();
  return ret;
}

bool PowerUpManager::powerupExpires() {
  bool ret = false;
  _mtx.lock();
  //se fija si se expira el powerup
  if ((_powerupMode) && (Timer::getTime() - _powerUpEnabledTime > POWERUP_DURATION)) {
    _powerupMode = false;
    _powerUpEnabledTime = 0;
    ret = true;
  }
  _mtx.unlock();
  return ret;
}

void PowerUpManager::enablePowerup() {
  _mtx.lock();
  //habilita power up
  _powerupMode = true;
  _powerUpEnabledTime = Timer::getTime();
  _mtx.unlock();
}
