//implementacion de clase Log
#include "log.h"
#include "date.h"
#include <iostream>
#include <fstream>
using std::ofstream;
using std::cout;
using std::endl;

Log::Log(const string &path) {
  _path.assign(path);
}

void Log::logMsg(const string &msg) {
  if (_path.compare("") == 0) {
    //salida por stdout
    _mtx.lock();
    cout << Date::now() << "> " << msg << endl;
    _mtx.unlock();
    return;
  }
  //salida por archivo
  _mtx.lock();
  ofstream arch(_path.c_str(),std::ios::app);
  arch << Date::now() << "> " << msg << endl;
  arch.close();
  _mtx.unlock();
}
