#ifndef MAPCELL_H_
#define MAPCELL_H_

#include <gtkmm.h>
#include <string>
#include <iostream>
#include "lib/utilities.h"

class TableManager;

/**
 * Para implementar estos GtkEventBox, que además se necesitaba que sepan su posición en el Table, dado que debe validarse que en la posición indicada pueda colocarse el elemento seleccionado, se utilizó la clase MapCell. Esta clase hereda de EventBox, y tiene como atributos el GtkImage que irá dentro de si mismo (dado que es un GtkEventBox), su tipo de celda (de elementos: Pill, Bonus, PacmanStart, etc), y las coordenadas en el Table donde se encuentra, en forma matricial ( fila, columna ).
 * */
class MapCell: public Gtk::EventBox
{
private:
	int i;
	int j;
	TableManager* tm;
	Gtk::Image* _image;
	tCell _cell;
	
	//los ocultamos por seguridad
	MapCell ( const MapCell&);
	MapCell& operator=( const MapCell&);

public:
	///
	MapCell(int i, int j, TableManager* tm);
	/// se encarga de validar si puede modificar la imagen que muestra, y si puede lo hace
	bool on_cell_clicked(GdkEventButton* event);
	/// modifica la imagen que muestra por la indicada segun el tCell pasado
	void setImage(tCell cell);
	/// getter
	inline tCell getCell() const{
		return _cell;	
	}
	///
	virtual ~MapCell();
	
	///
	bool on_mouse_moving(GdkEventMotion* event);
	
	///
	bool on_button_release(GdkEventButton* event);
	
	///
	static const int CELL_SIZE;
};

#endif /*MAPCELL_H_*/
