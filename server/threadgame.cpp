#include "threadgame.h"
#include "lib/helper.h"
#include <sstream>
using std::ostringstream;
using std::endl;

//tiempo de espera para primer jugador
const int ThreadGame::WAIT_FIRST_PLAYER = 100;

ThreadGame::ThreadGame(World &world,const tGameConfig &gameConfig, int protocolVersion): _config(gameConfig), _log(gameConfig.logFile), _world(world) {
  _protocolVersion = protocolVersion;
  _state = OK;
  _level = 0;
  _ended = false;
  _started = false;
  _playing = false;
  //lanza distribution
  _thdistrib = new ThreadDistribution;
  _thdistrib->run();
  if (!*_thdistrib)
    _state = ERROR_THREAD;
  _playerHandler = new PlayerHandler(*this);
  _clientManager = new ClientManager(*_thdistrib);
  _packetmanager = new PacketManager(gameConfig.protocolVersion);
  if (!Thread::operator bool())
    _state = ERROR_THREAD;
}

ThreadGame::~ThreadGame() {
  _thdistrib->stop();
  _thdistrib->join();
  delete _thdistrib;
  delete _packetmanager;
  delete _playerHandler;
  delete _clientManager;
}

ThreadGame::operator bool() const {
  return (_state == OK) && (_thdistrib);
}

void ThreadGame::addPlayer(SocketProtocol *socketAccept) {
  int playerId = _clientManager->getNumberClients();
  bool isPacman = (playerId == 0);
  ThreadTransmitter *thTransmitter = new ThreadTransmitter(socketAccept,playerId,isPacman);
  ThreadReceiver *thReceiver = new ThreadReceiver(socketAccept,playerId,_protocolVersion,*this);
  //lanza el threads para el jugador
  thTransmitter->run();
  thReceiver->run();
  if ((!*thTransmitter) || (!*thReceiver)) {
    //hay problemas de conexion
    //cierra thread receiver
    thReceiver->stop();
    thReceiver->join();
    delete thReceiver;
    //no hay vacantes para jugar => envia stop
    StatisticsManager statManager;
    statManager.reset(0,0);
    tStats stats = statManager.getStats();
    Packet stop;
    _packetmanager->createStop(stop,SOCKET_ERROR,0,stats);
    thTransmitter->sendPacket(stop);
    //envia quit
    Packet quit;
    _packetmanager->createQuit(quit);
    thTransmitter->sendPacket(quit);
    thTransmitter->stop();
    thTransmitter->join();
    delete thTransmitter;
    return;
  }
  //se fija si el numero de jugadores alcanza el maximo
  if (_clientManager->getNumberClients() == _config.max) {
    //no hay vacantes para jugar => envia stop
    StatisticsManager statManager;
    statManager.reset(0,0);
    tStats stats = statManager.getStats();
    Packet stop;
    _packetmanager->createStop(stop,MAXIMUM_PLAYERS,0,stats);
    thTransmitter->sendPacket(stop);
    //envia quit
    Packet quit;
    _packetmanager->createQuit(quit);
    thTransmitter->sendPacket(quit);
    //finaliza hilo transmitter
    thTransmitter->stop();
    thTransmitter->join();
    delete thTransmitter;
    //cierra thread receiver
    thReceiver->stop();
    thReceiver->join();
    delete thReceiver;
    return;
  }
  //suscribe request a distribuidor
  _thdistrib->suscribe(thTransmitter);
  //inserta cliente
  _clientManager->addClient(playerId,thTransmitter,thReceiver);
  //se fija si ya comenzo el juego
  if (_playing) {
    //envia init al cliente
    Packet init;
    _packetmanager->createInit(init,thTransmitter->isPacman(),*_level);
    thTransmitter->sendPacket(init);
    //envia start
    Packet start;
    _packetmanager->createStart(start,thTransmitter->getPlayerId());
    thTransmitter->sendPacket(start);
    _playerHandler->addPlayer(playerId);
  }
  //loguea nuevo cliente  
  ostringstream oss;
  if (thTransmitter->isPacman())
    oss << "New client pacman.";
  else
    oss << "New client ghost.";
  oss << " [" << thTransmitter->getHostName() << "]";
  _log.logMsg(oss.str());
}

