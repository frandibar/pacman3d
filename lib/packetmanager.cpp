//implementacion de clase PacketManager
#include "common/packetmanager.h"
#include "common/stringtoken.h"
#include "common/helper.h"
#include <sstream>

void PacketManager::buildNames(const NameList &names,string &s) {
  std::ostringstream oss;
  for (NameList::const_iterator it = names.begin(); it != names.end(); it++) {
    if (it != names.begin())
      oss << "#";
    oss << it->id << "#" << it->name;
  }
  s.assign(oss.str());
}

void PacketManager::createInit(Packet &packet,const bool &pacman,const Level &level) {
  char aux = 4;//hay 2 bits de padding
  if (pacman)
    aux = 0;
  Header header(_version,INIT,aux);
  packet.setHeader(header);
  
  //grafo
  Graph *graph = level.getGraph();
  char height = graph->getHeight();
  char width = graph->getWidth();
  int graphLen = (height * width * 2) / 8;
  if ((height * width * 2) % 8 > 0)
    graphLen++;
  graphLen += 2;//sumo dos bytes para width y height
  unsigned char *bodyGraph = new unsigned char[graphLen];
  memset(bodyGraph,0,graphLen);
  //arma body de grafo
  bodyGraph[0] = width;
  bodyGraph[1] = height;
  //inserta aristas
  EdgeList* edges = graph->getEdges();
  for (EdgeList::iterator it = edges->begin();it != edges->end();it++) {
    int pos = 2 + (*it)->getID() / 8;//byte a modificar
    int bit = (*it)->getID() % 8;//bit a prender
    unsigned char mask = 1 << (7 - bit);//setea bit prendido
    bodyGraph[pos] |= mask;
  }
  delete edges;
  
  //elementos
  ElementList *elements = level.getElementList();
  short int numElements = elements->size();
  int elemLen = 2 + numElements * 3;
  unsigned char *bodyElements = new unsigned char[elemLen];
  memset(bodyElements,0,elemLen);
  //cantidad de elementos
  memcpy(bodyElements,&numElements,2);
  int cursorElem = 2;
  //elementos
  for(ElementList::iterator it = elements->begin();it != elements->end();it++) {
    bodyElements[cursorElem] = it->getType() << 2; //tipo de elemento
    bodyElements[cursorElem] |= it->getOrientation();//orientacion
    cursorElem++;
    short int idVertex = it->getIdVertex();
    memcpy(bodyElements+cursorElem,&idVertex,2);//id de vertice
    cursorElem += 2;
  }
  delete elements;
  
  //calculo de longitud de cuerpo
  int len = graphLen;
  len += elemLen;
  unsigned char* body = new unsigned char[len];
  memset(body,0,len);
  //agrego datos de grafo
  int cursor = 0;
  memcpy(body,bodyGraph,graphLen);
  delete[] bodyGraph;
  cursor += graphLen;
  //agrego datos de elementos
  memcpy(body+cursor,bodyElements,elemLen);
  delete[] bodyElements;
  cursor += elemLen;
  packet.setBody(body,len);
  delete[] body;
}

void PacketManager::createInit(Packet &packet,const bool &pacman,unsigned char* body,int bodylen) {
  char aux = 4;//hay 2 bits de padding
  if (pacman)
    aux = 0;
  Header header(_version,INIT,aux);
  packet.setHeader(header);
  packet.setBody(body,bodylen);
}

void PacketManager::createStart(Packet &packet,const short int &idClient) {
  //inserta id en cliente
  unsigned char* body = new unsigned char[2];
  memcpy(body,&idClient,2);
  Header header(_version,START,0);
  packet.setHeader(header);
  packet.setBody(body,2);
  delete[] body;
}

