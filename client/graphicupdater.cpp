#include "graphicupdater.h"
#include "Player.h"
#include "Maze.h"

#include "lib/debug.h"

#include <cmath>

std::string vStopReasons[] = { "Pacman WON!",
                               "Pacman was eaten by a ghost!",
                               "Pacman was disconnected",
                               "Minimum amount of players not achieved",
                               "Server was terminated",
                               "Maximum players achieved",
                               "Socket error"
                            };

const string GraphicUpdater::DIR_CLASSIC = "/etc/Pacman3D/wav/classic";
const string GraphicUpdater::DIR_MODERN  = "/etc/Pacman3D/wav/modern";

const int FRAME_AVERAGE  = 10;  // average timer over n frames 
const int SCREEN_BPP = 16;      // bits per pixel
const int INVALID_PLAYER = 999;

GraphicUpdater::tGameStates GraphicUpdater::_state = GraphicUpdater::STATE_WAIT;

const int NSCREEN_MODES = 4;
int GraphicUpdater::_screenWidths[]  = { 640, 800, 1024, 1280 };
int GraphicUpdater::_screenHeights[] = { 480, 600,  768, 1024 };

// constructor
GraphicUpdater::GraphicUpdater(SocketProtocol& connection, tSoundType soundType, const string& playerName, 
                               tMovementKeys movementKeys, const ShortcutMap& chatShortcuts, int protocolVersion) : 
                                    _connection(connection), _isPacman(false), _playerId(INVALID_PLAYER), _quit(false), 
                                    _screenMode(0), _showMap(true),
                                    _soundManager(soundType), _packetManager(protocolVersion), 
                                    _keys(movementKeys), _chatMap(chatShortcuts)
{
    _playerName.assign(playerName);

    // initialize sdl
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0) {
        std::cerr << "Video initialization failed: " << SDL_GetError();
        quit(1);
    }

    SDL_ShowCursor(false); // hide mouse

    const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();    // fetch the video info
    if (!videoInfo) {
        std::cerr << "Video query failed: " << SDL_GetError() << std::endl;
        quit(1);
    }

    // flags to pass to SDL_SetVideoMode
    _videoFlags = SDL_OPENGL;       // enable opengl in sdl
    _videoFlags |= SDL_GL_DOUBLEBUFFER; // enable double buffering
    _videoFlags |= SDL_HWPALETTE;       // store the palette in hardware

    // checks if surfaces can be stored in memory
    if (videoInfo->hw_available) _videoFlags |= SDL_HWSURFACE;
    else _videoFlags |= SDL_SWSURFACE;

    // checks if hardware blits can be done
    if (videoInfo->blit_hw) _videoFlags |= SDL_HWACCEL;

    // set up opengl double buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // get an sdl surface
    Maze::instance().setSurface(SDL_SetVideoMode(_screenWidths[_screenMode], _screenHeights[_screenMode], SCREEN_BPP, _videoFlags));

    // verify there is a surface
    if (!Maze::instance().getSurface()) {
        std::cerr << "Video mode set failed: " << SDL_GetError() << std::endl;
        quit(1);
    }

    initGL();                                                               // initialize opengl
    resizeWindow(_screenWidths[_screenMode], _screenHeights[_screenMode]);  // resize the initial window
}

// destructor
GraphicUpdater::~GraphicUpdater() 
{
}

void GraphicUpdater::setIsPacman(bool isPacman) 
{
    _isPacman = isPacman;
    DEBUG_1<bool>(isPacman, "INIT- PACMAN = ");
}

//detiene el juego
void GraphicUpdater::quitGame() 
{
    _soundManager.playGameOver();    // play sound
    _quit = true;
}

//pacman come pill
void GraphicUpdater::eatPill()
{
    _soundManager.playEatPill();
}

//pacman come fantasma
void GraphicUpdater::eatGhost()
{
    _soundManager.playEatGhost();
}

