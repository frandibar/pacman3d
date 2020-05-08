#include "server/bonusmanager.h"
#include "server/powerupmanager.h"
#include "common/timer.h"
#include <iostream>
using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
  //testeo de bonus manager
  BonusManager bonusManager;
  cout << "Testing Bonus..." << endl;
  int counter = 0;
  int time = Timer::getTime();
  while (counter < 5) {
    if (bonusManager.createBonus()) {
      //creo bonus
      cout << "    bonus CREATED: " << Timer::getTime() - time << endl;
      time = Timer::getTime();
      counter++;
    }
    if (bonusManager.bonusExpired()) {
      //expiro bonus
      cout << "    bonus EXPIRED: " << Timer::getTime() - time << endl;
      time = Timer::getTime();
    }
  }
  //testeo de power up manager
  PowerUpManager powerupManager;
  cout << "Testing Power Up..." << endl;
  time = Timer::getTime();
  cout << "    Power Up enabled" << endl;
  powerupManager.enablePowerup();
  while (1 == 1) {
    if (powerupManager.powerupExpires()) {
      cout << "    Power Up expired: " << Timer::getTime() - time << endl;
      break;
    }
  }
  return 0;
}
