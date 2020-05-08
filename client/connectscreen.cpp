#include "connectscreen.h"
#include "lib/helper.h"
#include "lib/utilities.h"
#include "lib/debug.h"

const int ConnectScreen::ACCEPT_WAIT = 1000;
const string ConnectScreen::STR_MODERN = "Modern";
const string ConnectScreen::STR_CLASSIC = "Classic";
const string ConnectScreen::GLADE_FILE = "/etc/Pacman3D/img/clientconfig.glade";

//constructor
ConnectScreen::ConnectScreen(int &argc, char **argv) {
  _socket = 0;
  _connected = false;
  Gtk::Main kit(argc,argv);
  //carga xml de glade
  _xml = Gnome::Glade::Xml::create(GLADE_FILE);
  _xml->get_widget("window",_window);
  _xml->get_widget("entryServer",_serverEntry);
  _xml->get_widget("spinPort",_portSpin);
  _xml->get_widget("comboSound",_soundCombo);
  _soundCombo->set_active(0);
  _xml->get_widget("entryName",_nameEntry);
  _xml->get_widget("btnConnect",_btnConnect);
  _btnConnect->signal_clicked().connect(sigc::mem_fun(*this,&ConnectScreen::on_btnConnect_clicked));

  _xml->get_widget("eventboxMove",_eventboxMove);
  _eventboxMove->set_events(Gdk::BUTTON_PRESS_MASK | Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK);
  _eventboxMove->signal_button_press_event().connect(
    sigc::mem_fun(*this, &ConnectScreen::on_eventboxMove_press));
  _eventboxMove->signal_enter_notify_event().connect(sigc::mem_fun(*this, &ConnectScreen::on_eventboxMove_enter_notify));
  _eventboxMove->signal_leave_notify_event().connect(sigc::mem_fun(*this, &ConnectScreen::on_eventboxMove_leave_notify));

  _xml->get_widget("eventboxMsg",_eventboxMsg);
  _eventboxMsg->set_events(Gdk::BUTTON_PRESS_MASK | Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK);
  _eventboxMsg->signal_button_press_event().connect(sigc::mem_fun(*this,&ConnectScreen::on_eventboxMsg_press));
  _eventboxMsg->signal_enter_notify_event().connect(sigc::mem_fun(*this, &ConnectScreen::on_eventboxMsg_enter_notify));
  _eventboxMsg->signal_leave_notify_event().connect(sigc::mem_fun(*this, &ConnectScreen::on_eventboxMsg_leave_notify));


  //carga xml de configuracion
  if (XmlConfigClient::load(_config)) {
    _serverEntry->set_text(_config.address);
    _portSpin->set_value(_config.port);
    switch (_config.soundType) {
      case MODERN:
        _soundCombo->set_active(0);
        break;
      case CLASSIC:
        _soundCombo->set_active(1);
        break;
      case MUTE:
        _soundCombo->set_active(2);
    }
    _nameEntry->set_text(_config.playerName);
    // Si no cargo algun valor para los movement keys, seteamos el default:
    if (_config.movementKeys.forward == 0)
    	_config.movementKeys.forward = SDLK_UP;
    if (_config.movementKeys.turnBack == 0)
    	_config.movementKeys.turnBack = SDLK_DOWN;
    if (_config.movementKeys.left == 0)
    	_config.movementKeys.left = SDLK_LEFT;
    if (_config.movementKeys.right == 0)
    	_config.movementKeys.right = SDLK_RIGHT;
    
  } else {
  	// Seteamos los movement keys default si no pudo cargar la config XML:
   	_config.movementKeys.forward = SDLK_UP;
   	_config.movementKeys.turnBack = SDLK_DOWN;
   	_config.movementKeys.left = SDLK_LEFT;
   	_config.movementKeys.right = SDLK_RIGHT;
  }


  _gladeHelper = new GladeHelper;
  loadWidgetsMsg();
  loadWidgetsMove();
  //muestra pantalla de bienvenida
  showWelcome();
}

ConnectScreen::~ConnectScreen(){
  delete _gladeHelper;
}

void ConnectScreen::showWelcome() {
  _xml->get_widget("windowWelcome",_windowWelcome);
  _xml->get_widget("btnWelcomePlay",_btnWelcomePlay);
  _btnWelcomePlay->signal_clicked().connect(sigc::mem_fun(*this,&ConnectScreen::on_btnWelcomePlay_show));
  Gtk::Main::run(*_windowWelcome);
}