//pacman come power up
void GraphicUpdater::powerUp()
{
    _soundManager.playPowerUp();
}

//pacman come bonus
void GraphicUpdater::bonus() 
{
    _soundManager.playBonus();
}

//murio en el juego
void GraphicUpdater::killPlayer()
{
    _soundManager.playKilled();
}

//recibe mensaje de chat
void GraphicUpdater::showChatMessage(const string& msg)
{
    _soundManager.playChatMessage();
    _chat = _playerName + ": " + msg;
    DEBUG_1<string>(msg, " MESSAGE-  msg= ");
}

//para el juego
void GraphicUpdater::stopLevel(tStopReason reason, tStats stats) 
{
    _state = STATE_STATISTICS;
    _stats = stats;
    _reason = reason;

    DEBUG_1<string>(vStopReasons[reason], " MESSAGE-  msg= ");
    DEBUG_1<string>("   stats");
    DEBUG_1<int>(stats.pills,         "      pills = ");
    DEBUG_1<int>(stats.powerups,      "      powerUps = ");
    DEBUG_1<int>(stats.bonusCounter,  "      bonus = ");
    DEBUG_1<int>(stats.pillsEaten,    "      pillsEaten = ");
    DEBUG_1<int>(stats.powerupsEaten, "      powerupsEaten = ");
    DEBUG_1<int>(stats.ghostsCounter, "      ghostsCounter = ");

    for (EatenGhostsMap::const_iterator it = stats.eatenGhosts.begin(); it != stats.eatenGhosts.end(); ++it) {
        DEBUG_1<int>(it->first, "      ghost id = ", true, false);
        DEBUG_1<int>(it->second, "  times = ", false, true);
    }
}

//cambia de nivel
void GraphicUpdater::updateScore(int newscore) 
{
    Maze::instance().getPlayer(_playerId)->setScore(newscore);
    //DEBUG_1<int>(newscore, " Update score-  score= ");
}

//setea id de jugador
void GraphicUpdater::setPlayerId(unsigned int playerId) 
{
    // _isPacman was set prior to calling this function
    _playerId = playerId;
    // add myself to maze
    Maze::instance().addPlayer(_playerId, _isPacman ? Player::PACMAN : Player::GHOST);
    DEBUG_1<unsigned int>(playerId, "Set Player Id -  id = ");
}

//inicializa nivel
void GraphicUpdater::initLevel(int width, int height, const EdgeSet& edges, const ElementList& elements) 
{
    Lock lock(_mutex);
    // load the maze
    Maze::instance().load(width, height, edges);

    DEBUG_1<int>(width, "INIT - width = ", true, false);
    DEBUG_1<int>(height, " height = ", false, true);

    DEBUG_1<string>("Edges:");
    for (EdgeSet::iterator it = edges.begin(); it != edges.end(); ++it)
        DEBUG_1<int>(*it);

    DEBUG_1<string>("Elements:");
    for (ElementList::const_iterator it = elements.begin(); it != elements.end(); ++it) {
        // add element to maze
        Maze::instance().addElement(it->getType(), it->getIdVertex(), it->getOrientation());
        
        DEBUG_1<string>("    Element:");
        DEBUG_1<int>(it->getIdVertex(), "       vertexId = ");
        switch (it->getType()) {
            case PACMAN_START:
                DEBUG_1<string>("       type = PACMAN_START");
                break;
            case GHOST_HOUSE:
                DEBUG_1<string>("       type = GHOST_HOUSE");
                break;
            case POWER_UP:
                DEBUG_1<string>("       type = POWER_UP");
                break;
            case BONUS:
                DEBUG_1<string>("       type = BONUS");
                break;
            case PILL:
                DEBUG_1<string>("       type = PILL");
                break;
        }
        switch (it->getOrientation()) {
            case NORTH:
                DEBUG_1<string>("       orientation = NORTH");
                break;
            case SOUTH:
                DEBUG_1<string>("       orientation = SOUTH");
                break;
            case EAST:
                DEBUG_1<string>("       orientation = EAST");
                break;
            case WEST:
                DEBUG_1<string>("       orientation = WEST");
                break;
        }
    }
}

