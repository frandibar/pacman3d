#ifndef __ELEMENTMANAGER_H__
#define __ELEMENTMANAGER_H__
#include "lib/mutex.h"
#include "lib/element.h"

/**
 * Clase para administrar los elementos dinámicos en un mapa
 * Permite encolar elementos cuyo estado cambio, para ser mandados en el proximo status
 * Metodos protegidos por mutex
 */
class ElementManager {
  
  private:
    ElementList _elementsUpdate;
    Mutex _mtx;
    
  public:
  	/// constructor
    ElementManager();
    
    /// destructor
    ~ElementManager();
    
    /// inicializa manager
    void reset();
    
    /// agrega un nuevo elemento a enviar
    void addElement(const Element &elem);
    
    /**
     * devuelve copia de listado de elementos y borra la lista interna
     * DEBE LIBERARSE EL PUNTERO DESDE AFUERA
     */
    ElementList* getElementsAndWipe();
};

#endif /* __ELEMENTMANAGER_H__ */
