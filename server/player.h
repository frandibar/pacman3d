/**
 * @file player.h
 * header de clase Player
 * */
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "lib/utilities.h"
#include "lib/edge.h"
#include "lib/vertex.h"
#include "lib/thread.h"
#include "lib/mutex.h"
#include <map>
#include <vector>

/**
 * Clase abstracta padre de Pacman y Ghost. Esta clase contiene el id del jugador, la orientación que tiene en el arco, su velocidad, la posición en el arco, una referencia al arco, un booleano para saber si esta vivo o no, y una referencia a un mutex (el cual evitará superposiciones entre los jugadores).
 * La clase básicamente tiene métodos setters y getters, pero además un método keyPressed() que recibe la tecla presionada y efectúa el movimiento, que si es un giro, lo implementa facilmente, pero si la acción es la de moverse, entonces utiliza un método abstracto move(), el que será redefinido por cada uno de sus descendientes.
 * */
class Player{
  private:
    int _idPlayer; ///< id del jugador
    tPlayerOrientation _orientation;///< orientacion de jugador
    int _speed;///< velocidad de jugador
    unsigned short _position; ///< posicion relativa dentro de la arista 0 a 63
    const Edge*	_tunnel; ///< arista en la que se encuentra
    bool	_alive;///< flag para saber si esta vivo
    int		_edgeID;///< id del arco que contiene
    
    inline void setOrientation(const tPlayerOrientation & orientation){		
		_orientation = orientation;
	}
	
    Mutex & _key;
    
  protected:    
    
    virtual void move(){};
    
    void lockPlayers(){
		_key.lock();	
	}
	void unlockPlayers(){
		_key.unlock();	
	}
    
  public:
  	/// 
  	static const unsigned short steps;
  	/// 
  	static const unsigned short ratio;
    ///constructor con parametros
    Player(const int &idPlayer, const Edge * tunnel, int initPosition, tPlayerOrientation orientation, Mutex & key);
    ///destructor
    virtual ~Player(){};
    ///devuelve id de jugador al que esta detectando
    inline int getPlayerId() const {return _idPlayer;}
    ///devuelve posicion
    inline unsigned short getPosition() const { return _position; }
    ///setea la posicion del jugador
    inline void setPosition(unsigned short position){ _position = position; }
        
    ///jugador toco tecla
    void keyPressed(const tKeyType &key);
    ///cambia velocidad
    inline void setSpeed (const int &speed) {
  		_speed = speed;
	}
	///devuelve la velocidad
    inline int getSpeed () const{
  		return _speed;
	}
    ///devuelve la arista en la que se encuentra
    inline const Edge * getEdge() const{return _tunnel;}
    ///setea una nueva arista para que se posicione el jugador
    void setEdge(const Edge* edge);
    ///devuelve el id del arco actual donde se encuentra el jugador
    inline int getEdgeID() const{
    	return _edgeID;	
    }
    ///devuelve la direccion del jugador segun el tipo de arco
    inline tPlayerOrientation getDirection() const{
		return _orientation;
	}
    ///termina la ejecucion del thread para ese jugador
    void endGame();
    ///devuelve si el jugador esta vivo o no
    inline bool isAlive() const{
    	return _alive;
    }
    ///setea la variable alive
    inline void setAlive(bool alive){
    	_alive = alive;
    }
    ///devuelve la orientacion del jugador
    tOrientation getOrientation();
    
};

/// Lista de Jugadores
typedef std::vector< Player* >  PlayerList;

#endif /* __PLAYER_H__ */
