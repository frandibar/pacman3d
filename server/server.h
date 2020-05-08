#pragma once

#include "configscreen.h"
#include "threaddispatcher.h"
#include "threaddistribution.h"
#include "threadgame.h"
#include "lib/helper.h"

#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

class Server 
{
public:
  	// constructor
    Server(int argc, char **argv): _argc(argc), _argv(argv){};
    // ejecuta el server y habilita el servicio de juegos
    int run();

private:
    //se fija si se presiono una tecla en STDIN
    bool kbhit();
    void loadConfig(const tConfig &config);
    int startService(const tConfig &config);

    static const int END;
    static const int ERROR_PARAM;
    static const int ERROR_THREAD;
    static const int ERROR_XML;
    static const int MAX_CLIENTS;
    static const int MAX_PLAYERS;
    static const int PROTOCOL_VERSION;
    static const int WAITKBD;
    static const char SALIDA;
    static const string STR_SERVER_ABORT;
    static const string STR_XML_ERROR;
    static const string STR_XML_WORLD_NO_LEVELS;
    static const string STR_CONFIG_ERROR;
    static const string STR_SERVER_ERROR;
    static const string STR_CONNECT_ERROR;
    static const string STR_SERVER_INIT;

    // member variables
    int _argc;
    char** _argv;
    tGameConfig _gameConfig;
};

