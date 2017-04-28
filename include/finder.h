/****************************************************
***             D.I.V.A - FINDER  BETA            ***
*** Austin Herman - austin.herman@valvoline.com   ***
*** Started: 3/3/17         Finished: 3/21/2017   ***
***     Updated:                                  ***
*** 3/23/2017  big changes to integrate with full ***
***            DIVA solution                      ***
*** 3/28/2017  changed color mode / command input ***
****************************************************/

#ifndef FINDER_H
#define FINDER_H

#include <iostream>
#include <string>
#include "DIVAlib\Types.h"
#include "DIVAlib\ColorMode.h"
#include "DIVAlib\Command.h"
#include "input_system.h"
#include "search_system.h"

using namespace fndr;

class Finder
{
public:
	Finder();
	void run(Command*);
	void displayFront();
	void displayAbout();
	void displayManual();

private:
	ColorMode newmode;
};

#endif // FINDER_H
