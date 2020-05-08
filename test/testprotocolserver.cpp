#include "common/socketprotocol.h"
#include "common/packetmanager.h"

void init (SocketProtocol* cnx,PacketManager &manager,Packet &packet) {
  //arma grafo
  PacManMap *map = new PacManMap(7,5);
  Element* elem1 = new Element(0,PILL,NORTH);
  Vertex *vertex1 = map->addVertex(elem1,0);
  Element* elem2 = new Element(1,POWER_UP,EAST);
  Vertex *vertex2 = map->addVertex(elem2,1);
  Element* elem3 = new Element(8,BONUS,WEST);
  Vertex *vertex3 = map->addVertex(elem3,8);
  map->addEdge(vertex1,vertex2);
  map->addEdge(vertex2,vertex3);
  //arma elements
  ElementList *elements = new ElementList;
  Element elemPS(0,PACMAN_START,NORTH);
  elements->push_back(elemPS);
  Element elemGH(8,GHOST_HOUSE,SOUTH);
  elements->push_back(elemGH);
  Level level(map,SPEED_LOW);
  //arma nombres
  NameList names;
  tPlayerName playerName;
  playerName.id = 0;
  playerName.name = "pacman";
  names.push_back(playerName);
  playerName.id = 1;
  playerName.name = "ghost1";
  names.push_back(playerName);
  playerName.id = 2;
  playerName.name = "ghost2";
  names.push_back(playerName);
  manager.createInit(packet,true,level,names);
  cnx->send(packet);
}

void start (SocketProtocol* cnx,PacketManager &manager,Packet &packet) {
  manager.createStart(packet,2);
  cnx->send(packet);
}

void status (SocketProtocol* cnx,PacketManager &manager,Packet &packet) {
  int score = 500;
  PlayerStatusList positionList;
  tPlayerStatus playerStatus;
  playerStatus.playerId = 0;
  playerStatus.powerUpEnabled = true;
  playerStatus.alive = true;
  playerStatus.edgeId = 7;
  playerStatus.position = 31;
  playerStatus.direction = 0;
  positionList.push_back(playerStatus);
  playerStatus.playerId = 1;
  playerStatus.powerUpEnabled = false;
  playerStatus.alive = false;
  playerStatus.edgeId = 15;
  playerStatus.position = 0;
  playerStatus.direction = 1;
  positionList.push_back(playerStatus);
  playerStatus.playerId = 2;
  playerStatus.alive = true;
  playerStatus.edgeId = 7;
  playerStatus.position = 7;
  playerStatus.direction = 0;
  positionList.push_back(playerStatus);
  //arma elements
  ElementList elements;
  Element elemPill(0,PILL,NORTH);
  elemPill.setState(EATEN);
  elements.push_back(elemPill);
  Element elemBonus(8,BONUS,SOUTH);
  elemBonus.setState(DISAPPEARED);
  elements.push_back(elemBonus);
  //arma nombres
  NameList namesChanged;
  tPlayerName playerName;
  playerName.id = 2;
  playerName.name = "ghost2";
  namesChanged.push_back(playerName);
  manager.createStatus(packet,score,positionList,elements,namesChanged);
  cnx->send(packet);
}

void stop (SocketProtocol* cnx,PacketManager &manager,Packet &packet) {
  manager.createStop(packet,PACMAN_WON,777);
  cnx->send(packet);
}

void quit (SocketProtocol* cnx,PacketManager &manager,Packet &packet) {
  manager.createQuit(packet);
  cnx->send(packet);
}

void key (SocketProtocol* cnx,PacketManager &manager,Packet &packet) {
  manager.createKey(packet,RIGHT);
  cnx->send(packet);
}

void message (SocketProtocol* cnx,PacketManager &manager,Packet &packet) {
  manager.createMessage(packet,"server:hola");
  cnx->send(packet);
}

void connect (SocketProtocol* cnx,PacketManager &manager,Packet &packet) {
  manager.createConnect(packet,"fulano");
  cnx->send(packet);
}

int main(int argc, char *argv[]) {
  SocketProtocol server("localhost",5000,8);
  SocketProtocol* cnx = server.accept();
  PacketManager manager(0);
  Packet packet;
  init(cnx,manager,packet);
  start(cnx,manager,packet);
  status(cnx,manager,packet);
  stop(cnx,manager,packet);
  quit(cnx,manager,packet);
  key(cnx,manager,packet);
  message(cnx,manager,packet);
  connect(cnx,manager,packet);
}
