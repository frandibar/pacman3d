#pragma once

#include <gtkmm.h>
#include <glade/glade.h>
#include <libglademm.h>
#include <string>
using std::string;

/**
 * Clase que aporta los dialog estandar GTK:
 * FileChooseDialog
 * InformationDialog
 * WarningDialog
 * ErrorDialog
 * YesNoDialog 
 */
class GladeHelper {
  private:
    static const string DIALOGS_FILE;
    bool _questionYes;
    Glib::RefPtr<Gnome::Glade::Xml> _xml;
    //info
    Gtk::Dialog* _dialogInfo;
    Gtk::Label* _lblInfoPrimary;
    Gtk::Label* _lblInfoSecondary;
    //warning
    Gtk::Dialog* _dialogWarning;
    Gtk::Label* _lblWarningPrimary;
    Gtk::Label* _lblWarningSecondary;
    //error
    Gtk::Dialog* _dialogError;
    Gtk::Label* _lblErrorPrimary;
    Gtk::Label* _lblErrorSecondary;
    //yes no
    Gtk::Dialog* _dialogYesNo;
    Gtk::Label* _lblYesNoPrimary;
    Gtk::Label* _lblYesNoSecondary;
    Gtk::Button* _btnQuestionYes;
    Gtk::Button* _btnQuestionNo;

    void on_btnQuestionYes_clicked();
    void on_btnQuestionNo_clicked();
    
    //los ocultamos por seguridad
    GladeHelper (const GladeHelper&);
    GladeHelper& operator=(const GladeHelper&);
  public:
  	/// Constructor default
    GladeHelper();
    /// permite abrir un file chooser open/save con un filtro
    bool showFileChooserDialog(Gtk::Window &window,const bool &save,const string &title,const string &filterName,const string &filterPattern,string &filename);
    ///muestra en pantalla un dialog de informacion
    void showInformationDialog(Gtk::Window &window,const string &msg,const string &secondary);
    ///muestra en pantalla un dialog de advertencia
    void showWarningDialog(Gtk::Window &window,const string &msg,const string &secondary);
    ///muestra en pantalla un dialog de error
    void showErrorDialog(Gtk::Window &window,const string &msg,const string &secondary);
    ///muestra en pantalla un dialog Yes/No y devuelve true si se eligio Yes
    bool showYesNoDialog(Gtk::Window &window,const string &msg,const string &secondary);
};
