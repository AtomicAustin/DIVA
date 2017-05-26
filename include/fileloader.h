/********************************************************
***        D.I.V.A : File Loader BETA                 ***
*** Austin Herman - austin.herman@valvoline.com       ***
***     Updated:                                      ***
***	5/08/2017  Delimiter functionality				  ***
*** 5/08/2017 added filepath                          ***
********************************************************/

#ifndef FILELOADER_H
#define FILELOADER_H

#include <iostream>
#include <fstream>
#include <string>
#include "Types.h"
#include "ColorMode.h"

using namespace fndr;

class FileLoader
{
public:
	FileLoader();
	std::vector<Doc_Value> loadFile(Input_Package);
	std::vector<Doc_Value> loadLibrary(Input_Package);

	Doc_Value storeElements(std::string&, Doc_Value);
	std::string removeTabs(std::string&);
	//virtual ~FileLoader();

protected:

private:
	ColorMode curColor;

	std::string _810library;
	std::string _850library;
	std::string _855library;
	std::string _856library;
	std::string _857library;
	std::string ufile;

	std::string delimiter;
};

#endif // FILELOADER_H
