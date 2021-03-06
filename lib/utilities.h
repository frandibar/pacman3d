/**
 * @file utilities.h
 * 
 * utilidades comunes de PacMan3D
 */
#ifndef __UTILITIES_H__
#define __UTILITIES_H__
#include <list>
#include <vector>
#include <set>
#include <string>
#include <map>
using std::map;
using std::string;

///teclas q puede tocar
typedef enum tKeyType{UP = 0,DOWN = 1,LEFT = 2,RIGHT = 3};

///razones por la que puede hacer un stop
typedef enum tStopReason {PACMAN_WON = 0,PACMAN_EATEN = 1,PACMAN_DISCONNECTED = 2,MINIMUM_NOT_ACHIEVED = 3,SERVER_TERMINATED = 4,MAXIMUM_PLAYERS = 5,SOCKET_ERROR = 6};

///tipo de elemento
typedef enum tElementType {PACMAN_START = 0,GHOST_HOUSE = 1,POWER_UP = 2,BONUS = 3,PILL = 4};

/// velocidad de cada nivel
typedef enum tLevelSpeed {SPEED_LOW = 0, SPEED_MEDIUM = 1, SPEED_HIGH = 2};

///estado de elemento status
typedef enum tElementState {APPEARED = 0, DISAPPEARED = 1, EATEN = 2};

///orientacion
typedef enum tOrientation {NORTH = 0, SOUTH = 1, EAST = 2, WEST = 3};


///orientacion de los arcos
typedef enum tPlayerOrientation{ WEST_SOUTH = 0, EAST_NORTH = 1};

///posicion de jugador
typedef struct {
  unsigned char playerId;
  bool powerUpEnabled;
  bool alive;
  unsigned int edgeId;
  unsigned char position;
  unsigned char direction;
}tPlayerStatus;

///tipo de celda
typedef enum tCell {cPACMAN_START, cPILL, cPOWER_UP, cBONUS, cWALL, cEMPTY, cGATE_V, cGATE_H, cGHOUSE_N, cGHOUSE_E, cGHOUSE_S, cGHOUSE_W, cCANCELED};

///lista de posiciones de jugadores
typedef std::list<tPlayerStatus> PlayerStatusList;

///mapa de nombres e IP
typedef struct {
  string name;
  string IP;
}tName;

///mapa para nombre e IP para cada jugador
typedef std::map<int,tName> NameMap;

///lista de nombres
typedef struct {
  unsigned int id;
  string name;
}tPlayerName;

///lista de nombres de jugadores
typedef std::vector<tPlayerName> NameList;

///
const int STATUS_PERIOD = 50;

///
const int PLAYER_SPEED = 300;

/// minimo nro de puerto permitido
const int MINIMUM_ALLOWED_PORT = 1024;

/// nombres de mapa
typedef struct {
  string path;
  tLevelSpeed speed;
}tMapFile;

///lista de paths de mapas incluidos en un xml de world
typedef std::vector<tMapFile> MapFileList;

///set de aristas activas
typedef std::set<int> EdgeSet;

///mapa para guardar estadisticas de cuantas veces fue comido cada fantasma
typedef map<int,int> EatenGhostsMap;

///estadisticas de cada nivel
typedef struct tStats {
  unsigned int pills;
  unsigned int powerups;
  unsigned int bonusCounter;
  unsigned int ghostsCounter;
  unsigned int pillsEaten;
  unsigned int powerupsEaten;
  EatenGhostsMap eatenGhosts;
} tStats;

#endif /* __UTILITIES_H__ */
