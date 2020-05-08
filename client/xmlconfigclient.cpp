#include "xmlconfigclient.h"
#include "lib/tinyxml/tinyxml.h"
#include "lib/tinyxml/tinystr.h"
#include "lib/helper.h"
#include "lib/debug.h"
/*
    TAGS XML
*/
#define XMLTAG_CONFIG "Config"
#define XMLTAG_PORT "Port"
#define XMLTAG_SERVER "Server"
#define XMLTAG_SOUNDTYPE "SoundType"
#define XMLTAG_PLAYERNAME "PlayerName"

#define XMLTAG_MOVEMENTKEYS "MovementKeys"
#define XMLTAG_MOVEMENTKEY_FWD "MovementKeyForward"
#define XMLTAG_MOVEMENTKEY_BACK "MovementKeyBack"
#define XMLTAG_MOVEMENTKEY_LEFT "MovementKeyLeft"
#define XMLTAG_MOVEMENTKEY_RIGHT "MovementKeyRight"

#define XMLTAG_SHORTCUTMESSAGES "ShortCutMessages"
#define XMLTAG_SHORTCUTMESSAGE "ShortCutMessage"

#define XMLTAG_ATTR_SHORTCUTKEY "shortcutkey"

#define XMLTAG_ATTR_VALUE "value"


const string XmlConfigClient::XML_FILE = "clientconfig.xml";

void XmlConfigClient::save(const tConfigClient & config) {

	//inicializa xml
	TiXmlDocument *doc = new TiXmlDocument(XML_FILE);
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "no");
	doc->LinkEndChild(decl);
  
	//guarda root
	TiXmlElement* root = new TiXmlElement(XMLTAG_CONFIG);
	doc->LinkEndChild(root);
  
	//puerto
	TiXmlElement* port = new TiXmlElement(XMLTAG_PORT);
	port->SetAttribute(XMLTAG_ATTR_VALUE, StringConverter<int>(config.port).c_str());
	root->LinkEndChild(port);
  
	//direccion
	TiXmlElement* address = new TiXmlElement(XMLTAG_SERVER);
	address->SetAttribute(XMLTAG_ATTR_VALUE, config.address.c_str());
	root->LinkEndChild(address);

	//sound type
	TiXmlElement* soundtype = new TiXmlElement(XMLTAG_SOUNDTYPE);
	soundtype->SetAttribute(XMLTAG_ATTR_VALUE, StringConverter<int>(config.soundType).c_str());
	root->LinkEndChild(soundtype);

	//playername
	TiXmlElement* playerName = new TiXmlElement(XMLTAG_PLAYERNAME);
	playerName->SetAttribute(XMLTAG_ATTR_VALUE, config.playerName);
	root->LinkEndChild(playerName);

	// Movement Keys:
	TiXmlElement* movementKeys = new TiXmlElement(XMLTAG_MOVEMENTKEYS);
  
	TiXmlElement* movementKeyFwd = new TiXmlElement(XMLTAG_MOVEMENTKEY_FWD);
	movementKeyFwd->SetAttribute(XMLTAG_ATTR_VALUE, StringConverter<int>(config.movementKeys.forward));
	movementKeys->LinkEndChild(movementKeyFwd);
  
	TiXmlElement* movementKeyBack = new TiXmlElement(XMLTAG_MOVEMENTKEY_BACK);
	movementKeyBack->SetAttribute(XMLTAG_ATTR_VALUE, StringConverter<int>(config.movementKeys.turnBack));
	movementKeys->LinkEndChild(movementKeyBack);
  
	TiXmlElement* movementKeyLeft = new TiXmlElement(XMLTAG_MOVEMENTKEY_LEFT);
	movementKeyLeft->SetAttribute(XMLTAG_ATTR_VALUE, StringConverter<int>(config.movementKeys.left));
	movementKeys->LinkEndChild(movementKeyLeft);
  
	TiXmlElement* movementKeyRight = new TiXmlElement(XMLTAG_MOVEMENTKEY_RIGHT);
	movementKeyRight->SetAttribute(XMLTAG_ATTR_VALUE, StringConverter<int>(config.movementKeys.right));
	movementKeys->LinkEndChild(movementKeyRight);
  
	root->LinkEndChild(movementKeys);
  
	// Shortcut messages
	TiXmlElement* shortCutMesssages = new TiXmlElement(XMLTAG_SHORTCUTMESSAGES);

	for(ShortcutMap::const_iterator it = config.messageMap.begin(); it != config.messageMap.end(); ++it) {
		TiXmlElement* messageEntry = new TiXmlElement(XMLTAG_SHORTCUTMESSAGE);
		messageEntry->SetAttribute(XMLTAG_ATTR_SHORTCUTKEY, StringConverter<int>((int)(*it).first));
		TiXmlText * text = new TiXmlText( (*it).second );
		messageEntry->LinkEndChild( text );		
		
		shortCutMesssages->LinkEndChild(messageEntry);
	}
  
	root->LinkEndChild(shortCutMesssages);
  
	//guarda a disco
	doc->SaveFile(XML_FILE);
	delete doc;

}

