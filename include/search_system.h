/********************************************************
***          D.I.V.A : Search System BETA             ***
*** Austin Herman - austin.herman@valvoline.com       ***
***     Updated:                                      ***
***	4/24/2017 Added element count to return results	  ***
********************************************************/

#ifndef SEARCH_SYSTEM_H
#define SEARCH_SYSTEM_H

#include <iostream>
#include <vector>
#include <string>
#include "DIVAlib\Types.h"
#include "DIVAlib\ColorMode.h"
#include "fileloader.h"

using namespace fndr;

class Search_System
{
public:
	Search_System();
	void loadFiles(Input_Package);
	void clear(std::string);
	bool filesLoaded();

	void findQuery(Input_Package);
	bool checkValue(Input_Package, std::vector<Doc_Value>&, int, std::string);
	bool storeValues(Found_Value&, Data_Type, Doc_Value, int, int);
	void fillFileData();
	void storeFileData(int, std::string, int);
	void fillLibraryData();
	void storeLibraryData(int, std::string, int);
	void storeFileLine();
	void storeLibraryDefinition();
	void displayFound(Input_Package);
	//virtual ~Search_System();

private:
	ColorMode curColor;

	FileLoader f_loader;
	bool file_loaded;
	bool library_loaded;

	int lib_data_size;
	int file_data_size;

	std::vector<Doc_Value> lib_data;
	std::vector<Doc_Value> file_data;

	std::vector<Found_Value> to_return;
};

#endif // SEARCH_SYSTEM_H
