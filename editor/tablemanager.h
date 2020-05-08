#ifndef TABLEMANAGER_H_
#define TABLEMANAGER_H_

#include <gtkmm.h>
#include "editor/mapcell.h"
#include "lib/utilities.h"
#include "lib/graphhandler.h"
#include "lib/grapherrors.h"
#include "lib/gladehelper.h"
#include <vector>
#include <map>
#include <set>

/**
 * Para poder manejar la GtkTable, una vez obtenida del XML generado por Glade, ésta es entregada al TableManager, que se encargará de todo su manejo e interacción con el GraphHandler (que será explicado luego).
 * El TableManager guarda entre sus atributos a la GtkTable, el tipo de celda que se ha seleccionado para agregar, la cantidad de filas y columnas de la Table, el GtkWindow que lo contiene, para así poder mostrar mensajes de error gracias a un GladeHelper que figura como otro de sus atributos, un GraphHandler con el cual se comunicará para validar que lo que se quiere ingresar sea válido para el mapa, y por ultimo una matriz de MapCell*, que se encargará de guardar por cada uno de ellos que se crea, y luego los eliminará cuando la clase se desinstancie.
 * De esta forma, el EditorScreen toma el Table, inicializa el grafo y el GraphHandler, e inicializa también el TableManager, entregándole los atributos que necesita. Luego, cuando se seleccione un botón de elemento a colocar, el tipo de celda del TableManager se modificará y así­ podrá saber cual es la celda que se quiere agregar. Luego, cuando se haga un click en la celda correspondiente, la misma reaccionará avisando al TableManager. Éste, obtendrá las coordenadas de la celda que fue clickeada. Con esto, podrá evaluar (gracias al GraphHandler) si es posible colocar la nueva celda que se quiere colocar en esa posición o no, y en caso de ser posible, devolverá un valor de verdad al MapCell que fue clickeado, para que éste cambie su imagen a mostrar. En casos especiales (GhostHouse y Portales) el TableManager se encargará de avisar a ciertos MapCell de alrededor del seleccionado para que modifiquen su imagen a mostrar.
 * Además de esto, si la celda de introdujo con éxito, el TableManager guarda las coordenadas de la celda seleccionada en un set. Esto lo hace porque si el usuario no soltó el botón del Mouse, podrá arrastrarse sobre otras celdas, y ellas reaccionarán pidiendo también al TableManager que introduzca un elemento y cambie su imagen. Lo que hará que todas las celdas por las que se va pasando y dibujando queden almacenadas en el set, para no redibujarlas cada vez que se pasa por sobre ellas. Y luego, cuando se suelte el botón del Mouse, se logrará que ese set se vacie para la proxima insersion.
 * Esta posibilidad de insertar varios elementos a la vez no sirve para elementos especiales o que pueden ser insertados una sola vez en el mapa, como bonus, pacman start, ghost house y portales.
 * En caso de que el GraphHandler notificara que no se pudo incluir un vértice con elemento seleccionado, se obtendrá el entero correspondiente al error. Con este entero y gracias a la clase GraphErrors (que no tiene más que un vector de strings con los mensajes de error correspondientes a cada código, y un método para obtener el string correspondiente) se obtiene el mensaje de error para pasar al GladeHelper, para que este último pueda hacer aparecer una ventana de error con ese mensaje.
 * */
class TableManager {
private:		
	bool _started;
	unsigned int _rows;
	unsigned int _columns;
	tCell _actualCell;
	Gtk::Window* _window;
	Gtk::Table* _table;
	std::vector< std::vector< MapCell* > > _cells;
	GraphHandler* _graphHandler;
	GladeHelper _gh;
	bool _saved;
	//aca van las coordenadas de las celdas que ya fueron dibujadas en un arrastre de mouse
	std::set< std::pair< int, int > > _drawnCells;
	
	void setGhostHouse(std::vector< std::pair<int,int> > & coord, tCell cell);
	void setPortal(tEdgeOrientation portalType, int row, int column, tCell type);
	
	//los ocultamos por seguridad
	TableManager (const TableManager&);
	TableManager& operator=(const TableManager&);
	
public:
	/// 
	TableManager(GraphHandler* graphHandler, Gtk::Window* window, Gtk::Table* table, tCell cell);
	
	///
	inline void setActualCell(tCell cell){
		_actualCell = cell;
	}
	
	/// 
	inline tCell getActualCell() const{
		return _actualCell;	
	}
	
	/// 
	inline void changeGraphHandler(GraphHandler* graphHandler){
		_graphHandler = graphHandler;	
	}
	
	/// 
	inline void setSaved(bool saved){
		_saved = saved;	
	}
	
	/// 
	inline bool isSaved()const{
		return _saved;
	}
	
	/// 
	inline bool hasStarted()const{
		return _started;	
	}
	
	/// modifica el size de filas y columnas de la tabla, volviendo a cargar walls en cada celda
	void reset(unsigned int  rows, unsigned int  columns);
	
	/// intenta colocar el elemento pedido en la posicion pedida comunicandose con el GraphHandler, devuelve false se no se pudo
	bool changeElement(int row, int column, tCell type);
	
	/// reporta el error mediante un mensaje, mediante un metodo del editorScreen
	void reportError(int code);
	
	/// avisa si el TableManager esta vacio, que significa que todos sus cell son walls
	bool isEmpty();
	
	/// intenta colocar el elemento pedido por tCell, en el vertice pedido. Devuelve false si no pudo hacerlo
	bool changeVertexAndImage(int vertexID, tCell type);
	
	/// limpia el set de coordenadas de elementos ya dibujados durante un arrastre del mouse con boton apretado
	void resetDrawn();
	
	/// inserta el elemento y lo dibuja en la celda de la coordenada pasada (si esta es valida)
	void changeAndDrawElement(int row, int column);

	/// 		
	virtual ~TableManager();
};

#endif /*TABLEMANAGER_H_*/