void ConnectScreen::on_btnWelcomePlay_show() {
  _window->show();
  _windowWelcome->hide();
  // El foco lo ponemos en el boton Connect
  _btnConnect->grab_focus();

  Gtk::Main::run(*_window);
}

//valida entrada de datos
bool ConnectScreen::validate() {
  _serverEntry->show();
  _portSpin->show();
  _soundCombo->show();
  _nameEntry->show();
  //direccion de server
  string server(_serverEntry->get_text());
  if (server.compare("") == 0) {
    _gladeHelper->showWarningDialog(*_window,"Missing Parameter!","Type in server's address.");
    _serverEntry->grab_focus();
    return false;
  }
  //sonido
  _sound = (tSoundType)_soundCombo->get_active_row_number();
  //nombre de jugador
  _playerName.assign(_nameEntry->get_text());
  if (_playerName.compare("") == 0) {
    _gladeHelper->showWarningDialog(*_window,"Missing Parameter!","Type in name.");
    _nameEntry->grab_focus();
    return false;
  }
  _config.playerName.assign(_playerName);

  // server
  _config.address.assign(_serverEntry->get_text());
  // puerto
  // Validamos primero que sea un nro correcto:
  int portNmbr = (int)_portSpin->get_value();
  if (portNmbr < MINIMUM_ALLOWED_PORT) {
    _gladeHelper->showWarningDialog(*_window,"Invalid port number!", "Port number must be greater than " + StringConverter<int>(MINIMUM_ALLOWED_PORT) );
    _portSpin->grab_focus();
    return false;
  }
  _config.port = portNmbr;
  // sound type
  _config.soundType = _sound;
	// Movement Keys:
	_config.movementKeys = getMovementKeys();
	
	// Shortcut messagex
	_config.messageMap = getShortcuts();
	
  return true;
}

//funcion callback para atender el botón
void ConnectScreen::on_btnConnect_clicked() {

	// SI no pasa la validacion no hacemos nada...
	if (! validate()) return;

	//guarda configuracion en xml
	XmlConfigClient::save(_config);
	//realiza conexion
	string server(_serverEntry->get_text());
	int port = (int)_portSpin->get_value();
	_socket = new SocketProtocol(server,port);
	_socket->connect();
	if (!*_socket) {
	  _gladeHelper->showErrorDialog(*_window,"Connection Error!","Client cannot establish a connection with server.");
	  delete _socket;
	  _socket = 0;
	  return;
	}
	_connected = true;
	//termina con el screen
	_window->hide();
	Gtk::Main::quit();

}

bool ConnectScreen::on_eventboxMsg_press(GdkEventButton* event) {
  _windowMsg->show();
  return true;
}

bool ConnectScreen::on_eventboxMove_press(GdkEventButton* event) {
  _windowMove->show();
  return true;
}

bool ConnectScreen::on_eventboxMsg_enter_notify(GdkEventCrossing* event) {
	_window->get_window()->set_cursor (Gdk::Cursor(Gdk::HAND1));
	return true;
}

bool ConnectScreen::on_eventboxMove_enter_notify(GdkEventCrossing* event) {
	_window->get_window()->set_cursor (Gdk::Cursor(Gdk::HAND1));
	return true;
}


bool ConnectScreen::on_eventboxMsg_leave_notify(GdkEventCrossing* event) {
	_window->get_window()->set_cursor();
	return true;
}


bool ConnectScreen::on_eventboxMove_leave_notify(GdkEventCrossing* event) {
	_window->get_window()->set_cursor();
	return true;
}

void ConnectScreen::loadWidgetsMsg() {
	
	_xml->get_widget("windowMsg",_windowMsg);
	_xml->get_widget("entryF4",_entryF4);
	_xml->get_widget("entryF5",_entryF5);
	_xml->get_widget("entryF6",_entryF6);
	_xml->get_widget("entryF7",_entryF7);
	_xml->get_widget("entryF8",_entryF8);
	_xml->get_widget("entryF9",_entryF9);
	_xml->get_widget("entryF10",_entryF10);
	_xml->get_widget("entryF11",_entryF11);
	_xml->get_widget("btnMsgOK",_btnMsgOK);
	_btnMsgOK->signal_clicked().connect(sigc::mem_fun(*this,&ConnectScreen::on_btnMsgOK_clicked));
  
  	// Si la configuracion inicial define mensajes, los seteamos: 
	for(ShortcutMap::const_iterator it = _config.messageMap.begin(); it != _config.messageMap.end(); ++it) {
		
		switch ((*it).first) {
			case SDLK_F4:
				_entryF4->set_text( (*it).second );
				break;
			case SDLK_F5:
				_entryF5->set_text( (*it).second );
				break;
			case SDLK_F6:
				_entryF6->set_text( (*it).second );
				break;
			case SDLK_F7:
				_entryF7->set_text( (*it).second );
				break;
			case SDLK_F8:
				_entryF8->set_text( (*it).second );
				break;
			case SDLK_F9:
				_entryF9->set_text( (*it).second );
				break;
			case SDLK_F10:
				_entryF10->set_text( (*it).second );
				break;
			case SDLK_F11:
				_entryF11->set_text( (*it).second );
				break;
			default:
				break;
		}

	}   
  
}

