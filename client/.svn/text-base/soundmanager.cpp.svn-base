#include "soundmanager.h"

const string SoundManager::DIR_CLASSIC = "/etc/Pacman3D/wav/classic";
const string SoundManager::DIR_MODERN = "/etc/Pacman3D/wav/modern";

SoundManager::SoundManager(tSoundType soundType) {
  _soundType = soundType;
  _mute = (_soundType == MUTE);
  if (_soundType == MUTE) {
    _start = 0;
    _eatpill = 0;
    _bonus = 0;
    _powerup = 0;
    _killed = 0;
    _msg = 0;
    _eatghost = 0;
    _gameover = 0;
    return;
  }
  string dir("");
  if (_soundType == CLASSIC)
    dir.assign(DIR_CLASSIC);
  else
    dir.assign(DIR_MODERN);
  _start = new SoundPlayer (dir + "/start.wav");
  _eatpill = new SoundPlayer (dir + "/eat.wav");
  _bonus = new SoundPlayer (dir + "/bonus.wav");
  _powerup = new SoundPlayer (dir + "/powerup.wav");
  _killed = new SoundPlayer (dir + "/killed.wav");
  _msg = new SoundPlayer (dir + "/msg.wav");
  _eatghost = new SoundPlayer (dir + "/eatghost.wav");
  _gameover = new SoundPlayer (dir + "/gameover.wav");
}

SoundManager::~SoundManager() {
  if (_soundType != MUTE) {
    delete _start;
    delete _eatpill;
    delete _bonus;
    delete _powerup;
    delete _killed;
    delete _msg;
    delete _eatghost;
    delete _gameover;
  }
}

void SoundManager::playStart() {
  if (!_mute && _soundType != MUTE)
    _start->play();
}

void SoundManager::playEatPill() {
  if (!_mute && _soundType != MUTE)
    _eatpill->play();
}

void SoundManager::playBonus() {
  if (!_mute && _soundType != MUTE)
    _bonus->play();
}

void SoundManager::playPowerUp() {
  if (!_mute && _soundType != MUTE)
    _powerup->play();
}

void SoundManager::playKilled() {
  if (!_mute && _soundType != MUTE)
    _killed->play();
}

void SoundManager::playChatMessage() {
  if (!_mute && _soundType != MUTE)
    _msg->play();
}

void SoundManager::playEatGhost() {
  if (!_mute && _soundType != MUTE)
    _eatghost->play();
}

void SoundManager::playGameOver() {
  if (!_mute && _soundType != MUTE)
    _gameover->play(true);
}
