//definicion de metodos de clase ConfigScreen
#include "configscreen.h"
#include "lib/helper.h"
#include "lib/xmlworld.h"
#include "lib/gladehelper.h"
#include "lib/utilities.h"
#include "lib/debug.h"

const string ConfigScreen::GLADE_FILE = "/etc/Pacman3D/img/server.glade";

ConfigScreen::ConfigScreen(int &argc, char **argv, const int maxPlayers) {
  _loaded = false;
  Gtk::Main kit(argc,argv);
  //carga xml de glade
  _xml = Gnome::Glade::Xml::create(GLADE_FILE);
  //ventanas
  _xml->get_widget("windowServer",_windowServer);
  _xml->get_widget("windowGame",_windowGame);
  _xml->get_widget("lblWorldFile",_lblWorldFile);
  _lblWorldFileTitle = _lblWorldFile->get_text();
  _xml->get_widget("btnBrowseWorld",_btnBrowseWorld);
  _btnBrowseWorld->signal_clicked().connect(sigc::mem_fun(*this,&ConfigScreen::on_btnBrowseWorld_clicked));
  _xml->get_widget("btnBrowseLog",_btnBrowseLog);
  _btnBrowseLog->signal_clicked().connect(sigc::mem_fun(*this,&ConfigScreen::on_btnBrowseLog_clicked));
  _xml->get_widget("entryAddress",_addressEntry);
  _xml->get_widget("spinPort",_portSpin);
  _xml->get_widget("spinMin",_minSpin);
  _xml->get_widget("spinMax",_maxSpin);
  _maxSpin->set_range(2,maxPlayers);
  _maxSpin->set_value(5);
  _xml->get_widget("spinLives",_livesSpin);
  _xml->get_widget("lblLogFile",_lblLogFile);
  _lblLogFileTitle = _lblLogFile->get_text();
  _xml->get_widget("btnClearLogFile",_btnClearLogFile);
  _btnClearLogFile->signal_clicked().connect(sigc::mem_fun(*this,&ConfigScreen::on_btnClearLogFile_clicked));
  _xml->get_widget("chkGhostComm",_ghostcommCheck);
  _xml->get_widget("spinTime",_timeSpin);
  _xml->get_widget("btnStart",_btnStart);
  _btnStart->signal_clicked().connect(sigc::mem_fun(*this,&ConfigScreen::on_btnStart_clicked));
  _xml->get_widget("btnNext",_btnNext);
  _btnNext->signal_clicked().connect(sigc::mem_fun(*this,&ConfigScreen::on_btnNext_clicked));
  _xml->get_widget("btnBack",_btnBack);
  _btnBack->signal_clicked().connect(sigc::mem_fun(*this,&ConfigScreen::on_btnBack_clicked));
  //carga xml de configuracion
  if (_xmlconfigserver.load(_config)) {
  	if ( _config.worldFile.size() != 0 )
    	_lblWorldFile->set_text(_config.worldFile);
    _addressEntry->set_text(_config.address);
    _portSpin->set_value(_config.port);
    _minSpin->set_value(_config.min);
    _maxSpin->set_value(_config.max);
    _livesSpin->set_value(_config.lives);
    if ( _config.logFile.size() != 0 )
    	_lblLogFile->set_text(_config.logFile);
    _ghostcommCheck->set_active(_config.ghostComm);
    _timeSpin->set_value(_config.waitingTime);
  }
  _gladehelper = new GladeHelper;
  _btnNext->grab_focus();
  Gtk::Main::run(*_windowServer);
}

ConfigScreen::~ConfigScreen() {
  delete _gladehelper;
}

