#ifndef GRAPHERRORS_H_
#define GRAPHERRORS_H_

#include <string>

/**
 * Manejo de errores de Graph
 * 
 * @sa Graph
 */
class GraphErrors
{
private:

	GraphErrors();
	static std::string errors[14];
	
public:
	/// 
	virtual ~GraphErrors();
	
	/// devuelve un std::string con el mensaje de error segun el codigo correspondiente
	static std::string getErrorText(int code);
};

#endif /*GRAPHERRORS_H_*/