void PacketManager::createStatus(Packet &packet,const int &score,const PlayerStatusList &players,const ElementList &elements,const NameList &names) {
  //nombres
  string strnames("");
  buildNames(names,strnames);
  int len = 4 + players.size()*5 + 1 + elements.size()*3 + strnames.length();
  //inserta puntaje en el cuerpo
  unsigned char* body = new unsigned char[len];
  memcpy(body,&score,4);
  int cursor = 4;
  //inserta jugadores
  for(PlayerStatusList::const_iterator it = players.begin();it != players.end();it++) {
    //id de jugador
    body[cursor] = it->playerId;
    cursor ++;
    //byte de configuracion extensible
    body[cursor] = 0;
    if (it->powerUpEnabled)
      body[cursor] |= 1 << 7;
    if (it->alive)
      body[cursor] |= 1 << 6;
    cursor ++;
    //id de arista
    short int edge = it->edgeId >> 1;
    memcpy(body+cursor,&edge,2);
    cursor += 2;
    //ultimo bit de arista
    body[cursor] = (it->edgeId & 1) << 7;
    //posicion
    body[cursor] |= it->position << 1;
    //direccion
    body[cursor] |= it->direction;
    cursor ++;
  }
  //inserta cantidad de elementos
  unsigned char cant = elements.size();
  body[cursor] = cant;
  cursor++;
  //inserta elementos
  for(ElementList::const_iterator it = elements.begin();it != elements.end();it++) {
    //tipo
    body[cursor] = it->getType() << 4;
    //orientacion
    body[cursor] |= it->getOrientation() << 2;
    //estado
    body[cursor] |= it->getState();
    cursor++;
    //id de vertice
    short int idVertex = it->getIdVertex();
    memcpy(body+cursor,&idVertex,2);
    cursor += 2;
  }
  //agrega nombres
  memcpy(body+cursor,strnames.c_str(),strnames.length());
  Header header(_version,STATUS,players.size()-1);
  packet.setHeader(header);
  packet.setBody(body,len);
  delete[] body;
}

void PacketManager::createStop(Packet &packet,const tStopReason &stopReason,const int &score, const tStats &stats) {
  //calcula longitud de estadisticas
  int statlen = 6 + 2*stats.eatenGhosts.size();
  int len = 4 + statlen;
  //inserta puntaje en el cuerpo  
  unsigned char* body = new unsigned char[len];
  memcpy(body,&score,4);
  int cursor = 4;
  //inserta estadisticas en el cuerpo
  body[cursor] = stats.pills;
  cursor++;
  body[cursor] = stats.powerups;
  cursor++;
  body[cursor] = stats.bonusCounter;
  cursor++;
  body[cursor] = stats.ghostsCounter;
  cursor++;
  body[cursor] = stats.pillsEaten;
  cursor++;
  body[cursor] = stats.powerupsEaten;
  cursor++;
  for(EatenGhostsMap::const_iterator it = stats.eatenGhosts.begin(); it != stats.eatenGhosts.end();++it) {
    body[cursor] = it->first;
    body[cursor+1] = it->second;
    cursor += 2;
  }
  Header header(_version,STOP,stopReason);
  packet.setHeader(header);
  packet.setBody(body,len);
  delete[] body;
}

void PacketManager::createQuit(Packet &packet) {
  Header header(_version,QUIT,0);
  packet.setHeader(header);
  packet.setBody(0,0);
}

void PacketManager::createKey(Packet &packet,const tKeyType &key) {
  Header header(_version,KEY,key);
  packet.setHeader(header);
  packet.setBody(0,0);
}

void PacketManager::createMessage(Packet &packet,const string &msg) {
  //define cuerpo, solo puede mandar 70 bytes
  int len = msg.length();
  if (len > 70)
    len = 70;
  unsigned char *body = new unsigned char[len];
  memcpy(body,msg.c_str(),len);
  Header header(_version,MESSAGE,0);
  packet.setHeader(header);
  packet.setBody(body,len);
  delete[] body;
}

void PacketManager::createConnect(Packet &packet,const string &name) {
  //define cuerpo, solo puede mandar 8 bytes
  int len = name.length();
  if (len > 8)
    len = 8;
  unsigned char *body = new unsigned char[len];
  memcpy(body,name.c_str(),len);
  Header header(_version,CONNECT,0);
  packet.setHeader(header);
  packet.setBody(body,len);
  delete[] body;
}

void PacketManager::parsePlayerNames(const string& buffer,NameList &names) {
  StringToken token;
  string s;
  if (!token.extractToken(buffer,"#",s))
    return;
  int id = fromString<int>(s);
  if (!token.extractToken("#",s))
    return;
  tPlayerName playerName;
  playerName.id = id;
  playerName.name.assign(s);
  names.push_back(playerName);
  while (1 == 1){
    if (!token.extractToken("#",s))
      return;
    id = fromString<int>(s);
    if (!token.extractToken("#",s))
      return;
  playerName.id = id;
  playerName.name.assign(s);
  names.push_back(playerName);
  }
}

