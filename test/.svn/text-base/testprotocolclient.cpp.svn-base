#include "common/socketprotocol.h"
#include "common/packetmanager.h"
#include "common/packet.h"
#include <iostream>
using std::cout;
using std::endl;

void init(SocketProtocol &client,PacketManager &packetManager,Packet &packet) {
  client.rcv(packet);
  bool isPacman = false;
  int width = 0;
  int height = 0;
  EdgeSet edges;
  ElementList elements;
  NameList names;
  packetManager.parseInit(packet,isPacman,width,height,edges,elements,names);

  cout << "INIT Type:"<< packet.getHeader().getType() << endl;
  if (isPacman)
    cout << "  isPacman: TRUE" << endl;
  else
    cout << "  isPacman: FALSE" << endl;
  cout << "  width: " << width << "  height: " << height << endl;
  cout << "  Edges: ";
  for(EdgeSet::iterator it = edges.begin(); it != edges.end();++it)
    cout << *it << " ";
  cout << endl;
  cout << "  Elements: " << endl;
  for(ElementList::iterator it = elements.begin(); it != elements.end();++it) {
    cout << "    Element: " << endl;
    cout << "      vertexId = " << it->getIdVertex() << endl;
    switch (it->getType()) {
      case PACMAN_START:
        cout << "      type = PACMAN_START" << endl;
        break;
      case GHOST_HOUSE:
        cout << "      type = GHOST_HOUSE" << endl;
        break;
      case POWER_UP:
        cout << "      type = POWER_UP" << endl;
        break;
      case BONUS:
        cout << "      type = BONUS" << endl;
        break;
      case PILL:
        cout << "      type = PILL" << endl;
    }
    switch (it->getOrientation()) {
      case NORTH:
        cout << "      orientation = NORTH" << endl;
        break;
      case SOUTH:
        cout << "      orientation = SOUTH" << endl;
        break;
      case EAST:
        cout << "      orientation = EAST" << endl;
        break;
      case WEST:
        cout << "      orientation = WEST" << endl;
    }
  }
  cout << "  Names: ";
  for(NameList::iterator it = names.begin(); it != names.end();++it)
    cout << "    id = " << it->id << " name = " << it->name << endl;
}

void start(SocketProtocol &client,PacketManager &packetManager,Packet &packet) {
  client.rcv(packet);
  int id = 0;
  packetManager.parseStart(packet,id);

  cout << "START Type:"<< packet.getHeader().getType() << "- Id=" << id << endl;
}

void status(SocketProtocol &client,PacketManager &packetManager,Packet &packet) {
  client.rcv(packet);
  int score = 0;
  PlayerStatusList positionList;
  ElementList elements;
  NameList namesChanged;
  packetManager.parseStatus(packet,score,positionList,elements,namesChanged);

  cout << "STATUS Type:"<< packet.getHeader().getType() << endl;
  cout << "  Score: " << score << endl;
  cout << "  Player Status: " << endl;
  for(PlayerStatusList::iterator it = positionList.begin(); it != positionList.end();++it) {
    cout << "    Player " << (int)it->playerId << ":" << endl;
    if (it->powerUpEnabled)
      cout << "      powerUpEnabled = TRUE" << endl;
    else
      cout << "      powerUpEnabled = FALSE" << endl;
    if (it->alive)
      cout << "      alive = TRUE" << endl;
    else
      cout << "      alive = FALSE" << endl;
    cout << "      edgeId = " << it->edgeId << endl;
    cout << "      position = " << (int)it->position << endl;
    cout << "      direction = " << (int)it->direction << endl;
  }
  cout << "  Elements: " << endl;
  for(ElementList::iterator it = elements.begin(); it != elements.end();++it) {
    cout << "    Element:" << endl;
    cout << "      vertexId = " << it->getIdVertex() << endl;
    switch (it->getType()) {
      case PACMAN_START:
        cout << "      type = PACMAN_START" << endl;
        break;
      case GHOST_HOUSE:
        cout << "      type = GHOST_HOUSE" << endl;
        break;
      case POWER_UP:
        cout << "      type = POWER_UP" << endl;
        break;
      case BONUS:
        cout << "      type = BONUS" << endl;
        break;
      case PILL:
        cout << "      type = PILL" << endl;
    }
    switch (it->getOrientation()) {
      case NORTH:
        cout << "      orientation = NORTH" << endl;
        break;
      case SOUTH:
        cout << "      orientation = SOUTH" << endl;
        break;
      case EAST:
        cout << "      orientation = EAST" << endl;
        break;
      case WEST:
        cout << "      orientation = WEST" << endl;
    }
    switch (it->getState()) {
      case APPEARED:
        cout << "      state = APPEARED" << endl;
        break;
      case DISAPPEARED:
        cout << "      state = DISAPPEARED" << endl;
        break;
      case EATEN:
        cout << "      state = EATEN" << endl;
    }    
  }
  cout << "  Names: " << endl;
  for(NameList::iterator it = namesChanged.begin(); it != namesChanged.end();++it)
    cout << "    id = " << it->id << " name = " << it->name << endl;
}

