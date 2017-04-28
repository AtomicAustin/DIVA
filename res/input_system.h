/****************************************************************
***             D.I.V.A : Input System BETA                   ***
*** Austin Herman - austin.herman@valvoline.com               ***
***     Updated:                                              ***
***	3/28/2017 -combined splitter/finder functions       	  ***
***			  -added functionality with library input class	  ***
****************************************************************/

#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include <iostream>
#include <fstream>
#include <string>
#include "DIVAlib\Types.h"
#include "DIVAlib\ColorMode.h"
#include "DIVAlib\UserInput.h"

class Input_System
{
public:
	Input_System();
	int parseString(Finder_Input&);
	bool isCommand(std::string&);
	void getSetting(std::string&, Finder_Input&);
	bool validSetting(std::string&, Finder_Input&);
	void getQuery(std::string&, Finder_Input&);
	void getFilename(std::string&, Finder_Input&);
	std::string stripSpaces(std::string);
	bool isInt(std::string&);
	fndr::INPUT_STATE getState();
	//Finder_Input getInput();

private:
	//Finder_Input inp;
	fndr::INPUT_STATE cur_state;

	ColorMode curColor;

	int err_code;   //0 - good
					//1 - bad command
					//2 - bad file name
					//3 - bad setting name
					//4 - unknown error

	static const int d_setting_size = 6;
	static const int l_setting_size = 2;

	std::string disp_setting[d_setting_size] = {
		"a", "all", "m", "more", "s", "simple"
	};
	std::string load_setting[l_setting_size] = {
		"810", "850"
	};
public:
	Splitter_Input breakup(std::string in);

};

#endif // INPUT_SYSTEM_H