//valida entrada de datos
bool ConfigScreen::validate() {
  _portSpin->show();
  _minSpin->show();
  _maxSpin->show();
  _livesSpin->show();
  _timeSpin->show();
  //direccion
  string aux(_addressEntry->get_text());
  if (aux.compare("") == 0) {
    _gladehelper->showWarningDialog(*_windowGame,"Missing Parameter!","Select server's address.");
    _addressEntry->grab_focus();
    return false;
  }
  _config.address.assign(aux);
  
  //puerto
  // Validamos primero que sea un nro correcto:
  int portNmbr = (int)_portSpin->get_value();
  if (portNmbr < MINIMUM_ALLOWED_PORT) {
    _gladehelper->showWarningDialog(*_windowGame,"Invalid port number!", "Port number must be greater than " + StringConverter<int>(MINIMUM_ALLOWED_PORT) );
    _portSpin->grab_focus();
    return false;
  }
  _config.port = portNmbr;
  
  //mundo
  if ( _lblWorldFile->get_text() == _lblWorldFileTitle ){
  	aux.assign("");
  }else{
  	aux.assign(_lblWorldFile->get_text());
  }  
  if (aux.compare("") == 0) {
    _gladehelper->showWarningDialog(*_windowGame,"Missing Parameter!","Select the XML world.");
    _lblWorldFile->grab_focus();
    return false;
  }
  _config.worldFile.assign(aux);
  //log file
  if ( _lblLogFile->get_text() == _lblLogFileTitle ){
  	_config.logFile.assign("");
  }else{
  	_config.logFile.assign(_lblLogFile->get_text());  	
  }
  //minimo de jugadores
  _config.min = (int)_minSpin->get_value();
  //maximo de jugadores
  _config.max = (int)_maxSpin->get_value();
  if (_config.max < _config.min){
    _gladehelper->showWarningDialog(*_windowGame,"Invalid Range!","Maximum is smaller than minimum.");
    _maxSpin->grab_focus();
    return false;
  }
  //comunicacion entre fantasmas
  _config.ghostComm = _ghostcommCheck->get_active();
  //vidas
  _config.lives = (int)_livesSpin->get_value();
  //tiempo de espera
  _config.waitingTime = (int)_timeSpin->get_value();
  return true;
}

void ConfigScreen::on_btnStart_clicked() {
  if (!validate())
    return;
  World world;
  try {
  	XmlWorld::Load(_config.worldFile,world);
  } catch (string & e) {
    // _gladehelper->showErrorDialog(*_windowGame,"Invalid World File!","The world file is corrupt.");
    _gladehelper->showErrorDialog(*_windowGame,"Invalid World File!", e);
    _lblWorldFile->grab_focus();
    return;
  }    
  if (world.getLevels()->size() == 0) {
    _gladehelper->showErrorDialog(*_windowGame,"Invalid World File!","The world has no levels.");
    _lblWorldFile->grab_focus();
    return;
  }
  _loaded = true;
  //guarda configuracion en xml
  _xmlconfigserver.save(_config);
  //termina con el screen
  _windowGame->hide();
  Gtk::Main::quit();
}

void ConfigScreen::on_btnNext_clicked() {
  _windowGame->show();
  _windowServer->hide();
  Gtk::Main::run(*_windowGame);
  _btnStart->grab_focus();
}

void ConfigScreen::on_btnBack_clicked() {
  _windowServer->show();
  _windowGame->hide();
  Gtk::Main::run(*_windowServer);
  _btnNext->grab_focus();
}

void ConfigScreen::on_btnBrowseWorld_clicked() {
  string filename("");
  if (_gladehelper->showFileChooserDialog(*_windowServer,false,"Select World File...","World files | *.world","*.world",filename))
    _lblWorldFile->set_text(filename);
}

void ConfigScreen::on_btnBrowseLog_clicked() {
  string filename("");
  if (_gladehelper->showFileChooserDialog(*_windowServer,true,"Save Log File...","Log files | *","*",filename))
    _lblLogFile->set_text(filename);
}

void ConfigScreen::on_btnClearLogFile_clicked(){
	_lblLogFile->set_text(_lblLogFileTitle);
}

