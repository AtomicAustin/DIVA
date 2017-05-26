/****************************************************
***             D.I.V.A - FINDER  BETA            ***
*** Austin Herman - austin.herman@valvoline.com   ***
*** Started: 3/3/17         Finished: 3/21/2017   ***
***     Updated:                                  ***
*** 3/23/2017  big changes to integrate with full ***
***            DIVA solution                      ***
*** 3/28/2017  changed color mode / command input ***
*** 5/02/2017  Fixed formatting/colors            ***
*** 5/08/2017  Added filepath                     ***
****************************************************/

#ifndef FINDER_H
#define FINDER_H

#include <iostream>
#include <string>
#include "Types.h"
#include "ColorMode.h"
#include "Command.h"
#include "input_system.h"
#include "search_system.h"

using namespace fndr;

class Finder
{
public:
	Finder();
	Finder(std::string);
	std::string run(Command*);
	void displayFront();
	void displayAbout();
	void displayManual();

private:
	std::string filepath;
	std::string ret_str;
	ColorMode newmode;
};

#endif // FINDER_H