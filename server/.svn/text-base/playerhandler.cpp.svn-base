#include "playerhandler.h"

PlayerHandler::PlayerHandler(ThreadGame & game):_game(game)
{
	_levelGraph = NULL;
	_actioner = NULL;
	_initialized = false;
	_stoppedPacman = true;
	_pacman = NULL;
}

void PlayerHandler::initializeLevel(Level & level){
	playersKey.lock();
	
	_levelGraph = level.getGraph();
	delete _actioner;
	_actioner = new PlayerActioner(*_levelGraph);
	delete _pacman;
	_pacman = NULL;
	_pillsLeft = 0;
	_powerUpLeft = 0;
	_levelSpeed = level.getSpeed();
	
	//cargo los pacmanInitVertex, ghostHouseVertex, cantidad de pills, y bonusVertex
	loadStartingVerticesAndElementsInfo(level);
	
	//PACMAN
	playerAtributeInitializer(_pacmanInitVertex, _pacmanStartingTunnel, _pacmanInitPosition, _pacmanOrientation);
	//GHOST
	playerAtributeInitializer(_ghostHouseVertex, _ghostStartingTunnel, _ghostInitPosition, _ghostOrientation);
		
	//_stoppedPacman = false;
	_initialized = true;
	
	playersKey.unlock();
	
	Reset();
}

void PlayerHandler::loadStartingVerticesAndElementsInfo(Level & level){
	ElementList* elements = level.getElementList();
	ElementList::iterator it;
	for ( it = elements->begin() ; it != elements->end() ; ++it){
		switch ( (*it).getType() ){
			case PACMAN_START:{
				_pacmanInitVertex = _levelGraph->getVertex( (*it).getIdVertex() );
				break;
			}
			case GHOST_HOUSE:{
				_ghostHouseVertex = _levelGraph->getVertex( (*it).getIdVertex() );
				break;	
			}
			case PILL:{
				++_pillsLeft;
				break;
			}
			case POWER_UP:{
				++_powerUpLeft;
				break;	
			}
			case BONUS:{
				_bonusVertex = it->getIdVertex();
				//destruyo ese elemento bonus y se lo saco al vertice para que lo seteen de afuera cuando se desee
				delete ( _levelGraph->getVertex(_bonusVertex)->getContent() );
				_levelGraph->getVertex(_bonusVertex)->setContent(NULL);
				break;	
			}
		}
	}
	delete elements;
}

PlayerHandler::~PlayerHandler()
{
	delete _pacman;
	GhostMap::iterator it;
	for ( it = _ghosts.begin(); it != _ghosts.end() ; ++it){
		delete it->second;	
	}
	delete _actioner;
}
//TODO ESTO PODRIA CAMBIAR!!
int PlayerHandler::getGhostSpeed(){
	switch (_levelSpeed){
		case SPEED_LOW:{
			return (STATUS_PERIOD * 5);	
		}
		case SPEED_MEDIUM:{
			return (STATUS_PERIOD * 4);	
		}
		case SPEED_HIGH:{
			return (STATUS_PERIOD * 3);	
		}
		
		default: return getPacmanSpeed();
	}
}
//TODO ESTO PODRIA CAMBIAR!!
int PlayerHandler::getPacmanSpeed(){
	return (STATUS_PERIOD * 6);
}

void PlayerHandler::addPlayer(int id){
	
	if ( id == 0 ){
		playersKey.lock();
		if ( !_initialized ){
			playersKey.unlock();
			return;
		}
		_pacman = new Pacman(id, _pacmanStartingTunnel, _pacmanInitPosition, _pacmanOrientation, *_actioner, playersKey);
		_pacman->setSpeed( getPacmanSpeed() );
		playersKey.unlock();
	}else{
		//lockeo para evitar que el mapa sea usado para un movimiento de jugador
		playersKey.lock();
		if ( !_initialized ){
			playersKey.unlock();
			return;
		}
		_ghosts[id] = new Ghost(id, _ghostStartingTunnel, _ghostInitPosition, _ghostOrientation, *_actioner, playersKey);
		_ghosts[id]->setSpeed( getGhostSpeed() );
		playersKey.unlock();
	}	
}

