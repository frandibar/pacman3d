#ifndef __GRAPHICUPDATER_H__
#define __GRAPHICUPDATER_H__

#include "Controls.h"
#include "clientutilities.h"
#include "soundmanager.h"
#include "lib/socketprotocol.h"
#include "lib/packetmanager.h"
#include "lib/utilities.h"
#include "lib/mutex.h"

#include <map>

using std::map;

class GraphicUpdater
{
public:
    // game states
    // STATE_ACTIVE    // game running
    // STATE_WAIT      // waiting for server to start game
    enum tGameStates { STATE_WAIT, STATE_ACTIVE, STATE_STATISTICS };

    // constructor
    GraphicUpdater(SocketProtocol& connection, tSoundType _soundType, const string& playerName, 
                   tMovementKeys movementKeys, const ShortcutMap& chatShortcuts, int protocolVersion);
    ~GraphicUpdater();

    int startGame();
    void quitGame();
    void initLevel(int width, int height, const EdgeSet &edges, const ElementList &elements);
    void stopLevel(tStopReason reason, tStats stats);
    void updateScore(int newscore);
    void setIsPacman(bool isPacman);
    void setPlayerId(unsigned int playerId);
    void showChatMessage(const string& msg);
    void refreshStatus(const PlayerStatusList& positionList, const ElementList& elements, 
                       const NameList& namesChanged);
    void abort();
    static tGameStates getState() { return _state; }

private:
    static tGameStates _state;

    void quit(int returnCode);
    void resizeWindow(unsigned int width, unsigned int height);
    void handleKeyPress(SDL_keysym* keysym, bool pressed, int timeval);
    void initGL();
    void drawGLScene(GLint timer);
    GLfloat getTime();

    void eatPill();
    void powerUp();
    void eatGhost();
    void bonus();
    void killPlayer();

    static const string DIR_CLASSIC;
    static const string DIR_MODERN;

    static int _screenWidths[];
    static int _screenHeights[];

    // member variables
    SocketProtocol& _connection;
    bool _isPacman;
    unsigned int _playerId;
    bool _quit;
    string _playerName;
    int _screenMode;
    bool _showMap;
    int _videoFlags;
    Controls _controls;
    SoundManager _soundManager;
    PacketManager _packetManager;
    Mutex _mutex;                   // refreshing status and drawing are mutually exclusive
    tStats _stats;
    int _reason;
    tMovementKeys _keys;
    ShortcutMap _chatMap;
    std::string _chat;
};

#endif