bool XmlConfigClient::load(tConfigClient & config) {

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
  
  //puerto
  TiXmlElement * aux = hRoot.FirstChildElement(XMLTAG_PORT).Element();
  if (!aux) {
    delete doc;
    return false;
  }
  string atrib(aux->Attribute(XMLTAG_ATTR_VALUE));
  if (!stringIsNumeric(atrib)) {
    delete doc;
    return false;
  }
  config.port = fromString<int>(atrib);
  
  // server
  aux = hRoot.FirstChildElement(XMLTAG_SERVER).Element();
  if (!aux) {
    delete doc;
    return false;
  }
  config.address = aux->Attribute(XMLTAG_ATTR_VALUE);
  if (config.address.compare("") == 0) {
    delete doc;
    return false;
  }
  
	// Sound Type
	aux = hRoot.FirstChildElement(XMLTAG_SOUNDTYPE).Element();
	if (!aux) {
		delete doc;
		return false;
	}
	atrib.assign(aux->Attribute(XMLTAG_ATTR_VALUE));
	if (!stringIsNumeric(atrib)) {
		delete doc;
		return false;
	}
	config.soundType = (tSoundType)fromString<int>(atrib);
  
	// playername
	aux = hRoot.FirstChildElement(XMLTAG_PLAYERNAME).Element();
	if (!aux) {
		delete doc;
		return false;
	}
	config.playerName = aux->Attribute(XMLTAG_ATTR_VALUE);
	if (config.playerName.compare("") == 0) {
		delete doc;
		return false;
	}
  
	// Movement Keys (inicializa en CERO)
	config.movementKeys.forward  = (SDLKey) 0;
	config.movementKeys.turnBack = (SDLKey) 0;
	config.movementKeys.left     = (SDLKey) 0;
	config.movementKeys.right    = (SDLKey) 0;
	aux = hRoot.FirstChildElement(XMLTAG_MOVEMENTKEYS).Element();
	if (aux) {
  		TiXmlHandle hAux(aux);
  		TiXmlElement * moveFwd = hAux.FirstChildElement(XMLTAG_MOVEMENTKEY_FWD).Element();
  		if (moveFwd) {
			atrib.assign(moveFwd->Attribute(XMLTAG_ATTR_VALUE));
			if (stringIsNumeric(atrib)) {
				config.movementKeys.forward = (SDLKey)fromString<int>(atrib);
			}
  		}
		TiXmlElement * moveBack = hAux.FirstChildElement(XMLTAG_MOVEMENTKEY_BACK).Element();
		if (moveBack) {
			atrib.assign(moveBack->Attribute(XMLTAG_ATTR_VALUE));
			if (stringIsNumeric(atrib)) {
				config.movementKeys.turnBack = (SDLKey)fromString<int>(atrib);
			}
  		}
		TiXmlElement * moveLeft = hAux.FirstChildElement(XMLTAG_MOVEMENTKEY_LEFT).Element();
		if (moveLeft) {
			atrib.assign(moveLeft->Attribute(XMLTAG_ATTR_VALUE));
			if (stringIsNumeric(atrib)) {
				config.movementKeys.left = (SDLKey)fromString<int>(atrib);
			}
  		}
  		TiXmlElement * moveRight = hAux.FirstChildElement(XMLTAG_MOVEMENTKEY_RIGHT).Element();
  		if (moveRight) {
			atrib.assign(moveRight->Attribute(XMLTAG_ATTR_VALUE));
			if (stringIsNumeric(atrib)) {
				config.movementKeys.right = (SDLKey)fromString<int>(atrib);
			}
  		}
	}
  
	// Shortcut messages:
	aux = hRoot.FirstChildElement(XMLTAG_SHORTCUTMESSAGES).Element();
	
	if (aux) {
		TiXmlHandle messagesHandle(aux);
		TiXmlElement * xmlMessage = messagesHandle.FirstChild( XMLTAG_SHORTCUTMESSAGE ).Element();
		
		for( ; xmlMessage; xmlMessage = xmlMessage->NextSiblingElement()) {	
			atrib.assign(xmlMessage->Attribute(XMLTAG_ATTR_SHORTCUTKEY));
			if (stringIsNumeric(atrib)) {
				config.messageMap[(SDLKey)fromString<int>(atrib)] = (xmlMessage->GetText() ? xmlMessage->GetText() : "");
			}		
		}	
		
	}
  
  
	delete doc;
	return true;
}