void PacketManager::parseInit(const Packet& packet,bool &isPacman,int &width,int &height,EdgeSet &edges,ElementList &elements) {
  edges.clear();
  elements.clear();
  //setea si es pacman
  isPacman = (packet.getHeader().getAux() == 0);
  unsigned char *body = packet.getBody();
  //carga grafo
  width = body[0];
  height = body[1];
  int cursor = 2;
  for (int i = 0; i < height*width*2; i++) {
    int pos = cursor + i/8;
    unsigned char mask = 1 << (7 - (i%8));
    if ((body[pos] & mask) == mask)
      edges.insert(i);//arista esta activa
  }
  cursor += (height*width*2)/8;
  if ((height*width*2)%8 != 0)
    cursor++;
  //carga elementos
  short int cant = 0;
  memcpy(&cant,body+cursor,2);
  cursor += 2;
  for (int i = 0; i < cant; i++) {
    tElementType type = (tElementType)(body[cursor] >> 2);//tipo
    tOrientation orientation = (tOrientation)(body[cursor] & 3);//orientacion
    cursor++;
    //lee id de vertice
    short int idVertex = 0;
    memcpy(&idVertex,body+cursor,2);
    cursor += 2;
    //agrega elemento
    Element elem(idVertex,type,orientation);
    elements.push_back(elem);
  }
}

void PacketManager::parseStart(const Packet& packet,int &id) {
  short int aux = 0;
  memcpy(&aux,packet.getBody(),2);
  id = aux;
}

void PacketManager::parseStatus(const Packet& packet,int &score,PlayerStatusList &positionList,ElementList &elements,NameList &namesChanged) {
  positionList.clear();
  elements.clear();
  namesChanged.clear();
  unsigned char *body = packet.getBody();
  memcpy(&score,body,4);
  //posiciones
  int cursor = 4;
  int cant = packet.getHeader().getAux() + 1;//cantidad de jugadores
  for(int i = 0;i < cant; i++) {
    tPlayerStatus playerPosition;
    //id de player
    playerPosition.playerId = body[cursor];
    cursor++;
    //para ver si esta en power up
    playerPosition.powerUpEnabled = body[cursor] & (1<<7);
    //se fija si el fantasma esta comido
    playerPosition.alive = body[cursor] & (1<<6);
    cursor++;
    //id de arista
    short int aux = 0;
    memcpy(&aux,body+cursor,2);
    cursor += 2;
    playerPosition.edgeId = aux << 1;
    if (body[cursor] & (1<<7))
      playerPosition.edgeId |= 1;
    //posicion en arista
    playerPosition.position = body[cursor] << 1;
    playerPosition.position = playerPosition.position >> 2;
    //direccion
    playerPosition.direction = body[cursor] & 1;
    cursor++;
    positionList.push_back(playerPosition);
  }
  //elementos
  unsigned char cantElem = body[cursor];
  cursor++;
  for (int i = 0; i < cantElem; i++) {
    tElementType type = (tElementType)(body[cursor] >> 4);
    tOrientation orientation = (tOrientation)((body[cursor] >> 2) & 3);
    tElementState state = (tElementState)(body[cursor] & 3);
    //vertexId
    cursor++;
    short int idVertex = 0;
    memcpy(&idVertex,body+cursor,2);
    cursor += 2;
    Element elem(idVertex,type,orientation);
    elem.setState(state);
    elements.push_back(elem);
  }
  //nombres
  string strnames((char*)(body+cursor),0,packet.getBodyLength() - cursor);
  parsePlayerNames(strnames,namesChanged);
}

void PacketManager::parseStop(const Packet& packet,tStopReason &reason,int &score, tStats &stats) {
  //reason
  reason = (tStopReason)packet.getHeader().getAux();
  unsigned char *body = packet.getBody();
  //puntaje
  memcpy(&score,body,4);
  //estadisticas
  int cursor = 4;
  //inserta estadisticas en el cuerpo
  stats.pills = body[cursor];
  cursor++;
  stats.powerups = body[cursor];
  cursor++;
  stats.bonusCounter = body[cursor];
  cursor++;
  stats.ghostsCounter = body[cursor];
  cursor++;
  stats.pillsEaten = body[cursor];
  cursor++;
  stats.powerupsEaten = body[cursor];
  cursor++;
  int len = packet.getBodyLength();
  int ghostId;
  int deaths;
  while (cursor < len) {
    ghostId = body[cursor];
    deaths = body[cursor+1];
    stats.eatenGhosts[ghostId] = deaths;
    cursor += 2;
  }
}

void PacketManager::parseQuit(const Packet& packet) {
  //vacio pues no parsea nada
}

void PacketManager::parseKey(const Packet& packet,tKeyType &key) {
  key = (tKeyType)packet.getHeader().getAux();
}

void PacketManager::parseMessage(const Packet& packet,string &msg) {
  msg.assign((char*)packet.getBody(),0,packet.getBodyLength());
}

void PacketManager::parseConnect(const Packet& packet,string &name) {
  name.assign((char*)packet.getBody(),0,packet.getBodyLength());
}
