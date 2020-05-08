#ifndef GAME_H_
#define GAME_H_

#include <sstream>
#include <fstream>
#include "common/graph.h"
#include "common/thread.h"
#include "common/mutex.h"
#include <set>

class PlayerHandler;

class IdSet: public std::set<int>{};

class Game: public Thread
{
private:
	PlayerHandler* _ph;
	Mutex	key;
	IdSet 	aliveGhosts;
	IdSet	pillsLeft;
	unsigned short int	_bonusCounter;
	std::fstream	log;
	bool jugando;

protected:
	// VER QUE HAY COSAS Q SERIAN NECESARIAS EN EL MAIN DE EJECUCION
	void main();

public:
	//CONSTANTES PARA UTILIZAR PARA LOS PUNTOS (SE NECESITAN PUBLICAS PORQUE GHOSTSCORE ES USADA AFUERA TAMBIEN)
	static const int GHOSTSCORE;
	static const int BONUSSCORE;
	static const int POWERUPSCORE;
	static const int PILLSCORE;
	
	Game(Graph & _levelGraph, Vertex* pacmanInitVertex, Vertex* ghostHouseVertex, int maxPlayers);
	inline PlayerHandler* getPH() const{
		return _ph;
	}
	// Esto es para que se sepa por fuera que esta jugando.. no creo que sea necesario en ThreadGame
	bool estaJugando(){
		bool playing;
		key.lock();
		playing = jugando;
		key.unlock();
		return playing;
	}
	// Para terminarlo si se toca de afuera.. tampoco lo creo necesario para ThreadGame
	void terminarJuego(){
		key.lock();
		jugando = false;
		key.unlock();
	}
	// METODOS NECESARIOS
	// para cuando pacman come un elemento.. dependiendo cual sea, setea los puntos, pUp y avisa en log.. NO HACE NADA PARA TERMINAR EL PUP DE PACMAN LUEGO.. TAMPOCO SE HACE NADA EN EL THREAD PARA QUE APAREZCA EL BONUS Y DESAPAREZCA
	void elementoComido(Element* elemento);
	// para cuando se come a pacman.. setea el jugando en false (usando mutex) y avisa en log quien lo comio con id
	void pacmanComido(int id);
	// agrega el jugador al PlayerHandler y avisa en log
	void agregarJugador();
	
	virtual ~Game();
	
	//ESTOS SON SOLAMENTE METODOS QUE ESCRIBEN EN EL LOG
	void avisarJugadorNuevo(const string & tipo);
	void avisarComienzoJuego();
	void avisarPacmanComido(int id);
	void avisarComeElemento(const string & tipo, int idVertice, int multip = 0);
	void avisarFinJuego();
	void avisarFantasmaComido(int id);
	void avisarFantasmaRevivido(int id);
};

#endif /*GAME_H_*/