void PlayerHandler::Reset(){
	//lockeo para evitar que el mapa sea usado para un movimiento de jugador
	playersKey.lock();
	
	if ( _pacman ){
		// renuevo a pacman
		delete _pacman;
		_pacman = new Pacman(0, _pacmanStartingTunnel, _pacmanInitPosition, _pacmanOrientation, *_actioner, playersKey);
		_pacman->setSpeed( getPacmanSpeed() );
	}
	// ahora con los ghosts
	GhostMap::iterator it;
	for ( it = _ghosts.begin(); it != _ghosts.end() ; ++it){
		delete it->second;
		_ghosts[it->first] = new Ghost(it->first, _ghostStartingTunnel, _ghostInitPosition, _ghostOrientation, *_actioner, playersKey);
		_ghosts[it->first]->setSpeed( getGhostSpeed() );
	}	
	_stoppedPacman = false;
	playersKey.unlock();
}

void PlayerHandler::removePlayer(int id){
	//lockeo para evitar que el mapa sea usado para un movimiento de jugador
	if ( id == 0 ){	
		playersKey.lock();	
		delete (_pacman);
		_pacman = NULL;	
		playersKey.unlock();	
	}else{
		playersKey.lock();
		if ( _ghosts.find(id) == _ghosts.end() ){
			playersKey.unlock();
			return;
		}
		delete (_ghosts[id]);
		_ghosts.erase(_ghosts.find(id));
		playersKey.unlock();
	}
		
}


void PlayerHandler::playerAtributeInitializer(Vertex* initVertex, Edge * & startingTunnel,	int & initPosition , tPlayerOrientation & orientation){
	EdgeList edges = initVertex->getEdges();
	//pruebo en todas las direcciones a ver en cual puede comenzar
	if ( edges[NORTH] ){
		startingTunnel = edges[NORTH];
		initPosition = 0;
		orientation = EAST_NORTH;
	}else{
		if ( edges[EAST] ){
			startingTunnel = edges[EAST];
			initPosition = 0;
			orientation = EAST_NORTH;
		}else{
			if ( edges[SOUTH] ){
				startingTunnel = edges[SOUTH];
				initPosition = 63;
				orientation = WEST_SOUTH;
			}else{
				startingTunnel = edges[WEST];
				initPosition = 63;
				orientation = WEST_SOUTH;	
			}
		}
	}
	
}

int PlayerHandler::playerInteraction(){
	GhostMap::iterator it;
	//para cada uno de los fantasmas, evaluo el choque con pacman
	for ( it = _ghosts.begin() ; it != _ghosts.end() ; ++it){
		if ( _actioner->evaluatePlayerInteraction(*_pacman, *(it->second) ) ){
			if ( _pacman->isPoweredUp() ){
				it->second->setAlive(false);
				_game.eatenGhost(it->second->getPlayerId());
			}else{
				_pacman->setAlive(false);
				return it->second->getPlayerId();
			}
		}
	}
	return -1;
}

void PlayerHandler::movePlayer(int id, tKeyType key){
	//PACMAN TIENE ID 0
	if ( id == 0 ){
		//lockeo para ver si el juego esta parado
		playersKey.lock();
		// si murio tengo que salir, no puedo seguir jugando
		if ( _stoppedPacman ){
			playersKey.unlock();
			return;
		}
		
		playersKey.unlock();
		//corro el movimiento, que va a activar el mutex para cambiar posiciones
		_pacman->keyPressed(key);
		//si paro, no sigo
		playersKey.lock();
		if ( _stoppedPacman ){
			playersKey.unlock();
			return;
		}
		
		playersKey.unlock();
		//si acaba de comer un elemento, aviso al ThreadGame
		Element* eaten = _pacman->getEatenElement();
		if ( eaten ){				
			_game.eatenElement(*eaten);
			_pacman->resetEatenElement();
			//si es pill o power up, voy contando para saber cuando termina el nivel
			if ( eaten->getType() == PILL ){
				playersKey.lock();
				--_pillsLeft;
				playersKey.unlock();
			}else{
				if ( eaten->getType() == POWER_UP ){
					playersKey.lock();
					--_powerUpLeft;
					playersKey.unlock();	
				}
			}
			delete eaten;
		}
	}else{
		Ghost* ghost = _ghosts[id];
		//si paro el juego, salgo
		playersKey.lock();
		if ( _stoppedPacman ){
			playersKey.unlock();
			return;
		}
		
		playersKey.unlock();
		//guardo si estaba vivo o no
		bool wasAlive = ghost->isAlive();
		//ejecuta el cambio de posicion
		ghost->keyPressed(key);
		//si paro el juego, salgo
		playersKey.lock();
		if ( _stoppedPacman ){
			playersKey.unlock();
			return;
		}
		
		playersKey.unlock();
		//si no estaba vivo, lo revivo si ahora lo esta
		if ( !wasAlive ){
			if ( ghost->isAlive() ){
				_game.revivedGhost(ghost->getPlayerId());
			}
		}
	}
	playersKey.lock();	
	if ( _stoppedPacman ){
		playersKey.unlock();
		return;
	}
	
	//evaluo la interaccion de pacman con los fantasmas
	int playerID = playerInteraction();
	//si me devuelve un id valido, se comieron a pacman, asi que lo aviso
	if ( playerID != -1 )
		_game.eatenPacman(playerID);
	
	playersKey.unlock();
	
	
}

