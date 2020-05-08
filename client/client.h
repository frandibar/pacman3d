#pragma once

#include "connectscreen.h"
#include "graphicupdater.h"
#include <string>

class Client 
{
public:
    Client(int argc, char **argv):_argc(argc),_argv(argv)
    {}
    int run();

private:
    static const int END;
    static const int ERROR_PARAM;
    static const int ERROR_THREAD;
    static const int ERROR_SERVER;
    static const std::string STR_CONFIG_ERROR;
    static const std::string STR_ERROR_THREAD;
    static const std::string STR_ERROR_SERVER;
    static const int PROTOCOL_VERSION;

    int _argc;
    char **_argv;
    tSoundType _sound;
    tMovementKeys _movementKeys;
    ShortcutMap _chatShortcuts;
    SocketProtocol *_socket;
    std::string _playerName;

    bool showScreen();
};
