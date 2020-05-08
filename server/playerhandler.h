/**
 * @file playerhandler.h
 */
#ifndef PLAYERHANDLER_H_
#define PLAYERHANDLER_H_

#include "server/threadgame.h"
#include "server/playeractioner.h"
#include "server/pacman.h"
#include "server/ghost.h"
#include "lib/level.h"
#include <map>
#include <list>

/// PlayerListC
typedef std::list<Player>  PlayerListC;
//class PlayerListC: public std::list<Player>{};

/// GhostMap
typedef std::map< int, Ghost* >  GhostMap;

class ThreadGame;

/**
 *  Esta clase es muy importante para la simulación, y encapsula todos los movimientos de los jugadores dentro de ella, para que el ThreadGame se pueda comunicar directamente con esta y nada más.
 * Como atributos contiene una referencia al grafo, un PlayerActioner que le entregara a los Players, una referencia al ThreadGame, un mutex, que será el que entregara a los Players, una instancia de Pacman, un map de Ghosts, y referencias a los vértices, arcos y posiciones donde deben comenzar un nivel tanto pacman como los ghosts. Además, guarda la velocidad del nivel que se esta jugando, para poder setearle las velocidades correspondiente a pacman y ghosts, guarda el id del vértice que tiene el bonus, y la cantidad de pills que tiene el mapa, para asÃ­ poder saber cuando se termina el mismo.
 * Entre sus métodos podemos nombrar: addPlayer() que se encarga de incorporar a un jugador con el id que se le pasa. Reconoce que si el id es 0 (por defecto pacman) debe instanciar a pacman. De lo contrario, suma un ghost al mapa. removePlayer() quita un jugador del map (estos dos son cuidadosamente realizados mediante el uso del mutex correspondiente). movePlayer(), que recibe el id y el tipo de tecla, y lo que hace es tomar el player y correr el keyPressed() del correspondiente, para luego activar el mutex, verificar si se producen colisiones entre pacman y fantasmas, y desactiva el mutex. Si se produjera que se comen a pacman o este a un ghost, lo avisa al ThreadGame, así­ como también, para pacman, luego de ser movido, verifica si tiene un elemento comido, lo resetea, asignándole 0, y lo avisa al ThreadGame.
 * Para evaluar si hay colisiones entre pacman y los ghosts, gracias a que cuenta con el PlayerActioner, lo utiliza, y recorriendo el map de ghosts, verifica que cuales tienen colisiones o no con pacman, para luego dar el aviso correspondiente.
 * Además, esta clase cuenta con el método getPlayerListCopy(), el cual activa el mutex, genera una lista con copias de todos los jugadores, y la devuelve, para que el ThreadGame pueda enviar un status a todos los jugadores.
 * Por último, contiene más métodos para comunicarse con ThreadGame, quien llamará a estos métodos según necesite. BonusCreate() crea el bonus en la posición indicada, BonusExpire() saca el bonus de ese vértice, SetPacmanPowerUp() setea a pacman en powerup o no, dependiendo el valor de verdad pasado, getPacmanPowerUp() devuelve si pacman está o no en powerup, pillsLeft() devuelve si quedan pills por comer. Todos estos métodos implican activar el mutex para evitar problemas de concurrencia.
 * Los únicos métodos que quedan son Stop() que hace que no se puedan seguir ejecutando movimientos sobre los jugadores, hasta que se haga un Reset() que vuelve a colocar a todos los jugadores en sus posiciones iniciales, y revive a los muertos.
 */
class PlayerHandler {
private:

	Graph * _levelGraph;
	PlayerActioner * _actioner;
	ThreadGame & _game;
	
	Mutex playersKey;
	
	Pacman*	_pacman;
	GhostMap _ghosts;
	
	Vertex* _pacmanInitVertex;
	Vertex* _ghostHouseVertex;
	
	Edge * _pacmanStartingTunnel;
	int _pacmanInitPosition;
	tPlayerOrientation _pacmanOrientation;
	
	Edge * _ghostStartingTunnel;
	int _ghostInitPosition;
	tPlayerOrientation _ghostOrientation;
	
	bool _stoppedPacman;
	int _bonusVertex;
	bool _initialized;
	
	unsigned int _pillsLeft;
	unsigned int _powerUpLeft;
	tLevelSpeed _levelSpeed;
	
	
	void playerAtributeInitializer(Vertex* initVertex, Edge * &_startingTunnel,	int & _initPosition , tPlayerOrientation & _orientation);
	
	int playerInteraction();
	
	void loadStartingVerticesAndElementsInfo(Level & level);
	
	int getGhostSpeed();
	
	int getPacmanSpeed();
	
	//los ocultamos por seguridad
	PlayerHandler (const PlayerHandler&);
	PlayerHandler& operator=(const PlayerHandler&);
	
public:

	///crea el PlayerHandler e inicializa el level pasado
	PlayerHandler(ThreadGame & game);
	/// 
	virtual ~PlayerHandler();
	/// 
	void addPlayer(int id);
	/// 
	void removePlayer(int id);
	/// 
	void movePlayer(int id, tKeyType key);
	
	///devuelve un PlayerListC (lista de jugadores) con una copia de todos los jugadores. el primero siempre es el pacman. Luego hay que destruir la lista
	PlayerListC* getPlayerListCopy();
	
	///agrega un bonus en el vertice correspondiente, y devuelve una copia del mismo
	Element BonusCreate();
	
	///remueve el bonus del vertice correspondiente
	void BonusExpire();
	
	///setea a pacman en PowerUp, true o false
	void setPacmanPowerUp(bool pUp);
	
	///avisa si pacman esta en PowerUp
	bool getPacmanPowerUp();
	
	///devuelve la cantidad de powerUp que quedan en el mapa
	unsigned int getPowerUpLeft();
	
	///devuelve la cantidad de pills que quedan en el mapa
	unsigned int getPillsLeft();
	
	///avisa si todavia quedan pills o PowerUps por comer
	bool pillsOrPowerUpLeft();
	
	///coloca a todos los jugadores en sus posiciones de inicio
	void Reset();
	
	///no permite que se puedan seguir ejecutando acciones sobre los jugadores
	void stop();
	
	/** 
	 * inicializa el PlayerHandler con el nivel pasado, borrando lo anterior.
	 * para cambiar de nivel puede ser utilizado, pero debe ser seguido de un Reset() 
	 * para recolocar a todos los jugadores en sus posiciones para recomenzar el nivel
	 */
	void initializeLevel(Level & level);
};

#endif /*PLAYERHANDLER_H_*/