//actualiza status
void GraphicUpdater::refreshStatus(const PlayerStatusList& positionList, const ElementList& elements, const NameList& namesChanged) 
{
    Lock lock(_mutex);
    if (_state == STATE_WAIT)
        _state = STATE_ACTIVE;

    // nameschanged tiene los jugadores nuevos...
    // puede ser q haya nuevos o q se hayan desconectado otros
    // la forma de validar si son nuevos o se desconectaron es preguntar por el id...
    // si no esta en el mapa q te paso en el init entonces es nuevo, sino se desconecto

    // refresh player positions
    DEBUG_1<string>("STATUS");
    for (PlayerStatusList::const_iterator it = positionList.begin(); it != positionList.end(); ++it) {
        DEBUG_1<int>(it->playerId, "     Player = ");
        Player* player = Maze::instance().getPlayer(it->playerId);
        if (player == NULL) {
            DEBUG_1<string>("     NEW!");
            // player is new, add it
            Maze::instance().addPlayer(it->playerId, _isPacman ? Player::GHOST : Player::PACMAN);
            player = Maze::instance().getPlayer(it->playerId);
            if (player == NULL) continue;
        }
        player->setPowerUpEnabled(it->powerUpEnabled);
        player->setIsAlive(it->alive);
        //Lock lock(_mutex);
        Maze::instance().setPlayerPosition(player, it->edgeId, it->position, it->direction);
        // find player name in namesChanged
        for (NameList::const_iterator it2 = namesChanged.begin(); it2 != namesChanged.end(); ++it2) {
            if (it2->id == it->playerId) {
                player->setName(it2->name);
                DEBUG_1<string>(it2->name, "     Name = ");
                break;
            }
        }

        // debugging info
        DEBUG_1<string>((it->powerUpEnabled ? "TRUE" : "FALSE"), "     powerupEnabled = ");
        DEBUG_1<string>((it->alive ? "TRUE" : "FALSE"), "     alive = ");
        DEBUG_1<int>(it->edgeId,    "     edgeId = ");
        DEBUG_1<int>(it->position,  "     position = ");
        DEBUG_1<int>(it->direction, "     direction = ");
    }

    // refresh elements
    DEBUG_1<string>("  Elements:");
    for (ElementList::const_iterator it = elements.begin(); it != elements.end(); ++it) {
        // update element state
        Maze::instance().updateElementState(it->getType(), it->getIdVertex(), it->getState()); 
        // play sound if element was eaten
        if (it->getState() == EATEN) {
            switch (it->getType()) {
                case PILL:     eatPill();  break;
                case POWER_UP: powerUp();  break;
                case BONUS:    bonus();    break;
                default: break;
            }
        }

        // debugging info
        DEBUG_1<string>("    Element:");
        DEBUG_1<int>(it->getIdVertex(), "       vertexId = ");
        switch (it->getType()) {
            case PACMAN_START:
                DEBUG_1<string>("       type = PACMAN_START");
                break;
            case GHOST_HOUSE:
                DEBUG_1<string>("       type = GHOST_HOUSE");
                break;
            case POWER_UP:
                DEBUG_1<string>("       type = POWER_UP");
                break;
            case BONUS:
                DEBUG_1<string>("       type = BONUS");
                break;
            case PILL:
                DEBUG_1<string>("       type = PILL");
                break;
        }
        switch (it->getOrientation()) {
            case NORTH:
                DEBUG_1<string>("       orientation = NORTH");
                break;
            case SOUTH:
                DEBUG_1<string>("       orientation = SOUTH");
                break;
            case EAST:
                DEBUG_1<string>("       orientation = EAST");
                break;
            case WEST:
                DEBUG_1<string>("       orientation = WEST");
                break;
        }
    }
}

