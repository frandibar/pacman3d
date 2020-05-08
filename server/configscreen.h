/*cabecera de la clase ConfigScreen*/
#ifndef __CONFIGSCREEN_H__
#define __CONFIGSCREEN_H__
#include "xmlconfigserver.h"
#include "lib/world.h"
#include "lib/gladehelper.h"
#include <gtkmm.h>
#include <glade/glade.h>
#include <libglademm.h>
#include <string>
using std::string;

/**
 * Ventana GTK para setear los parametros de configuracion del Server
 * direccion y puerto donde escuchar, 
 * archivo XML del mundo a jugar,
 * archivo de log
 * cantidad minima y maxima de jugadores, etc
 * 
 * Esta ventana aparece cuando se ejecuta el server.
 * 
 */
class ConfigScreen {
  private:
    static const string GLADE_FILE;
    GladeHelper *_gladehelper;
    bool _loaded;//indica si se cargaron los parametros
    tConfig _config;//configuracion de server
    XmlConfigServer _xmlconfigserver;//manejador de xml de configuracion
    Glib::RefPtr<Gnome::Glade::Xml> _xml;//xml de glade
    //ventanas
    Gtk::Window* _windowServer;
    Gtk::Window* _windowGame;
    Gtk::Entry* _addressEntry;
    Gtk::SpinButton* _portSpin;
    Gtk::SpinButton* _minSpin;
    Gtk::SpinButton* _maxSpin;
    Gtk::CheckButton* _ghostcommCheck;
    Gtk::SpinButton* _livesSpin;
    Gtk::SpinButton* _timeSpin;
    string _lblWorldFileTitle;
    Gtk::Label* _lblWorldFile;
    string _lblLogFileTitle;
    Gtk::Label* _lblLogFile;
    Gtk::Button* _btnBrowseWorld;
    Gtk::Button* _btnBrowseLog;
    Gtk::Button* _btnStart;
    Gtk::Button* _btnNext;
    Gtk::Button* _btnBack;
    Gtk::Button* _btnClearLogFile;
    
    void on_btnStart_clicked();
    void on_btnNext_clicked();
    void on_btnBack_clicked();
    void on_btnBrowseWorld_clicked();
    void on_btnBrowseLog_clicked();
    void on_btnClearLogFile_clicked();

    bool validate();
    
  public:
  
    ///constructor q lanza pantalla de configuracion
    ConfigScreen(int &argc, char **argv, const int maxPlayers);
    
    /// destructor
    ~ConfigScreen();
    
    ///devuelve si se cargaron los parametros
    bool configLoaded() const {return _loaded;}
    
    ///devuelve la configuracion cargada
    tConfig getConfig() const {return _config;}
};

#endif /* __CONFIGSCREEN_H__ */