PlayerListC* PlayerHandler::getPlayerListCopy(){
	//creo una lista
	PlayerListC* list = new PlayerListC();
	GhostMap::iterator it;
	//lockeo para poder tener a todos quietos
	playersKey.lock();
	//meto copia d epacman
	list->push_back(*_pacman);
	//meto copia de todos los fantasmas
	for ( it = _ghosts.begin() ; it != _ghosts.end() ; ++it){
		list->push_back(*(it->second));
	}
	//los libero
	playersKey.unlock();
	
	return list;
}


Element PlayerHandler::BonusCreate(){
	Vertex* vertex = _levelGraph->getVertex(_bonusVertex);
	
	playersKey.lock();// si el vertice no tiene un element dentro, puedo colocarle uno
	Element* elementBonus = new Element(_bonusVertex, BONUS, NORTH);
	//se lo pongo al vertice
	vertex->setContent( elementBonus );
	//hago una copia
	Element bonus(*elementBonus);
	playersKey.unlock();
	
	//devuelvo la copia
	return bonus;
}

void PlayerHandler::BonusExpire(){	
	Vertex* vertex = _levelGraph->getVertex(_bonusVertex);
	playersKey.lock();
	
	Element* element = vertex->getContent();
	// seteo en NULL, si tenia algo lo saque, y sino ya era NULL
	vertex->setContent(NULL);
	playersKey.unlock();
	
	//lo elimino, si era NULL no pasa nada
	delete element;
}

void PlayerHandler::setPacmanPowerUp(bool pUp){
	GhostMap::iterator it;
	playersKey.lock();
	if ( pUp ){//si me piden que habilite pUp le doy a pacman la velocidad de fantasmas		
		for ( it = _ghosts.begin() ; it != _ghosts.end() ; ++it){
			it->second->setSpeed( _pacman->getSpeed() );
		}
		_pacman->setSpeed( getGhostSpeed() );
		
	}else{//si me piden que lo deshabilite
		for ( it = _ghosts.begin() ; it != _ghosts.end() ; ++it){
			it->second->setSpeed( getGhostSpeed() );
		}
		_pacman->setSpeed( PLAYER_SPEED );
	}
	_pacman->setPowerUp(pUp);
	playersKey.unlock();
}

bool PlayerHandler::getPacmanPowerUp(){
	bool pUp;
	playersKey.lock();
	pUp = _pacman->isPoweredUp();
	playersKey.unlock();
	return pUp;
}

bool PlayerHandler::pillsOrPowerUpLeft(){
	bool left;
	playersKey.lock();
	left = ( (_pillsLeft + _powerUpLeft) != 0 );
	playersKey.unlock();
	return left;
}

unsigned int PlayerHandler::getPowerUpLeft(){
	unsigned int left;
	playersKey.lock();
	left = _powerUpLeft;
	playersKey.unlock();
	return left;
}

unsigned int PlayerHandler::getPillsLeft(){
	unsigned int left;
	playersKey.lock();
	left = _pillsLeft;
	playersKey.unlock();
	return left;
}

void PlayerHandler::stop(){	
	playersKey.lock();
	_stoppedPacman = true;
	playersKey.unlock();
}
