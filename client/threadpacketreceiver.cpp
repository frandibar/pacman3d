#include "threadpacketreceiver.h"
#include "lib/timer.h"

const int ThreadPacketReceiver::DELAY_TIME = 100;

ThreadPacketReceiver::ThreadPacketReceiver(const int &protocolVersion,SocketProtocol &connection,GraphicUpdater &graphicUpdater):_packetManager(protocolVersion), _connection(connection), _graphicUpdater(graphicUpdater) {
  _state = OK;
  _protocolVersion = protocolVersion;
  if (!_connection)
    _state = ERROR_SOCKET;
  if (!Thread::operator bool())
    _state = ERROR_THREAD;
}

ThreadPacketReceiver::operator bool() const {
  return ((_state == OK) && (_connection));
}

void ThreadPacketReceiver::initPacket(const Packet& packet) {
  //setea si es pacman
  bool isPacman = false;
  int width = 0;
  int height = 0;
  EdgeSet edges;
  ElementList elements;
  
  _packetManager.parseInit(packet,isPacman,width,height,edges,elements);
  
  _graphicUpdater.setIsPacman(isPacman);
  _graphicUpdater.initLevel(width,height,edges,elements);
  edges.clear();
  elements.clear();
}

void ThreadPacketReceiver::startPacket(const Packet& packet) {
  int id = 0;
  
  _packetManager.parseStart(packet,id);
  
  _graphicUpdater.setPlayerId(id);
}

void ThreadPacketReceiver::statusPacket(const Packet& packet) {
  int score = 0;
  PlayerStatusList positionList;
  ElementList elements;
  NameList namesChanged;
  
  _packetManager.parseStatus(packet,score,positionList,elements,namesChanged);
  
  _graphicUpdater.updateScore(score);
  _graphicUpdater.refreshStatus(positionList,elements,namesChanged);
  positionList.clear();
  elements.clear();
  namesChanged.clear();
}

void ThreadPacketReceiver::stopPacket(const Packet& packet) {
  tStopReason reason;
  int score = 0;
  tStats stats;

  _packetManager.parseStop(packet,reason,score,stats);
  
  _graphicUpdater.updateScore(score);
  _graphicUpdater.stopLevel(reason,stats);
}

void ThreadPacketReceiver::quitPacket(const Packet& packet) {
  _packetManager.parseQuit(packet);
  
  _graphicUpdater.quitGame();
  stop();
}

void ThreadPacketReceiver::messagePacket(const Packet& packet) {
  string msg("");
  _packetManager.parseMessage(packet,msg);
  
  _graphicUpdater.showChatMessage(msg);
}

void ThreadPacketReceiver::main() {
  if (!*this)
    return;
  _phase = WAITING;
  bool read = true;
  Packet packet;
  //se fija si hay datos para leer en el socket
  while ((*this) && (!_mustStop)){
    if (_connection.select(read,DELAY_TIME)) {
      //hay datos a leer
      _connection.rcv(packet);
      if (!_connection) {
        _graphicUpdater.abort();
        _state = ERROR_SOCKET;
        return;
      }
      Header header(packet.getHeader());
      //se fija que el protocolo sea "aceptable"
      if (header.getVersion() > _protocolVersion)
        continue;
      switch (header.getType()) {
        case INIT:
          if (_phase == WAITING) { //esta esperando init
            initPacket(packet);
            _phase = INITIALIZED;
          }
          break;
        case START:
          if (_phase == INITIALIZED) { //esta esperando start
            startPacket(packet);
            _phase = STARTED;
          }
          break;
        case STATUS:
          if (_phase == STARTED) //esta esperando status
            statusPacket(packet);
          break;
        case STOP:
          _phase = WAITING;
          stopPacket(packet);
          break;
        case QUIT:
          quitPacket(packet);
          break;
        case MESSAGE:
          messagePacket(packet);
          break;
        default:
          continue;
      }
    }
  }
}