void ConnectScreen::on_btnMsgOK_clicked() { 
  _windowMsg->hide();
}

//devuelve mensajes para cada tecla
ShortcutMap ConnectScreen::getShortcuts() const {
  ShortcutMap map;
  map[SDLK_F4] = _entryF4->get_text();
  map[SDLK_F5] = _entryF5->get_text();
  map[SDLK_F6] = _entryF6->get_text();
  map[SDLK_F7] = _entryF7->get_text();
  map[SDLK_F8] = _entryF8->get_text();
  map[SDLK_F9] = _entryF9->get_text();
  map[SDLK_F10] = _entryF10->get_text();
  map[SDLK_F11] = _entryF11->get_text();
  return map;
}

void ConnectScreen::loadWidgetsMove() {
  _xml->get_widget("windowMove",_windowMove);
  _xml->get_widget("comboForward",_comboForward);
  _xml->get_widget("comboLeft",_comboLeft);
  _xml->get_widget("comboRight",_comboRight);
  _xml->get_widget("comboTurnBack",_comboTurnBack);
  _xml->get_widget("btnMoveOK",_btnMoveOK);
  _btnMoveOK->signal_clicked().connect(sigc::mem_fun(*this,&ConnectScreen::on_btnMoveOK_clicked));
  populateKeyCombos();
}

void ConnectScreen::on_btnMoveOK_clicked() {
	
	tMovementKeys tmk;
	DEBUG_0<int>(tmk.forward);
	DEBUG_0<int>(tmk.left);
	
  //valida que no haya teclas repetidas
  std::map<int,bool> usedKeys;
  //forward
  Gtk::TreeModel::iterator iter = _comboForward->get_active();
  Gtk::TreeModel::Row row = *iter;
  usedKeys[row[_model.col_sdlkey]] = true;
  //left
  iter = _comboLeft->get_active();
  row = *iter;
  if (usedKeys.find(row[_model.col_sdlkey]) != usedKeys.end()) {
    _gladeHelper->showErrorDialog(*_windowMove,"Duplicated keys!","A key can only be used to move in one direction");
    return;
  }
  usedKeys[row[_model.col_sdlkey]] = true;
  //right
  iter = _comboRight->get_active();
  row = *iter;
  if (usedKeys.find(row[_model.col_sdlkey]) != usedKeys.end()) {
    _gladeHelper->showErrorDialog(*_windowMove,"Duplicated keys!","A key can only be used to move in one direction");
    return;
  }
  usedKeys[row[_model.col_sdlkey]] = true;
  //back
  iter = _comboTurnBack->get_active();
  row = *iter;
  if (usedKeys.find(row[_model.col_sdlkey]) != usedKeys.end()) {
    _gladeHelper->showErrorDialog(*_windowMove,"Duplicated keys!","A key can only be used to move in one direction");
    return;
  }
  usedKeys[row[_model.col_sdlkey]] = true;
  //si no hubo erro cierra ventana 
  _windowMove->hide();
}

void ConnectScreen::addKey(Glib::RefPtr<Gtk::ListStore> &refListStore,const SDLKey &key,const string &desc) {
  Gtk::TreeModel::iterator iter = refListStore->append();
  Gtk::TreeModel::Row row = *iter;
  row[_model.col_sdlkey] = (int)key;
  row[_model.col_desc] = desc;
  
  if (key == _config.movementKeys.forward) { 
  	_comboForward->set_active(iter);
  }else if (key == _config.movementKeys.left) {
  	_comboLeft->set_active(iter);
  }else if (key == _config.movementKeys.right) {
  	_comboRight->set_active(iter);
  }else if (key == _config.movementKeys.turnBack) {
  	_comboTurnBack->set_active(iter);
  }
  
}