void GraphicUpdater::quit(int returnCode)
{
    SDL_Quit();         // clean up the window
    //exit(returnCode);   // and exit appropriately
}

void GraphicUpdater::resizeWindow(unsigned int width, unsigned int height) 
// reset viewport after a window resize
{
    if (height == 0) height = 1;                        // avoid division by zero

    GLfloat ratio = (GLfloat)width / (GLfloat)height;

    glViewport(0, 0, (GLint)width, (GLint)height);      // setup viewport

    // change to projection matrix and set viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, ratio, 0.1f, 100.0f);         // set perspective

    // make sure we're changing the model view and not the projection
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();                                   // reset view
}

// function to handle key press events
void GraphicUpdater::handleKeyPress(SDL_keysym* keysym, bool pressed, int timeval) 
{
    const float STEP = 0.01;            // step unit when advancing
    const float ANGLE_STEP = 10.0;     // step angle when turning
    const float EPSILON = 0.01;        // used to compare differences with floats

    enum tDirection { DIR_NONE = 0, DIR_RIGHT = 1, DIR_LEFT = 2, DIR_UP = 4, DIR_DOWN = 8 }; // values allow combination
    static tDirection currentDir = DIR_NONE;  // current walking direction
    static tDirection nextDir = DIR_NONE;     // next direction read from keyboard
    static bool turning = false;              // true while turning right or left
    static float degrees = 0.0;               // current angle of turn (0-90)
    static float steps = 0.0;                 // advanced steps from last stop
    static bool turnAround = false;

    if (keysym != NULL) {
        // a key was pressed or released
        if (pressed) {
            // handle special keys
            if (keysym->sym == SDLK_F1) {           // toggle fullscreen
                if (SDL_WM_ToggleFullScreen(Maze::instance().getSurface()) == 0)
                    std::cerr << "Toggle full screen failed!" << std::endl;
            }

            else if (keysym->sym == SDLK_F2) {      // set screen resolution
                _screenMode = (_screenMode + 1) % NSCREEN_MODES;
                Maze::instance().setSurface(SDL_SetVideoMode(_screenWidths[_screenMode], _screenHeights[_screenMode], SCREEN_BPP, _videoFlags));
                resizeWindow(_screenWidths[_screenMode], _screenHeights[_screenMode]);
            }

            else if (keysym->sym == SDLK_F3) {      // toggle sound
                _controls.toggleSound();
                _soundManager.toggleMute();
            }

            else if (keysym->sym >= SDLK_F4 && keysym->sym <= SDLK_F11) {      // send msg
                showChatMessage(_chatMap[keysym->sym]);
            }

            else if (keysym->sym == SDLK_F12) {     // quit game
                quitGame();
            }
            else if (keysym->sym == 'm' || keysym->sym == 'M') {     // toggle map view
                _showMap = !_showMap;
            }
        }

        Packet packet;
        if (keysym->sym == _keys.forward) {
            if (pressed) {
                nextDir = DIR_UP;
                _packetManager.createKey(packet, (tKeyType)UP);
                _connection.send(packet);
                //DEBUG_0<string>("UP key pressed and accepted");
            }
            else {
                nextDir = tDirection(nextDir & ~DIR_UP);
                //DEBUG_0<string>("UP key released and accepted");
            }
        }
        else if (keysym->sym == _keys.right) {
            if (pressed) {
                nextDir = tDirection((nextDir & ~DIR_LEFT) | DIR_RIGHT);
                _packetManager.createKey(packet, (tKeyType)RIGHT);
                _connection.send(packet);
                //DEBUG_0<string>("RIGHT key pressed and accepted");
                //DEBUG_0<int>(nextDir, "nextDir: ");
            }
            else {
                nextDir = tDirection(nextDir & ~DIR_RIGHT);
                //DEBUG_0<string>("RIGHT key released and accepted");
                //DEBUG_0<int>(nextDir, "nextDir: ");
            }
        }
        else if (keysym->sym == _keys.left) {
            if (pressed) {
                nextDir = tDirection((nextDir & ~DIR_RIGHT) | DIR_LEFT);
                _packetManager.createKey(packet, (tKeyType)LEFT);
                _connection.send(packet);
                //DEBUG_0<string>("LEFT key pressed and accepted");
                //DEBUG_0<int>(nextDir, "nextDir: ");
            }
            else {
                nextDir = tDirection(nextDir & ~DIR_LEFT);
                //DEBUG_0<string>("LEFT key released and accepted");
                //DEBUG_0<int>(nextDir, "nextDir: ");
            }
        }
        else if (keysym->sym == _keys.turnBack) {
            if (pressed) {
                nextDir = DIR_DOWN;
                _packetManager.createKey(packet, (tKeyType)DOWN);
                _connection.send(packet);
                //DEBUG_0<string>("DOWN key pressed and accepted");
            }
            else {
                nextDir = tDirection(nextDir & ~DIR_DOWN);
                //DEBUG_0<string>("DOWN key released and accepted");
            }
        }
    }

    return; // TODO: remove

    if (((currentDir & (DIR_RIGHT | DIR_LEFT)) == DIR_RIGHT) || ((currentDir & (DIR_RIGHT | DIR_LEFT)) == DIR_LEFT))  // turning?
        turning = true;
    else if ((currentDir & DIR_DOWN) == DIR_DOWN) {      // turning around
        currentDir = DIR_RIGHT;
        turning = true;
        turnAround = true;
    }

    // continue movement while key is pressed
    if (_state == STATE_ACTIVE) {
        // handle currently pressed keys
        if (turning) {
            // avoid overpassing 90 degrees
            float step = (degrees + ANGLE_STEP <= 90.0 ? ANGLE_STEP : 90.0 - degrees - ANGLE_STEP);
            if ((currentDir & (DIR_RIGHT | DIR_LEFT)) == DIR_RIGHT) {           // turning right?
                Maze::instance().getPlayer(_playerId)->turnRight(step);
                degrees += step;
            }
            else if ((currentDir & (DIR_RIGHT | DIR_LEFT)) == DIR_LEFT) {       // turning left?
                Maze::instance().getPlayer(_playerId)->turnLeft(step);
                degrees += step;
            }
            if (degrees >= 90.0) {
                if (turnAround) {
                    // continue turning other 90 deg
                    turnAround = false;
                }
                else {
                    turning = false;
                    currentDir = nextDir;
                    DEBUG_0<string>("finished turning");
                    DEBUG_0<int>(currentDir, "currentDir: ");
                }
                degrees = 0.0;
            }
        }
        else if ((currentDir & DIR_UP) == DIR_UP) {
            // advance but avoid overpassing 1.0 
            if (Maze::instance().addStep(_playerId, STEP))
                steps += STEP;
            else {
                // could not advance, collision was detected
                steps = 0.0;
                currentDir = nextDir;
                DEBUG_0<string>("colission detected");
                DEBUG_0<int>(currentDir, "currentDir: ");
            }
            if (std::abs(steps - 1.0) <= EPSILON) {
                steps = 0.0;
                currentDir = nextDir;
                DEBUG_0<string>("finished walking");
                DEBUG_0<int>(currentDir, "currentDir: ");
            }
        }

        if (currentDir == DIR_NONE) currentDir = nextDir;
    }
}