//filtra jugadores en buen estado
void ThreadGame::checkPlayersStatus(bool &pacmanDisconnected) {
  IdList idlist = _clientManager->checkPlayersStatus(pacmanDisconnected);
  for(IdList::iterator it = idlist.begin(); it != idlist.end();++it) {
    //loguea desconexion
    ostringstream oss;
    if (*it == 0)
      oss << "Pacman disconnected.";
    else
      oss << "Ghost disconnected.";
    oss << " [" << _namesManager.findIP(*it) << "]";
    _log.logMsg(oss.str());
    //elimina nombre
    _namesManager.removeName(*it);
    //elimina player
    _playerHandler->removePlayer(*it);
  }
  idlist.clear();
}

void ThreadGame::sendInit() {
  TransmitterMap* transmitters = _clientManager->getRequests();
  Packet init;
  /*  init optimizado
      genera una sola vez el paquete, luego solo modifica header
      pues los init comparten el body */
  for(TransmitterMap::iterator it = transmitters->begin();it != transmitters->end();it++) {
    if (it == transmitters->begin())
      //crea paquete
      _packetmanager->createInit(init,it->second->isPacman(),*_level);
    else
      //crea paquete a partir de otro paquete init ya creado
      _packetmanager->createInit(init,it->second->isPacman(),init.getBody(),init.getBodyLength());
    it->second->sendPacket(init);
  }
  transmitters->clear();
  delete transmitters;
}

void ThreadGame::sendStart() {
  TransmitterMap* transmitters = _clientManager->getRequests();
  Packet start;
  for(TransmitterMap::iterator it = transmitters->begin();it != transmitters->end();it++) {
    _packetmanager->createStart(start,it->second->getPlayerId());
    it->second->sendPacket(start);
  }
  transmitters->clear();
  delete transmitters;
}

void ThreadGame::sendStatus(bool &pacmanDead) {
  pacmanDead = false;
  //jugadores
  PlayerStatusList playersStatus;
  PlayerListC* players = _playerHandler->getPlayerListCopy();
  bool powerUpMode = _playerHandler->getPacmanPowerUp();
  for(PlayerListC::iterator it = players->begin();it != players->end();it++) {
    tPlayerStatus player;
    //carga datos
    player.playerId = it->getPlayerId();
    player.powerUpEnabled = powerUpMode;
    player.alive = it->isAlive();
    if (player.playerId == 0) {
      //pacman muerto
      if (!player.alive) 
        pacmanDead = true;
    }
    player.edgeId = it->getEdgeID();
    player.position = it->getPosition();
    player.direction = it->getDirection();
    playersStatus.push_back(player);
  }
  delete players;
  //nombres
  NameList *namelist = _namesManager.getUpdates();
  //elementos
  ElementList *elements = _elementManager.getElementsAndWipe();
  //paquete
  Packet status;
  _packetmanager->createStatus(status,_scoreManager.getScore(),playersStatus,*elements,*namelist);
  namelist->clear();
  delete namelist;
  elements->clear();
  delete elements;
  //envia status a todos los jugadores
  _thdistrib->broadcast(status);
}

void ThreadGame::sendStop(const tStopReason &reason) {
  tStats stats = _statsManager.getStats();
  //envia stop
  Packet stop;
  _packetmanager->createStop(stop,reason,_scoreManager.getScore(),stats);
  _thdistrib->broadcast(stop);
}

void ThreadGame::sendQuit() {
  //envia quit
  Packet quit;
  _packetmanager->createQuit(quit);
  _thdistrib->broadcast(quit);
}

bool ThreadGame::runable() const {
  return ((*this) && (!_mustStop));
}

bool ThreadGame::checkMinPlayers() {
  if (_clientManager->getNumberClients() < _config.min) {
    //no se cumple el minimo
    sendStop(MINIMUM_NOT_ACHIEVED);
    sendQuit();
    return false;
  }
  return true;
}

void ThreadGame::logStartGame() {
  ostringstream oss;
  oss << "Game started " << " [";
  //lista clientes
  NameList *nameList = _namesManager.getNameList();
  if (nameList->size() > 0) {
    NameList::iterator it = nameList->begin();
    oss << _namesManager.findIP(it->id);
    it++;
    while(it != nameList->end()) {
      oss << "," << _namesManager.findIP(it->id);
      it++;
    }
  }
  nameList->clear();
  delete nameList;
  oss << "]";
  _log.logMsg(oss.str());
}

