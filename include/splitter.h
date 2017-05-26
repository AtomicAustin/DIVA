/*******************************************************\
***      D.I.V.A : Line Splitter BETA                 ***
*** Austin Herman - austin.herman@valvoline.com       ***
*** Started: 2/13/17         Finished: 2/15/17        ***
***     Updated: 2/23/2017                            ***
*** 2/23/2017  Added color modes and line numbers     ***
*** 3/23/2017  Integrated with DIVA project           ***
*** 3/28/2017  fixed color mode, added input library  ***
*** 3/31/2017  removed split another question, fixed  ***
***  issue with \n as delimiter                       ***
*** 5/08/2017 added filepath                          ***
\*******************************************************/

#ifndef SPLITTER_H
#define SPLITTER_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "question.h"
#include "ColorMode.h"
#include "Command.h"
#include "Types.h"

class Splitter
{
public:
    Splitter();
	Splitter(std::string);
    std::string run(Command*);
	std::string pollQuestion();
	bool checkCommand(std::string);
	void nextState();
	void previousState();
	void clearData();
	void placeData(std::string);
	spltr::QuesState Splitter::getState();
    bool split(spltr::Data);
    std::string insertLinenums(std::string);
	void displayAbout();
	void displayFront();

private:
	std::string filepath;

	Command* m_cmd;
	ColorMode curMode;
	spltr::QuesState cur;
    Question nQues;
	spltr::Data data;
};

#endif // SPLITTER_H