// general opengl initialization function
void GraphicUpdater::initGL() 
{
    SDL_WM_SetCaption("Pacman 3D", NULL);   // set window title bar

    glShadeModel(GL_SMOOTH);                // enable smooth shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   // set the background color to black
    glClearDepth(1.0f);                     // depth buffer setup
    glDepthFunc(GL_LEQUAL);                 // the type of depth test to do

    // really nice perspective calculations
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // set lights
    GLfloat vLightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat vLightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat vLightPosition[] = { 0.0f, 0.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT1, GL_AMBIENT, vLightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, vLightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, vLightPosition);

    Maze::instance().init();
    _controls.init();
}

// drawing code
void GraphicUpdater::drawGLScene(GLint timer)
{
    Lock lock(_mutex);  // aquire mutex
    
    // display chat message for certain time
    static GLint T0 = 0;
    GLint t = SDL_GetTicks();
    if (t - T0 >= 10000) {
        T0 = t;
        _chat = "";
    }

    // TODO use timer!
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen and the depth buffer
      
    if (_state == STATE_STATISTICS) {
        // show statistics for 5 secs
        _controls.showStatistics(_screenWidths[_screenMode], _screenHeights[_screenMode], vStopReasons[_reason], _stats);
        SDL_GL_SwapBuffers();       // draw it to the screen
        sleep(5);
        _state = STATE_WAIT;
        return;
    }
    else if (_state == STATE_ACTIVE) {
        // draw players view
        Maze::instance().draw(_screenWidths[_screenMode], _screenHeights[_screenMode], _playerId);
        if (_showMap)
            Maze::instance().draw(_screenWidths[_screenMode], _screenHeights[_screenMode], _playerId, true);
        // draw 2d control stuff
        _controls.draw(_screenWidths[_screenMode], _screenHeights[_screenMode], _playerId, false, true, _chat);
    }
    else if (_state == STATE_WAIT) {
        // draw menu
        _controls.draw(_screenWidths[_screenMode], _screenHeights[_screenMode], _playerId, true, false, _chat);
    }
    
    SDL_GL_SwapBuffers();       // draw it to the screen
}

