/*
 * main() para test de las clases XmlWorld y XmlMap
 * */
#include "common/xmlworld.h"
#include "common/xmlmap.h"
#include "common/world.h"
#include "common/helper.h"

#include <iostream>
using namespace std;

/* renombrar a main para probar... */
int main_xml_tester_02(int argc, char **argv) {
// int main(int argc, char **argv) {
	
	MapFileList mapFileList;
	string destinationPath = "/home/mmilicich/world.world";
	
	try {
		// XmlWorld::Load("/home/mmilicich/pacmanworld_01.xml.world", mapFileList);
		XmlWorld::Load(destinationPath, mapFileList);
	} catch(string e) {
		cerr << "ERROR al hacer XmlWorld::Load(): " << e << endl;
		return 1; 
	}

	
	try {
    	XmlWorld::Save(destinationPath, mapFileList);
	} catch(string e) {
		cerr << "ERROR al hacer XmlWorld::Save(): " << e << endl;
		return 2;
	}
	
	return 0;
}
