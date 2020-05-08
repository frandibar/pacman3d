#include "common/gladehelper.h"
#include <gtkmm.h>
#include <glade/glade.h>
#include <libglademm.h>

const string GladeHelper::DIALOGS_FILE = "/etc/Pacman3D/img/dialogs.glade";

GladeHelper::GladeHelper(){
  _questionYes = false;
  _xml = Gnome::Glade::Xml::create(DIALOGS_FILE);
  //info
  _xml->get_widget("dialogInfo",_dialogInfo);
  _xml->get_widget("lblInfoPrimary",_lblInfoPrimary);
  _xml->get_widget("lblInfoSecondary",_lblInfoSecondary);
  //warning
  _xml->get_widget("dialogWarning",_dialogWarning);
  _xml->get_widget("lblWarningPrimary",_lblWarningPrimary);
  _xml->get_widget("lblWarningSecondary",_lblWarningSecondary);
  //error
  _xml->get_widget("dialogError",_dialogError);
  _xml->get_widget("lblErrorPrimary",_lblErrorPrimary);
  _xml->get_widget("lblErrorSecondary",_lblErrorSecondary);
  //yes no
  _xml->get_widget("dialogYesNo",_dialogYesNo);
  _xml->get_widget("lblYesNoPrimary",_lblYesNoPrimary);
  _xml->get_widget("lblYesNoSecondary",_lblYesNoSecondary);
  _xml->get_widget("btnQuestionYes",_btnQuestionYes);
  _btnQuestionYes->signal_clicked().connect(sigc::mem_fun(*this,&GladeHelper::on_btnQuestionYes_clicked));
  _xml->get_widget("btnQuestionNo",_btnQuestionNo);
  _btnQuestionNo->signal_clicked().connect(sigc::mem_fun(*this,&GladeHelper::on_btnQuestionNo_clicked));
}

bool GladeHelper::showFileChooserDialog(Gtk::Window &window,const bool &save,const string &title,const string &filterName,const string &filterPattern,string &filename) {
  //crea dialog
  Gtk::FileChooserAction action (Gtk::FILE_CHOOSER_ACTION_OPEN);
  if (save)
    action = Gtk::FILE_CHOOSER_ACTION_SAVE;
  Gtk::FileChooserDialog dialog(title,action);
  dialog.set_transient_for(window);
  //agrega botones estandar
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  if (save)
    dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
  else
    dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
  //agrega filtro
  Gtk::FileFilter filter;
  filter.set_name(filterName);
  filter.add_pattern(filterPattern);
  dialog.set_filter(filter);
  //muestra dialog
  if (dialog.run() == Gtk::RESPONSE_OK) {
    filename.assign(dialog.get_filename());
    return true;
  }
  filename.assign("");
  return false;
}

void GladeHelper::showInformationDialog(Gtk::Window &window,const string &msg,const string &secondary) {
  _dialogInfo->set_transient_for(window);
  string primary("<b>");
  primary.append(msg);
  primary.append("</b>");
  _lblInfoPrimary->set_markup(primary);
  string sec("  <i>");
  sec.append(secondary);
  sec.append("</i>");
  _lblInfoSecondary->set_markup(sec);
  _dialogInfo->run();
  _dialogInfo->hide();
}

void GladeHelper::showWarningDialog(Gtk::Window &window,const string &msg,const string &secondary) {
  _dialogWarning->set_transient_for(window);
  string primary("<b>");
  primary.append(msg);
  primary.append("</b>");
  _lblWarningPrimary->set_markup(primary);
  string sec("  <i>");
  sec.append(secondary);
  sec.append("</i>");
  _lblWarningSecondary->set_markup(sec);
  _dialogWarning->run();
  _dialogWarning->hide();
}

void GladeHelper::showErrorDialog(Gtk::Window &window,const string &msg,const string &secondary) {
  _dialogError->set_transient_for(window);
  string primary("<b>");
  primary.append(msg);
  primary.append("</b>");
  _lblErrorPrimary->set_markup(primary);
  string sec("  <i>");
  sec.append(secondary);
  sec.append("</i>");
  _lblErrorSecondary->set_markup(sec);
  _dialogError->run();
  _dialogError->hide();
}

bool GladeHelper::showYesNoDialog(Gtk::Window &window,const string &msg,const string &secondary) {
  _questionYes = false;
  _dialogYesNo->set_transient_for(window);
  _lblYesNoPrimary->set_markup(msg);
  _lblYesNoSecondary->set_markup(secondary);
  _dialogYesNo->run();
  _dialogYesNo->hide();
  return _questionYes;
}

void GladeHelper::on_btnQuestionYes_clicked() {
  _questionYes = true;
}

void GladeHelper::on_btnQuestionNo_clicked() {
  _questionYes = false;
}
