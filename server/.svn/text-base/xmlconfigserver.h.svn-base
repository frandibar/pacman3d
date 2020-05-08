/**
 * @file xmlconfigserver.h
 */
#ifndef __XMLCONFIGSERVER_H__
#define __XMLCONFIGSERVER_H__
#include <string>
using std::string;

/**
 * Parametros de configuracion del juego
 * Utilizado por XmlConfigServer y ConfigScreen
 * 
 * @sa XmlConfigServer
 * @sa ConfigScreen
 */
typedef struct tConfig {
  string worldFile;
  int port; ///< Puerto en donde escuchara el server
  string address; ///< direccion en donde escuchara el server
  int min; ///< minima cantidad de jugadores
  int max; ///< maxima cantidad de jugadores
  bool ghostComm; ///< permite GhostCommunication (si un Ghost ve al pacman, loven todos)
  int lives; ///< cantidad de vidas para el pacman
  string logFile; ///< nombre y ruta del log File (vacio es por cout)
  int waitingTime; ///< tiempo de espera hta comenzar el juego 
} tConfig;

/**
 * XmlConfigServer es una clase de tipo servicio (no se instancia, solo brinda metodos static)
 * 
 * El servicio brindado es de serializacion-deserializacion de la configuracion
 * de los parametros del server en formato XML
 * 
 * Dicha configuracion se guarda en formato XML en el archivo serverconfig.xml
 */
class XmlConfigServer {
  private:
    static const string XML_FILE;
  public:
  	/// Guarda la configuracion en formato xml en el archivo serverconfig.xml
    static void save(const tConfig &config);
    /// Carga la configuracion del server a partir del archivo XML serverconfig.xml
    static bool load(tConfig &config);
};

#endif /*__XMLCONFIGSERVER_H__*/
