#ifndef __SOUNDPLAYER_H__
#define __SOUNDPLAYER_H__
#include <string>
#include <SDL/SDL_mixer.h>
using std::string;

/**Clase para hacer sonar un sonido wav
*/
class SoundPlayer{
  private:
    Mix_Chunk *_sound;//sonido a reproducir
  public:
    ///constructor segun ruta al archivo wav
    SoundPlayer(const string &wavfile);
    ///destructor
    ~SoundPlayer();
    ///hace sonar el wav
    void play(bool wait = false);
};
#endif
