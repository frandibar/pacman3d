#include "grapherrors.h"

std::string GraphErrors::errors[14] = {"No element can be placed in that position.", "Bonus element already exists.",
										"Four elements cannot be together as a square.", "Pacman Start already exists.", 
										"Ghost House already exists.", "Invalid position for portal.", 
										"Not enough room for that element.", "Ghost House cannot be placed on the edges of the map.",
										"No Pills or PowerUp elements have been inserted.", "Bonus element has to be inserted.",
										"Pacman Start has to be inserted.", "Ghost House has to be inserted.", "Map cannot be disconnected.",
										"Map must be cycle."};

std::string GraphErrors::getErrorText(int code){
	return errors[code-1];
}

GraphErrors::GraphErrors()
{
}

GraphErrors::~GraphErrors()
{
}
