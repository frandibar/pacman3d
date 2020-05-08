#ifndef WRAPPER_H_
#define WRAPPER_H_

/**
 * Wrapper es una clase que sirve para wrapear un puntero a un objeto, para luego poder utilizar metodos de ese objeto como la comparacion == o <
 */
template < class TApuntado > class Wrapper {

private:
	TApuntado* pointed;

public:
	///constructor que pide al puntero al objeto ya inicializado
	Wrapper(TApuntado* objeto){
		pointed = objeto;
	}
	///constructor que no pide al puntero sino que inicializa en 0
	Wrapper(){
		pointed = 0;
	}
	///destructor que no hace nada. Tampoco hace delete del objeto
	virtual ~Wrapper(){
	}
	///sobrecarga del operador == para poder utilizar el metodo == del objeto apuntado
	bool operator==(const Wrapper& other) const {
		return ( (*pointed) == (*(other.pointed)) );
	}
	///sobrecarga del operador < para poder utilizar el metodo < del objeto apuntado
	bool operator<(const Wrapper& other) const{
		return ( (*pointed) < (*(other.pointed)) );	
	}
	///devuelve el puntero al objeto, contenido
	TApuntado* getPointer(){
		return pointed;
	}
	///setea el puntero al objeto contenido
	void setPointer(TApuntado* objeto){
		pointed = objeto;
	}
};

#endif /*WRAPPER_H_*/
