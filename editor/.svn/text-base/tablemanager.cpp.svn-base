#include "editor/tablemanager.h"
#include "editor/editorscreen.h"

TableManager::TableManager(GraphHandler* graphHandler, Gtk::Window* window, Gtk::Table* table, tCell cell)
{
	_saved = false;
	_started = false;
	_graphHandler = graphHandler;
	_actualCell = cell;
	_window = window;
	_table = table;
	_rows = _table->property_n_rows();
	_columns = _table->property_n_columns();
	//redimensiono mi matriz
	_cells.resize(_rows);	
	for ( unsigned int i = 0; i < _rows ; i++){
		_cells[i].resize(_columns);
		//inicializo todos los punteros en nulo
		for ( unsigned int j = 0; j < _columns ; ++j){
			_cells[i][j] = NULL;
		}
	}
	_table->show_all();
}

bool TableManager::isEmpty(){
	if ( !_started )//si no empezo entonces esta vacio seguro
		return true;
	for ( unsigned int i = 0; i < _rows ; ++i){
		for ( unsigned int j = 0; j < _columns ; ++j){
			if ( _cells[i][j]->getCell() != cWALL )//si tengo algo distinto de Wall, no esta vacio
				return false;
		}	
	}
	return true;
}

void TableManager::reset(unsigned int rows, unsigned int columns){
	MapCell* mapcell = NULL;
	//primero libero toda la memoria que habia tomado
	for ( unsigned int i = 0; i < _rows ; ++i){
		for ( unsigned int j = 0; j < _columns ; ++j){
			delete _cells[i][j];
		}	
	}	
	
	_rows = rows;
	_columns = columns;
	//redimensiono mi matriz
	_cells.resize(_rows);	
	for ( unsigned int i = 0; i < _rows ; i++){
		_cells[i].resize(_columns);	
	}
	//redimensiono la Table
	_table->resize(_rows, _columns);
	//cargo Walls nuevamente en la Table y en la matriz
	for ( unsigned int i = 0; i < _rows ; ++i){
		for ( unsigned int j = 0; j < _columns ; ++j){
			mapcell = new MapCell(i,j, this);
			_table->attach(*mapcell, j, j+1, i, i+1);
			_cells[i][j] = mapcell;
		}	
	}
	_started = true;
	_table->show_all();
}

void TableManager::setGhostHouse(std::vector< std::pair<int,int> > & coord, tCell cell){
	for ( unsigned int k = 0; k < coord.size() ; ++k){
		_cells[ coord[k].first ][ coord[k].second ]->setImage( cell );	
	}	
}

void TableManager::setPortal(tEdgeOrientation portalType, int row, int column, tCell type){
	switch (portalType){
		//seteo la imagen para todo el portal en ambos casos
		case VERTICAL:{			
			_cells[ 0 ][ column ]->setImage( type );
			_cells[ 0 ][ column+1 ]->setImage( type );
			_cells[ 0 ][ column-1 ]->setImage( type );
			_cells[ _rows-1 ][ column ]->setImage( type );
			_cells[ _rows-1 ][ column+1 ]->setImage( type );
			_cells[ _rows-1 ][ column-1 ]->setImage( type );			
			break;	
		}
		case HORIZONTAL:{
			_cells[ row-1 ][ 0 ]->setImage( type );
			_cells[ row+1 ][ 0 ]->setImage( type );
			_cells[ row ][ 0 ]->setImage( type );
			_cells[ row-1 ][ _columns-1 ]->setImage( type );
			_cells[ row+1 ][ _columns-1 ]->setImage( type );
			_cells[ row ][ _columns-1 ]->setImage( type );			
			break;		
		}
	}
}