GLfloat GraphicUpdater::getTime() 
{
    static GLint timespec[FRAME_AVERAGE] = { 0 };

    // not initialized
    if (timespec[0] == 0) 
        for (int i = 0; i < FRAME_AVERAGE; ++i)
            timespec[i] = 0;

    // get current time spec
    GLint t = SDL_GetTicks();
    GLint ret = t - timespec[FRAME_AVERAGE-1];

    // shift timespec up
    for (int i = FRAME_AVERAGE - 1; i > 0; --i)
        timespec[i] = timespec[i-1];

    // store current spec
    timespec[0] = t;

    return (float)ret / FRAME_AVERAGE;
}


int GraphicUpdater::startGame()
{
    _soundManager.playStart();

    // wait for events
    GLfloat timer = 0;
    while (!_quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {

                case SDL_KEYDOWN:
                    handleKeyPress(&event.key.keysym, true, (int)timer);
                    break;

                case SDL_KEYUP:
                    handleKeyPress(&event.key.keysym, false, (int)timer);
                    break;

                case SDL_VIDEORESIZE:
                    // handle resize event
                    Maze::instance().setSurface(SDL_SetVideoMode(event.resize.w, event.resize.h, SCREEN_BPP, _videoFlags));
                    if (!Maze::instance().getSurface()) {
                        std::cerr << "Could not get a surface after resize: " << SDL_GetError() << std::endl;
                        quit(1);
                    }
                    resizeWindow(event.resize.w, event.resize.h);
                    break;

                case SDL_QUIT:
                    //quit(0);
                    //quitGame();
                    _quit = true;
                    break;

                default:
                    break;
            }
        }
		
        handleKeyPress(NULL, false, (int)timer);
        drawGLScene((int)timer);
        timer = getTime();          // get timer for constant speed stuff
        
		usleep(STATUS_PERIOD*1000);
    }

    quit(0);
    return 0; // should never get here
}

void GraphicUpdater::abort() 
{ 
    //LLAMADO SI SE CUELGA EL SERVER... CIERRE DE CLIENTE 
    quitGame(); 
} 

