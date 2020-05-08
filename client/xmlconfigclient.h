#ifndef XMLCONFIGCLIENT_H_
#define XMLCONFIGCLIENT_H_

#include "client/clientutilities.h"

#include <string>
using std::string;

/**
 * Estructura que representa los parametros de configuracion del Cliente PacMan3D
 */
typedef struct tConfigClient {
  int port; ///< puerto a donde se conectara
  string address; ///< direccion a donde se conectara
  tSoundType soundType; ///< tipo de sonido
  string playerName; ///< nombre del jugador
  ShortcutMap messageMap; ///< Mensajes (solo para fantasmas)
  tMovementKeys movementKeys; ///< teclas de movimiento
} tConfigClient;

/**
 * XmlConfigClient es una clase de tipo servicio (no se instancia, solo brinda metodos static)
 * 
 * Brinda metodos save y load para guardar y levantar los parametros de configuracion
 * del Cliente PacMan3D
 * 
 * Dicha configuracion se guarda en formato XML en el archivo clientconfig.xml
 * 
 */
class XmlConfigClient {

private:
    static const string XML_FILE;
    
public:
	/// Guarda la configuracion al archivo clientconfig.xml
    static void save(const tConfigClient & config);
    /// carga a memoria la configuracion del archivo clientconfig.xml
    static bool load(tConfigClient & config);
};

#endif /*XMLCONFIGCLIENT_H_*/