bool TableManager::changeElement(int row, int column, tCell type){
	// intento subir el elemento via GraphHandler
	int error = _graphHandler->addElement(row, column, type);
	//si me devuelve 0, todo salio bien y pudo incluirlo
	if ( error == 0 ){
		//pongo el salvado en false porque entonces puso algo
		_saved = false;
		tCell oldType = _cells[row][column]->getCell();
		//veo si el tipo anterior era un tipo especial, donde tengo que dibujar mas de una celda
		switch (oldType){
			case cGATE_V:{
				//seteo todo el portal como wall
				setPortal(VERTICAL, row, column, cWALL);
				break;
			}
			case cGATE_H:{
				//seteo todo el portal como wall
				setPortal(HORIZONTAL, row, column, cWALL);
				break;
			}
			//seteo los bordes como wall
			case cGHOUSE_N:{
				std::vector< std::pair<int,int> > coord(GraphHandler::getGHCoord(row, column, NORTH));
				setGhostHouse(coord, cWALL);
				break;
			}
			case cGHOUSE_E:{
				std::vector< std::pair<int,int> > coord(GraphHandler::getGHCoord(row, column, EAST));
				setGhostHouse(coord, cWALL);
				break;
			}
			case cGHOUSE_S:{
				std::vector< std::pair<int,int> > coord(GraphHandler::getGHCoord(row, column, SOUTH));
				setGhostHouse(coord, cWALL);
				break;
			}
			case cGHOUSE_W:{
				std::vector< std::pair<int,int> > coord(GraphHandler::getGHCoord(row, column, WEST));
				setGhostHouse(coord, cWALL);
				break;
			}
			default: break;
		}
		//veo si el tipo anterior era un tipo especial, donde tengo que dibujar mas de una celda
		switch (type){
			case cGATE_V:{
				//primero seteo todos los bordes como cancelados
				setPortal(VERTICAL, row, column, cCANCELED);
				// seteo el portal del lado opuesto
				if ( row == 0 ){					
					_cells[ _rows-1 ][ column ]->setImage( cGATE_V  );
				}else{
					_cells[ 0 ][ column ]->setImage( cGATE_V );	
				}
				break;
			}
			case cGATE_H:{
				//primero seteo todos los bordes como cancelados
				setPortal(HORIZONTAL, row, column, cCANCELED);
				// seteo el portal del lado opuesto
				if ( column == 0 ){
					_cells[ row ][ _columns-1 ]->setImage( cGATE_H );
				}else{
					_cells[ row ][ 0 ]->setImage( cGATE_H );
				}				
				break;
			}
			//seteo todos los bordes como cancelados
			case cGHOUSE_N:{				
				std::vector< std::pair<int,int> > coord(GraphHandler::getGHCoord(row, column, NORTH));
				setGhostHouse(coord, cCANCELED);
				break;
			}
			case cGHOUSE_E:{
				std::vector< std::pair<int,int> > coord(GraphHandler::getGHCoord(row, column, EAST));
				setGhostHouse(coord, cCANCELED);
				break;
			}
			case cGHOUSE_S:{
				std::vector< std::pair<int,int> > coord(GraphHandler::getGHCoord(row, column, SOUTH));
				setGhostHouse(coord, cCANCELED);
				break;
			}
			case cGHOUSE_W:{
				std::vector< std::pair<int,int> > coord(GraphHandler::getGHCoord(row, column, WEST));
				setGhostHouse(coord, cCANCELED);
				break;
			}
			default: break;
		}
		//ahora agrego el que recien puse en el set de dibujados!
		_drawnCells.insert( std::pair<int, int>(row, column) );
		
		return true;
	}
	reportError(error);
	//vacio los dibujados
	resetDrawn();
	return false;
}

void TableManager::reportError(int code){
	_gh.showErrorDialog(*_window, "Error inserting Element.",GraphErrors::getErrorText(code));
}

bool TableManager::changeVertexAndImage(int vertexID, tCell type){
	//calculo fila y columna a partir del numero de vertice
	unsigned int i = vertexID / _columns;//me da la fila
	unsigned int j = vertexID - (_columns * i );//me da la columna
	if ( this->changeElement(i,j, type ) ){
		//cambio la imagen
		_cells[i][j]->setImage( type );
		return true;
	}
	return false;
}

void TableManager::resetDrawn(){
	_drawnCells.clear();
}
	
void TableManager::changeAndDrawElement(int row, int column){
	if ( ! _graphHandler->isValid(row, column) )//si las coordenadas no son validas con el grafo, entonces esta afuera de la table y tengo que salir
		return;
	switch ( _actualCell ){
		case cGATE_H:
		case cGATE_V:
		case cGHOUSE_N:
		case cGHOUSE_E:
		case cGHOUSE_S:
		case cGHOUSE_W:
		case cPACMAN_START:
		case cBONUS:{
			return;	
		}
		default : break;	
	}
	std::pair <int, int> coords(row, column);
	if ( _drawnCells.find(coords) != _drawnCells.end() )//si lo encuentra es porque ya lo dibujo
		return;
	
	// primero veo si puedo cambiar la imagen.
	if ( changeElement(row,column, _actualCell ) ){
		//cambio la imagen
		_cells[row][column]->setImage( _actualCell );
		//agrego al set de dibujados
		_drawnCells.insert(coords);
	}else{
		resetDrawn();	
	}
}


TableManager::~TableManager()
{
	//destruyo todos los cell creados
	for ( unsigned int i = 0; i < _rows ; ++i){
		for ( unsigned int j = 0; j < _columns ; ++j){
			delete _cells[i][j];
		}	
	}
}
