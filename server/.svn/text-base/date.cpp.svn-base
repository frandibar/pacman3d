#include "date.h"
#include <time.h>
#include <sstream>

//devuelve la fecha del sistema
string Date::now() {
  time_t t;
  t = time(NULL);
  struct tm *sysTime = localtime(&t);
  std::ostringstream oss;
  //anio
  oss << (sysTime->tm_year+1900) << "-";
  //mes
  int aux = sysTime->tm_mon+1;
  if (aux < 10)
    oss << "0";
  oss << aux << "-";
  //dia
  aux = sysTime->tm_mday;
  if (aux < 10)
    oss << "0";
  oss << aux << " ";
  //hora
  aux = sysTime->tm_hour;
  if (aux < 10)
    oss << "0";
  oss << aux << ":";
  //minuto
  aux = sysTime->tm_min;
  if (aux < 10)
    oss << "0";
  oss << aux << ":";
  //segundo
  aux = sysTime->tm_sec;
  if (aux < 10)
    oss << "0";
  oss << aux;
  return oss.str();
}
