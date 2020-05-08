#include "editor/mapcell.h"
#include "editor/tablemanager.h"

const string dir = "/etc/Pacman3D/";
const int MapCell::CELL_SIZE = 32;

MapCell::MapCell(int i, int j, TableManager* tm)
{
	this->i = i;
	this->j = j;
	this->tm = tm;
	
	//inicializo el hijo con una imagen
	_image = new Gtk::Image("/etc/Pacman3D/img/wall.bmp");
	_cell = cWALL;
	//meto la GtkImage en el EventBox
	add(*_image);
	//le seteo eventos de botones para el mouse
	set_events(Gdk::BUTTON_PRESS_MASK);
	//set_events(Gdk::POINTER_MOTION_MASK);
	set_events(Gdk::BUTTON_RELEASE_MASK);
	//relaciono el evento de click con el correspondiente metodo
	signal_button_press_event().connect( sigc::mem_fun(*this, &MapCell::on_cell_clicked) );
	
	signal_motion_notify_event().connect( sigc::mem_fun(*this, &MapCell::on_mouse_moving) );
	
	signal_button_release_event().connect( sigc::mem_fun(*this, &MapCell::on_button_release) );
}

bool MapCell::on_mouse_moving(GdkEventMotion* event){
	
	int coordY = ( ( (int)event->y < 0)? ((int)event->y - CELL_SIZE) : ((int)event->y) );
	int coordX = ( ( (int)event->x < 0)? ((int)event->x - CELL_SIZE) : ((int)event->x) );
	
	int deltaRows = coordY / CELL_SIZE;
	int deltaColumns = coordX / CELL_SIZE;
	
	int actualRow = i + deltaRows;
	int actualColumn = j + deltaColumns;
		
	//tengo que pedirle que dibuje la celda actual
	tm->changeAndDrawElement(actualRow, actualColumn);
	
	return true;
}

bool MapCell::on_button_release(GdkEventButton* event){
	
	tm->resetDrawn();
	
	return true;
}


void MapCell::setImage(tCell cell){
	_cell = cell;
	std::string imageFile;
        switch ( cell ){
        	case cPACMAN_START:{
        		imageFile = "img/pacman.bmp";
        		break;
        	}
			case cPILL:{
				imageFile = "img/pill.bmp";
				break;
			}
			case cPOWER_UP:{
				imageFile = "img/powerup.bmp";
				break;	
			}
			case cBONUS:{
				imageFile = "img/cherry.bmp";
				break;	
			}
			case cWALL:{
				imageFile = "img/wall.bmp";
				break;
			}
			case cEMPTY:{
				imageFile = "img/none.bmp";
				break;
			}
			case cGATE_V:{
				imageFile = "img/tunnelV.bmp";
				break;
			}
			case cGATE_H:{
				imageFile = "img/tunnelH.bmp";
				break;
			}
			case cGHOUSE_N:{
				imageFile = "img/houseN.bmp";
				break;
			}
			case cGHOUSE_E:{
				imageFile = "img/houseE.bmp";
				break;
			}
			case cGHOUSE_S:{
				imageFile = "img/houseS.bmp";
				break;
			}
			case cGHOUSE_W:{
				imageFile = "img/houseW.bmp";
				break;
			}
			case cCANCELED:{
				imageFile = "img/canceled.bmp";
				break;	
			}
        }
    
    //creo el pixbuf correspondiente
	Glib::RefPtr<Gdk::Pixbuf> drawing = Gdk::Pixbuf::create_from_file(dir + imageFile);
	//cambio la imagen
	_image->set(drawing);
	
}

bool MapCell::on_cell_clicked(GdkEventButton* event){
	// primero pregunto al table manager si puedo cambiar la imagen.
	if ( tm->changeElement(i,j, tm->getActualCell() ) ){
		//cambio la imagen
		setImage( tm->getActualCell() );	
	}
	return true;
}

MapCell::~MapCell()
{
	delete _image;
}
