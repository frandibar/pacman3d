#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__
#include "soundplayer.h"
#include "clientutilities.h"
#include <string>
#include <SDL/SDL_mixer.h>
using std::string;

/**Clase para manejar de forma centralizada los sonidos
*/
class SoundManager{
  private:
    static const string DIR_CLASSIC;
    static const string DIR_MODERN;
    tSoundType _soundType;
    bool _mute;
    SoundPlayer *_start;
    SoundPlayer *_eatpill;
    SoundPlayer *_bonus;
    SoundPlayer *_powerup;
    SoundPlayer *_killed;
    SoundPlayer *_msg;
    SoundPlayer *_eatghost;
    SoundPlayer *_gameover;
  public:
    ///constructor
    SoundManager(tSoundType soundType);
    ///destructor
    ~SoundManager();
    ///sonido de inicio
    void playStart();
    ///sonido al comer pill
    void playEatPill();
    ///sonido al comer bonus
    void playBonus();
    ///sonido al comer power up
    void playPowerUp();
    ///sonido al morirse un jugador
    void playKilled();
    ///sonido al recibir un mensaje chat
    void playChatMessage();
    ///sonido al comerse un fantasma
    void playEatGhost();
    ///sonido al terminar el juego
    void playGameOver();
    void toggleMute() { _mute = !_mute; }
};
#endif
