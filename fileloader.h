/********************************************************
***        D.I.V.A : File Loader BETA                 ***
*** Austin Herman - austin.herman@valvoline.com       ***
***     Updated:                                      ***
***													  ***
********************************************************/

#ifndef FILELOADER_H
#define FILELOADER_H

#include <iostream>
#include <fstream>
#include <string>
#include "DIVAlib\Types.h"
#include "DIVAlib\ColorMode.h"

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

	std::string _850library;
	std::string _810library;
	std::string ufile;
};

#endif // FILELOADER_H
