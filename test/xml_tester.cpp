/*
 * main() para test de las clases XmlWorld y XmlMap
 * */
#include "common/xmlworld.h"
#include "common/xmlmap.h"
#include "common/world.h"
#include "common/helper.h"

/* renombrar a main para probar */
int main_xml_tester(int argc, char **argv) {	
// int main(int argc, char **argv) {
	
	World world;
	
	XmlWorld::Load("/home/mmilicich/pacmanworld_02.xml.world", world);

	string destinationPath = "/home/mmilicich";
	
    LevelList * lvls = world.getLevels();
    int i = 1;
    for(LevelList::iterator it = lvls->begin(); it != lvls->end(); ++it) {
    	string fileName = "map" + StringConverter<int>(i) + ".map";
    	XmlMap::Save(fileName, destinationPath, (* ((*it)->getPacManMap()) ) );
    	i++;
    }
    
    // 2007-12-03 mmilicich: XmlWorld YA NO GUARDA COMO WORLD. 
    // XmlWorld::Save("world.world", destinationPath, world);
	
	return 0;
}
