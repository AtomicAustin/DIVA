/********************************************************
***       D.I.V.A : Question Interface BETA           ***
*** Austin Herman - austin.herman@valvoline.com       ***
***     Updated:                                      ***
***	3/28/2017  changed color mode for command input	  ***
*** 5/08/2017 added filepath						  ***
********************************************************/

#ifndef QUESTION_H
#define QUESTION_H

#include <iostream>
#include <fstream>
#include <string>
#include "ColorMode.h"
#include "Command.h"
#include "Types.h"
#include "UserInput.h"
#include "input_system.h"

class Question
{
public:
    Question();
	Question(std::string);
	std::string pollQuestion(spltr::QuesState);
    bool validateData(spltr::QuesState, std::string&);
	void setFilePath(std::string);

private:
    ColorMode curMode;
    std::string questions[5];
	std::string filepath;
};

#endif // QUESTION_H
