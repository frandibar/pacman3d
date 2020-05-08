#include "Game.h"
#include "playerhandler.h"

const int Game::GHOSTSCORE = 10;
const int Game::BONUSSCORE = 50;
const int Game::POWERUPSCORE = -5;
const int Game::PILLSCORE = 1;

Game::Game(Graph & _levelGraph, Vertex* pacmanInitVertex, Vertex* ghostHouseVertex, int maxPlayers):log("pacman.log", std::ios::out)
{	
	_bonusCounter = 1;
	jugando = false;
	//_ph = new PlayerHandler(_levelGraph, pacmanInitVertex, ghostHouseVertex, maxPlayers, *this);
	VertexList * vertices = _levelGraph.getVertices();
	VertexList::iterator it;
	for ( it = vertices->begin() ; it != vertices->end() ; ++it){
		if ( (*it)->getContent() ){	
			if 	( ((*it)->getContent()->getType() == PILL) || ((*it)->getContent()->getType() == POWER_UP) )	
				pillsLeft.insert((*it)->getContent()->getIdVertex());			
		}
	}
	delete vertices;
}



void Game::agregarJugador(){
	string player;
	int playerID = _ph->addPlayer();
	if ( playerID ){
		key.lock();
		aliveGhosts.insert(playerID);
		key.unlock();
		player = "fantasma";
	}else player = "pacman";
	key.lock();
	avisarJugadorNuevo(player);
	key.unlock();
}

void Game::pacmanComido(int id){
	//EL VERDADERO DEBERIA PARAR EL JUEGO
	key.lock();
	avisarPacmanComido(id);	
	jugando = false;
	key.unlock();	
}

void Game::elementoComido(Element* elemento){
	switch ( elemento->getType() ){
		case (BONUS):{			
			key.lock();
			avisarComeElemento("bonus", elemento->getIdVertex(), _bonusCounter);
			key.unlock();
			_bonusCounter++;
			break;	
		}
		case (PILL):{
			key.lock();
			pillsLeft.erase( pillsLeft.find( elemento->getIdVertex() ) );
			key.unlock();
			break;	
		}
		case (POWER_UP):{
			_ph->setPacmanPowerUp(true);
			key.lock();
			pillsLeft.erase( pillsLeft.find( elemento->getIdVertex() ) );
			avisarComeElemento("pUp", elemento->getIdVertex());
			key.unlock();
			//ACA FALTARIA HACER QUE LA EJECUCION DEL THREAD CONTROLE EL TIEMPO DE PUp
			break;	
		}
		default: break;
	}
	
	delete elemento;
}

Game::~Game()
{
	delete _ph;
	log.close();
}

void Game::avisarJugadorNuevo(const string & tipo){
	string mensaje("Nuevo cLiente "+tipo+"\n");
	log.write(mensaje.c_str(), mensaje.size());
}
void Game::avisarComienzoJuego(){
	string mensaje("Comienzo Juego\n");
	log.write(mensaje.c_str(), mensaje.size());
}
void Game::avisarPacmanComido(int id){
	std::stringstream ss;
	ss << id;
	string mensaje("Fantasma id: "+ss.str() +" come al pacman\n");
	log.write(mensaje.c_str(), mensaje.size());
}
void Game::avisarComeElemento(const string & tipo, int idVertice, int multip ){
	std::stringstream ss;
	ss << idVertice;
	string vertice = ss.str();
	if ( multip != 0 ){
		std::stringstream ss2;
		ss2 << multip;
		vertice += " ["+ss2.str()+"]";	
	}
	string mensaje("Pacman come "+tipo+" del vertice "+vertice+"\n");
	log.write(mensaje.c_str(), mensaje.size());
}
void Game::avisarFinJuego(){
	std::stringstream ss;
	ss << _ph->getPacman()->getScore();
	string mensaje("Fin Juego. Cantidad de puntos: "+ ss.str() +"\n");
	log.write(mensaje.c_str(), mensaje.size());
}
void Game::avisarFantasmaComido(int id){
	std::stringstream ss;
	ss << id;
	string mensaje("Pacman come al fantasma id: "+ss.str() +" \n");
	log.write(mensaje.c_str(), mensaje.size());
}
void Game::avisarFantasmaRevivido(int id){
	std::stringstream ss;
	ss << id;
	string mensaje("Fantasma id: "+ss.str() +" regresa a la casa\n");
	log.write(mensaje.c_str(), mensaje.size());
}

void Game::main(){
	key.lock();
	avisarComienzoJuego();
	jugando = true;
	while(jugando){
		key.unlock();
		// pido una copia de los jugadores
		PlayerListC* players = _ph->getPlayerListCopy();
		// ACA PODRIA MANDAR EL STATUS
		
		//primero verifico que queden pills (IMPORTANTE)
		key.lock();
		if ( pillsLeft.empty() ){
			jugando = false;
			delete players;
			continue;	
		}
		key.unlock();
		
		PlayerListC::iterator it;
		// si pacman esta muerto.. salimooo (IMPORTANTE)
		it = players->begin();
		// si esta muerto
		if ( !it->isAlive() ){
			//lockeo
			key.lock();
			jugando = false;
			delete players;
			//en el proximo loop va a salir
			continue;
		}else{
			//saco a pacman
			players->erase( it );
			// veo si los demas estaban vivos y ahora murieron, o al reves
			for ( it = players->begin() ; it != players->end() ; ++it){
				// si el fantasma esta muerto
				if ( !it->isAlive()	){
					// si estaba en la lista de vivos
					if ( aliveGhosts.find(it->getPlayerId()) != aliveGhosts.end() ){
						key.lock();
						aliveGhosts.erase( aliveGhosts.find( it->getPlayerId() ) );						
						avisarFantasmaComido(it->getPlayerId());
						key.unlock();
					}
				}else{// si esta vivo
					// si no esta en la lista de vivos, revivio.. asi q lo agrego
					if ( aliveGhosts.find(it->getPlayerId()) == aliveGhosts.end() ){
						key.lock();
						aliveGhosts.insert( it->getPlayerId() );						
						avisarFantasmaRevivido(it->getPlayerId());
						key.unlock();
					}
				}
			}
		}
		delete players;
		//duermo hasta el proximo status..
		Sleep(50);
		key.lock();
	}
	avisarFinJuego();
	key.unlock();
}