void ThreadGame::logEndGame() {
  ostringstream oss;
  oss << "Game finished." << " [";
  //lista clientes
  NameList *nameList = _namesManager.getNameList();
  if (nameList->size() > 0) {
    NameList::iterator it = nameList->begin();
    oss << _namesManager.findIP(it->id);
    it++;
    while(it != nameList->end()) {
      oss << "," << _namesManager.findIP(it->id);
      it++;
    }
  }
  nameList->clear();
  delete nameList;
  oss << "]";
  _log.logMsg(oss.str());
}

//espera por jugadores para jugar
void ThreadGame::waitFirstPlayer() {
  //espera que se conecte el primer jugador
  while ((runable()) && (_clientManager->getNumberClients() == 0))
    Sleep(WAIT_FIRST_PLAYER);
  //espera a jugadores q se conecten como fantasmas
  Sleep(_config.waitingTime);
}

bool ThreadGame::playLevel() {
  //inicializacion
  _playerHandler->initializeLevel(*_level);
  _scoreManager.reset();
  _bonusManager.reset();
  _powerupManager.reset();
  _elementManager.reset();
  int pills = _playerHandler->getPillsLeft();
  int powerups = _playerHandler->getPowerUpLeft();
  _statsManager.reset(pills,powerups);
  sendInit();
  //carga jugadores
  TransmitterMap *transmitters = _clientManager->getRequests();
  for(TransmitterMap::iterator it = transmitters->begin();it != transmitters->end();it++)
    _playerHandler->addPlayer(it->first);
  transmitters->clear();
  delete transmitters;
  //comienzo de juego
  sendStart();
  int lives = _config.lives;
  Element *elemBonus = 0;
  //simula nivel
  _playing = true;
  while (!_mustStop) {
    //remueve jugadores con error y se fija si cumple con minimo
    bool pacmanDisconnected = false;
    checkPlayersStatus(pacmanDisconnected);
    //envia stop y quit si se desconecto el pacman
    if (pacmanDisconnected) {
      _playing = false;
      sendStop(PACMAN_DISCONNECTED);
      sendQuit();
      if (elemBonus)
        delete elemBonus;
      logEndGame();
      return false;
    }
    if (!checkMinPlayers()) {
      _playing = false;
      sendStop(MINIMUM_NOT_ACHIEVED);
      sendQuit();
      if (elemBonus)
        delete elemBonus;
      logEndGame();
      return false;
    }
    //pide insertar un bonus
    if (_bonusManager.createBonus()) {
      _log.logMsg("Bonus created.");
      //pudo insertar un bonus
      if (!elemBonus)
        elemBonus = new Element(_playerHandler->BonusCreate());
      elemBonus->setState(APPEARED);
      _elementManager.addElement(*elemBonus);
    }
    //se fija si expiro el bonus
    if (_bonusManager.bonusExpired()) {
      _log.logMsg("Bonus expired.");
      _playerHandler->BonusExpire();
      elemBonus->setState(DISAPPEARED);
      _elementManager.addElement(*elemBonus);
    }
    //se fija si expira power up
    if (_powerupManager.powerupExpires()) {
      _playerHandler->setPacmanPowerUp(false);
      //loguea
      _log.logMsg("Power-Up expired.");
    }
    bool pacmanDead;
    sendStatus(pacmanDead);
    //se fija si pacman gano
    if (!_playerHandler->pillsOrPowerUpLeft()) {
      sendStop(PACMAN_WON);
      _playing = false;
      if (elemBonus)
        delete elemBonus;
      return true;
    }
    if (pacmanDead)
      lives--;
    //se fija si pacman perdio todas las vidas
    if (lives == 0) {
      sendStop(PACMAN_EATEN);
      _playing = false;
      if (elemBonus)
        delete elemBonus;
      return true;
    }
    //se fija si pacman murio y resetea posiciones
    if (pacmanDead) 
      _playerHandler->Reset();
    Sleep (STATUS_PERIOD);
  }
  return true;
}

//jugador toco tecla
void ThreadGame::pressedKey(const int &playerId,const tKeyType &key) {
  //valida que se este jugando un nivel
  if (_playing)
    _playerHandler->movePlayer(playerId,key);
}

