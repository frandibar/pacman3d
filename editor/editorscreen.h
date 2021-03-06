#ifndef __EDITORSCREEN_H__
#define __EDITORTSCREEN_H__
#include "lib/utilities.h"
#include "lib/gladehelper.h"
#include <gtkmm.h>
#include <glade/glade.h>
#include <libglademm.h>
#include <string>
#include "editor/tablemanager.h"
#include "lib/graphhandler.h"
#include "lib/pacmanmap.h"
#include "lib/helper.h"
using std::string;

#ifndef IMAGE_FILES
#define IMAGE_FILES 24
#endif

/**
 * Esta clase se encarga de contener todos los widgets y toda la lógica de armado de mapas y mundos. Tomando todos los widgets que entrega el archivo glade previamente hecho, les asigna algunos de sus métodos a cada uno para así relacionarlos con las distintas señales que éstos soportan.
 * Para la parte de edición de mapas, se optó por tener una GtkTable para el armado del mapa. La GtkTable proporciona una matriz de posiciones donde pueden colocarse GtkWidgets. Lo que se pensó hacer es colocar en estas posiciones imágenes con los elementos que se deseaban colocar para ir formando el mapa. Dado que los GtkImage no reciben eventos, debieron ser wrappeados por objetos GtkEventBox, los cuales si reciben eventos (en nuestro caso, el evento requerido era un button clic, un movimiento y un button release del mouse). Entonces lo que se decidió hacer es que en cada posición de la GtkTable, haya un GtkEventBox, con un GtkImage dentro. Cada GtkEventBox va a captar el evento de click del mouse sobre si mismo, y va a hacer que la GtkImage que contiene cambie su dibujo por el escogido.
 */
class EditorScreen {
  private:
  	static const string imageFiles[IMAGE_FILES];
  	static const string GLADE_FILE;
	  static const string SPEED_LOW;
	  static const string SPEED_MEDIUM;
	  static const string SPEED_HIGH;    
    static const int SIZE_SMALL;
    static const int SIZE_MEDIUM;
    static const int SIZE_LARGE;
    static const int SIZE_GIANT;
    static const tCell DEFAULT_CELL;
    GladeHelper *_gladeHelper;
    unsigned int _size;
    bool _invalidSize;
    string _worldSaveFile;
    string _mapSaveFile;
    Glib::RefPtr<Gnome::Glade::Xml> _xml;//xml de glade
    //windows
    Gtk::Window* _windowWorld;
    Gtk::Window* _windowMap;    
    Gtk::Window* _windowHome;
    Gtk::Window* _windowAddMap;
    Gtk::Window* _windowNewMap;
    //home
    Gtk::Button* _btnHomeClose;
    Gtk::EventBox* _eventboxMap;
    Gtk::EventBox* _eventboxWorld;
    //toolbar
    Gtk::RadioToolButton* _btnPill;
    Gtk::RadioToolButton* _btnPowerUp;
    Gtk::RadioToolButton* _btnBonus;
    Gtk::RadioToolButton* _btnGhostHouseN;
    Gtk::RadioToolButton* _btnGhostHouseE;
    Gtk::RadioToolButton* _btnGhostHouseS;
    Gtk::RadioToolButton* _btnGhostHouseW;
    Gtk::RadioToolButton* _btnPacmanStart;
    Gtk::RadioToolButton* _btnGateV;
    Gtk::RadioToolButton* _btnGateH;
    Gtk::RadioToolButton* _btnWall;
    Gtk::RadioToolButton* _btnEmpty;
    //new map
    Gtk::RadioButton* _radioSmall;
    Gtk::RadioButton* _radioMedium;
    Gtk::RadioButton* _radioLarge;
    Gtk::RadioButton* _radioGiant;
    Gtk::Button* _btnNext;
    //map editor
    Gtk::ToolButton* _btnNewMap;
    Gtk::ToolButton* _btnOpenMap;
    Gtk::ToolButton* _btnSaveMap;
    Gtk::ToolButton* _btnSaveAsMap;
    Gtk::ToolButton* _btnMapHome;
    
    //AGREGADOS
    Gtk::Table* _table;
    TableManager* _tm;
    GraphHandler* _gh;
    PacManMap* _graphMap;  
    bool _saveMapAs;
    string _windowMapTitle;
    string _windowWorldTitle;
    bool _worldChanged;
    //FIN AGREGADOS
    