void ConnectScreen::populateKeyCombos() {
  Glib::RefPtr<Gtk::ListStore> refListStore = Gtk::ListStore::create(_model);
  //setea modelo
  _comboForward->set_model(refListStore);
  _comboLeft->set_model(refListStore);
  _comboRight->set_model(refListStore);
  _comboTurnBack->set_model(refListStore);
  //setea columnas a mostrar
  _comboForward->pack_start(_model.col_desc);
  _comboLeft->pack_start(_model.col_desc);
  _comboRight->pack_start(_model.col_desc);
  _comboTurnBack->pack_start(_model.col_desc);
  //agrega teclas
  addKey(refListStore,SDLK_BACKSPACE,"<backspace>");
  addKey(refListStore,SDLK_TAB,"<tab>");
  addKey(refListStore,SDLK_RETURN,"<return>");
  addKey(refListStore,SDLK_SPACE,"<spacebar>");
  addKey(refListStore,SDLK_COMMA,",");
  addKey(refListStore,SDLK_MINUS,"-");
  addKey(refListStore,SDLK_PERIOD,".");
  addKey(refListStore,SDLK_0,"0");
  addKey(refListStore,SDLK_1,"1");
  addKey(refListStore,SDLK_2,"2");
  addKey(refListStore,SDLK_3,"3");
  addKey(refListStore,SDLK_4,"4");
  addKey(refListStore,SDLK_5,"5");
  addKey(refListStore,SDLK_6,"6");
  addKey(refListStore,SDLK_7,"7");
  addKey(refListStore,SDLK_8,"8");
  addKey(refListStore,SDLK_9,"9");
  addKey(refListStore,SDLK_a,"a");
  addKey(refListStore,SDLK_b,"b");
  addKey(refListStore,SDLK_c,"c");
  addKey(refListStore,SDLK_d,"d");
  addKey(refListStore,SDLK_e,"e");
  addKey(refListStore,SDLK_f,"f");
  addKey(refListStore,SDLK_g,"g");
  addKey(refListStore,SDLK_h,"h");
  addKey(refListStore,SDLK_i,"i");
  addKey(refListStore,SDLK_j,"j");
  addKey(refListStore,SDLK_k,"k");
  addKey(refListStore,SDLK_l,"l");
  addKey(refListStore,SDLK_m,"m");
  addKey(refListStore,SDLK_n,"n");
  addKey(refListStore,SDLK_o,"o");
  addKey(refListStore,SDLK_p,"p");
  addKey(refListStore,SDLK_q,"q");
  addKey(refListStore,SDLK_r,"r");
  addKey(refListStore,SDLK_s,"s");
  addKey(refListStore,SDLK_t,"t");
  addKey(refListStore,SDLK_u,"u");
  addKey(refListStore,SDLK_v,"v");
  addKey(refListStore,SDLK_w,"w");
  addKey(refListStore,SDLK_x,"x");
  addKey(refListStore,SDLK_y,"y");
  addKey(refListStore,SDLK_z,"z");
  addKey(refListStore,SDLK_UP,"<up>");
  addKey(refListStore,SDLK_DOWN,"<down>");
  addKey(refListStore,SDLK_RIGHT,"<right>");
  addKey(refListStore,SDLK_LEFT,"<left>");
  addKey(refListStore,SDLK_RSHIFT,"<right shift>");
  addKey(refListStore,SDLK_LSHIFT,"<left shift>");
  addKey(refListStore,SDLK_RCTRL,"<right ctrl>");
  addKey(refListStore,SDLK_LCTRL,"<left ctrl>");
  addKey(refListStore,SDLK_RALT,"<right alt>");
  addKey(refListStore,SDLK_LALT,"<left alt>");
}

tMovementKeys ConnectScreen::getMovementKeys() {
  tMovementKeys keys;
  //forward
  Gtk::TreeModel::iterator iter = _comboForward->get_active();
  Gtk::TreeModel::Row row = *iter;
  keys.forward = (SDLKey)((int)row[_model.col_sdlkey]);
  //left
  iter = _comboLeft->get_active();
  row = *iter;
  keys.left = (SDLKey)((int)row[_model.col_sdlkey]);
  //right
  iter = _comboRight->get_active();
  row = *iter;
  keys.right = (SDLKey)((int)row[_model.col_sdlkey]);
  //turn back
  iter = _comboTurnBack->get_active();
  row = *iter;
  keys.turnBack = (SDLKey)((int)row[_model.col_sdlkey]);
  return keys;
}