//envia mensaje de chat
void ThreadGame::sendMessage(const int &playerId,const string &msg) {
  //valida que se este jugando un nivel
  if (!_playing)
    return;
  string chat = _namesManager.findName(playerId);
  chat.append(":");
  chat.append(msg);
  Packet packet;
  _packetmanager->createMessage(packet,chat);
  _thdistrib->broadcast(packet);
}

//agrega nombre de jugador
void ThreadGame::addName(const int &playerId,const string &playerName,const string &IP) {
  _namesManager.addName(playerId,playerName,IP);
}

void ThreadGame::main() {
  if (!*this) {
    _ended = true;
    return;
  }
  //corre hasta q tenga q parar o haya error
  _started = false;
  //espera a que se conecte el primer jugador => pacman
  waitFirstPlayer();
  if (_clientManager->getNumberClients() < _config.min) {
    sendStop(MINIMUM_NOT_ACHIEVED);
    sendQuit();
    _ended = true;
    return;
  }
  _started = true;
  //loguea inicio
  logStartGame();
  //recorre todos los niveles del mundo
  LevelList *levels = _world.getLevels();
  LevelList::const_iterator itLevel = levels->begin();
  int numLevel = 1;
  while ((!_mustStop) && (runable()) && (itLevel != levels->end())) {
    _log.logMsg("Playing Level " + StringConverter<int>(numLevel) + ".");
    _level = *itLevel;
    if (!playLevel()) {
      _ended = true;
      return;
    }
    //espera al terminar nivel
    Sleep(_config.waitingTime);
    itLevel++;
    numLevel++;
  }
  //termino el mundo
  sendQuit();
  _started = false;
  _ended = true;
  logEndGame();
}

//pacman comido por un fantasma
void ThreadGame::eatenPacman(int ghostId) {
  ostringstream oss;
  string name = _namesManager.findName(ghostId);
  string IP = _namesManager.findIP(ghostId);
  oss << " Ghost " << name << "["<< IP << "] ate pacman.";
  _log.logMsg(oss.str());
  //_playerHandler->stop();
}

//pacman se comio un fantasma
void ThreadGame::eatenGhost(int ghostId) {
  _scoreManager.add(SCORE_GHOST);
  ostringstream oss;
  string name = _namesManager.findName(ghostId);
  string IP = _namesManager.findIP(ghostId);
  oss << " Pacman ate ghost " << name << "["<< IP << "].";
  _log.logMsg(oss.str());
  _statsManager.eatGhost(ghostId);
}

//avisa q revivio un ghost (llego a la casa de fantasmas)
void ThreadGame::revivedGhost(int ghostId) {
  ostringstream oss;
  string name = _namesManager.findName(ghostId);
  string IP = _namesManager.findIP(ghostId);
  oss << " Ghost " << name << "["<< IP << "] reached ghost house.";
  _log.logMsg(oss.str());
}

void ThreadGame::eatenElement(const Element &elem) {
  Element element(elem);
  switch (element.getType()) {
    case POWER_UP:{
      _powerupManager.enablePowerup();
      _playerHandler->setPacmanPowerUp(true);
      _scoreManager.add(SCORE_POWERUP);
      element.setState(EATEN);
      //log
      ostringstream oss;
      oss << " Pacman ate power up at vertex " << element.getIdVertex() << ".";
      _log.logMsg(oss.str());
      _statsManager.eatPowerUp();
      break;}
    case BONUS:{
      _bonusManager.eatBonus();
      _scoreManager.add(SCORE_BONUS);
      element.setState(EATEN);
      //log
      ostringstream oss;
      oss << " Pacman ate bonus at vertex " << element.getIdVertex() << " [" << _bonusManager.getBonusCounter() << "].";
      _log.logMsg(oss.str());
      _statsManager.eatBonus();
      break;}
    case PILL:{
      element.setState(EATEN);
      _scoreManager.add(SCORE_PILL);
     _statsManager.eatPill();
      break;}
    default:
      return;
  }
  //agrega elemento para mandar en status
  _elementManager.addElement(element);
}

void ThreadGame::shutDown() {
  tStats stats = _statsManager.getStats();
  Packet stop;
  _packetmanager->createStop(stop,SERVER_TERMINATED,0,stats);
  _thdistrib->broadcast(stop);
  //envia quit
  Packet quit;
  _packetmanager->createQuit(quit);
  _thdistrib->broadcast(quit);
  _ended = true;
}