void stop(SocketProtocol &client,PacketManager &packetManager,Packet &packet) {
  client.rcv(packet);
  tStopReason reason;
  int score = 0;
  packetManager.parseStop(packet,reason,score);
  string sreason("");
  switch (reason) {
    case PACMAN_WON:
      sreason.assign("PACMAN_WON");
      break;
    case PACMAN_EATEN:
      sreason.assign("PACMAN_EATEN");
      break;
    case PACMAN_DISCONNECTED:
      sreason.assign("PACMAN_DISCONNECTED");
      break;
    case MINIMUM_NOT_ACHIEVED:
      sreason.assign("MINIMUM_NOT_ACHIEVED");
      break;
    case SERVER_TERMINATED:
      sreason.assign("SERVER_TERMINATED");
      break;
    case MAXIMUM_PLAYERS:
      sreason.assign("MAXIMUM_PLAYERS");
  }
  
  cout << "STOP Type:"<< packet.getHeader().getType() << "- Reason=" << sreason << "- Score=" << score << endl;
}

void quit(SocketProtocol &client,PacketManager &packetManager,Packet &packet) {
  client.rcv(packet);
  packetManager.parseQuit(packet);

  cout << "QUIT Type:"<< packet.getHeader().getType() << endl;
}

void key(SocketProtocol &client,PacketManager &packetManager,Packet &packet) {
  client.rcv(packet);
  tKeyType key;
  packetManager.parseKey(packet,key);
  
  string skey("");
  switch (key) {
    case UP:
      skey.assign("UP");
      break;
    case LEFT:
      skey.assign("LEFT");
      break;
    case DOWN:
      skey.assign("DOWN");
      break;
    case RIGHT:
      skey.assign("RIGHT");
  }
  cout << "KEY Type:"<< packet.getHeader().getType() << "- Key=" << skey << endl;
}

void message(SocketProtocol &client,PacketManager &packetManager,Packet &packet) {
  client.rcv(packet);
  string msg("");
  packetManager.parseMessage(packet,msg);
  
  cout << "MESSAGE Type:"<< packet.getHeader().getType() << "- Body:msg=" << msg << endl;
}

void connect(SocketProtocol &client,PacketManager &packetManager,Packet &packet) {
  client.rcv(packet);
  string name("");
  packetManager.parseConnect(packet,name);
  
  cout << "CONNECT Type:"<< packet.getHeader().getType() << "- Body:name=" << name << endl;
}

int main(int argc, char *argv[]) {
  SocketProtocol client("0.0.0.0",5000);
  client.connect();
  if (!client)
    return 1;
  PacketManager manager(0);
  Packet packet;
  init(client,manager,packet);
  start(client,manager,packet);
  status(client,manager,packet);
  stop(client,manager,packet);
  quit(client,manager,packet);
  key(client,manager,packet);
  message(client,manager,packet);
  connect(client,manager,packet);
}