    Gtk::Frame* _frameMap;
    Gtk::Toolbar* _toolbarElements;
    //world editor
    Gtk::TreeView* _treeviewMaps;
    Glib::RefPtr<Gtk::ListStore> _refListStore;
    Gtk::Button* _btnMapTop;
    Gtk::Button* _btnMapUp;
    Gtk::Button* _btnMapDown;
    Gtk::Button* _btnMapBottom;
    Gtk::ToolButton* _btnMapAdd;
    Gtk::ToolButton* _btnMapDelete;
    Gtk::ToolButton* _btnNewWorld;
    Gtk::ToolButton* _btnOpenWorld;
    Gtk::ToolButton* _btnSaveWorld;
    Gtk::ToolButton* _btnSaveAsWorld;
    Gtk::ToolButton* _btnWorldHome;
    //add map
    string _lblMapFileTitle;
    Gtk::Label* _lblMapFile;
    Gtk::Button* _btnAddBrowse;
    Gtk::ComboBox* _speedCombo;
    Gtk::Button* _btnAddOK;
    Gtk::Button* _btnAddClose;

    //home
    void on_btnHomeClose_clicked();
    bool on_eventboxMap_press(GdkEventButton* event);
    bool on_eventboxWorld_press(GdkEventButton* event);
    
    bool on_eventboxWorld_enter_notify(GdkEventCrossing* event);
    bool on_eventboxWorld_leave_notify(GdkEventCrossing* event);    

    bool on_eventboxMap_enter_notify(GdkEventCrossing* event);
    bool on_eventboxMap_leave_notify(GdkEventCrossing* event);    
    
    
    //map editor:toolbars
    void on_btnPill_clicked();
    void on_btnPowerUp_clicked();
    void on_btnBonus_clicked();
    void on_btnGhostHouseN_clicked();
    void on_btnGhostHouseE_clicked();
    void on_btnGhostHouseS_clicked();
    void on_btnGhostHouseW_clicked();
    void on_btnPacmanStart_clicked();
    void on_btnGateV_clicked();
    void on_btnGateH_clicked();
    void on_btnEmpty_clicked();
    void on_btnWall_clicked();
    //add map
    void on_btnAddOK_clicked();
    void on_btnAddClose_clicked();
    void on_btnAddBrowse_clicked();
    //new map
    void on_radioSmall_clicked();
    void on_radioMedium_clicked();
    void on_radioLarge_clicked();
    void on_radioGiant_clicked();
    void on_btnNext_clicked();
    //map editor
    void on_btnNewMap_clicked();
    void on_btnOpenMap_clicked();
    void on_btnSaveMap_clicked();
    void on_btnSaveAsMap_clicked();
    void on_btnMapHome_clicked();
    //world editor
    void on_btnMapAdd_clicked();
    void on_btnMapDelete_clicked();
    void on_btnMapTop_clicked();
    void on_btnMapUp_clicked();
    void on_btnMapDown_clicked();
    void on_btnMapBottom_clicked();
    void on_btnNewWorld_clicked();
    void on_btnOpenWorld_clicked();
    void on_btnSaveWorld_clicked();
    void on_btnSaveAsWorld_clicked();
    void on_btnWorldHome_clicked();

    bool loadMapWidgets();
    bool loadWorldWidgets();
    bool loadHomeWidgets();
    bool loadNewMapWidgets();
    bool loadAddMapWidgets();
    bool validateXmlMapFile(const string & path, string & errormsg);
    void addMapToTreeView(const string &path, int speed);
    //AGREGADO
    void initializeGraphAndHandler(int rows, int columns);
    bool isValidMap();
    bool discardMapChanges();
    bool loadTable(PacManMap* graphMap);
    bool on_closeWindowMap_clicked(GdkEventAny *);
    bool on_closeWindowWorld_clicked(GdkEventAny *);
    bool discardWorldChanges();
    void informInvalidGladeFile();
    //FIN AGREGADO
    
  protected:
    class ModelColumns : public Gtk::TreeModelColumnRecord {
      public:
        ModelColumns() {
          add(col_level);
          add(col_dirpath);
          add(col_name);
          add(col_speedtext);
          add(col_speed);
        }
        Gtk::TreeModelColumn<int> col_level;
        Gtk::TreeModelColumn<string> col_dirpath;
        Gtk::TreeModelColumn<string> col_name;
        Gtk::TreeModelColumn<int> col_speed;
        Gtk::TreeModelColumn<string> col_speedtext;
    };
    ModelColumns _model;
  public:
    ///constructor que lanza pantalla
    EditorScreen(int &argc, char **argv);
    ///destructor
    ~EditorScreen();
};
#endif /* __EDITORSCREEN_H__ */
