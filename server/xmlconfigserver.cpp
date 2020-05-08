#include "xmlconfigserver.h"
#include "lib/tinyxml/tinyxml.h"
#include "lib/tinyxml/tinystr.h"
#include "lib/helper.h"

const string XmlConfigServer::XML_FILE = "serverconfig.xml";

void XmlConfigServer::save(const tConfig &config) {
  //inicializa xml
  TiXmlDocument *doc = new TiXmlDocument(XML_FILE);
  TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "no");
  doc->LinkEndChild(decl);
  //guarda root
  TiXmlElement* root = new TiXmlElement("Config");
  doc->LinkEndChild(root);
  //mundo
  TiXmlElement* world = new TiXmlElement("World");
  world->SetAttribute("value",config.worldFile.c_str());
  root->LinkEndChild(world);
  //puerto
  TiXmlElement* port = new TiXmlElement("Port");
  port->SetAttribute("value",StringConverter<int>(config.port).c_str());
  root->LinkEndChild(port);
  //direccion
  TiXmlElement* address = new TiXmlElement("Address");
  address->SetAttribute("value",config.address.c_str());
  root->LinkEndChild(address);
  //minimo
  TiXmlElement* min = new TiXmlElement("MinimumPlayers");
  min->SetAttribute("value",StringConverter<int>(config.min).c_str());
  root->LinkEndChild(min);
  //maximo
  TiXmlElement* max = new TiXmlElement("MaximumPlayers");
  max->SetAttribute("value",StringConverter<int>(config.max).c_str());
  root->LinkEndChild(max);
  //ghost comm
  TiXmlElement* ghostcomm = new TiXmlElement("GhostCommunication");
  string aux ("false");
  if (config.ghostComm)
    aux.assign("true");
  ghostcomm->SetAttribute("value",aux.c_str());
  root->LinkEndChild(ghostcomm);
  //vidas
  TiXmlElement* lives = new TiXmlElement("PacmanLives");
  lives->SetAttribute("value",StringConverter<int>(config.lives).c_str());
  root->LinkEndChild(lives);
  //logFile
  TiXmlElement* logFile = new TiXmlElement("LogFile");
  logFile->SetAttribute("value",config.logFile.c_str());
  root->LinkEndChild(logFile);
  //waiting time
  TiXmlElement* waitingTime = new TiXmlElement("WaitingTime");
  waitingTime->SetAttribute("value",StringConverter<int>(config.waitingTime).c_str());
  root->LinkEndChild(waitingTime);
  //guarda a disco
  doc->SaveFile(XML_FILE);
  delete doc;
}

bool XmlConfigServer::load(tConfig &config){
  //carga documento XML
  TiXmlDocument *doc = new TiXmlDocument(XML_FILE);
  if (!doc->LoadFile()) {
    delete doc;
    return false;
  }
  //busca root
  TiXmlHandle hDoc(doc);
  TiXmlElement* root = hDoc.FirstChildElement().Element();
  if (!root) {
    delete doc;
    return false;
  }
  TiXmlHandle hRoot(root);
  //mundo
  TiXmlElement* aux = hRoot.FirstChildElement("World").Element();
  if (!aux) {
    delete doc;
    return false;
  }
  config.worldFile = aux->Attribute("value");
  if (config.worldFile.compare("") == 0) {
    delete doc;
    return false;
  }
  //puerto
  aux = hRoot.FirstChildElement("Port").Element();
  if (!aux) {
    delete doc;
    return false;
  }
  string atrib(aux->Attribute("value"));
  if (!stringIsNumeric(atrib)) {
    delete doc;
    return false;
  }
  config.port = fromString<int>(atrib);
  //address
  aux = hRoot.FirstChildElement("Address").Element();
  if (!aux) {
    delete doc;
    return false;
  }
  config.address = aux->Attribute("value");
  if (config.address.compare("") == 0) {
    delete doc;
    return false;
  }
  //minimo
  aux = hRoot.FirstChildElement("MinimumPlayers").Element();
  if (!aux) {
    delete doc;
    return false;
  }
  atrib.assign(aux->Attribute("value"));
  if (!stringIsNumeric(atrib)) {
    delete doc;
    return false;
  }
  config.min = fromString<int>(atrib);
  //maximo
  aux = hRoot.FirstChildElement("MaximumPlayers").Element();
  if (!aux) {
    delete doc;
    return false;
  }
  atrib.assign(aux->Attribute("value"));
  if (!stringIsNumeric(atrib)) {
    delete doc;
    return false;
  }
  config.max = fromString<int>(atrib);
  //ghost comm
  aux = hRoot.FirstChildElement("GhostCommunication").Element();
  if (!aux) {
    delete doc;
    return false;
  }
  atrib.assign(aux->Attribute("value"));
  if ((atrib.compare("true") != 0) && (atrib.compare("false") != 0))  {
    delete doc;
    return false;
  }
  config.ghostComm = (atrib.compare("true") == 0);
  //vidas del pacman
  aux = hRoot.FirstChildElement("PacmanLives").Element();
  if (!aux) {
    delete doc;
    return false;
  }
  atrib.assign(aux->Attribute("value"));
  if (!stringIsNumeric(atrib)) {
    delete doc;
    return false;
  }
  config.lives = fromString<int>(atrib);
  //log file
  aux = hRoot.FirstChildElement("LogFile").Element();
  if (!aux) {
    delete doc;
    return false;
  }
  config.logFile = aux->Attribute("value");
  //waiting time
  aux = hRoot.FirstChildElement("WaitingTime").Element();
  if (!aux) {
    delete doc;
    return false;
  }
  atrib.assign(aux->Attribute("value"));
  if (!stringIsNumeric(atrib)) {
    delete doc;
    return false;
  }
  config.waitingTime = fromString<int>(atrib);
  delete doc;
  return true;
}
