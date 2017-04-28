/********************************************************
***       D.I.V.A : Question Interface BETA           ***
*** Austin Herman - austin.herman@valvoline.com       ***
***     Updated:                                      ***
***	3/28/2017  changed color mode for command input	  ***
********************************************************/

#ifndef QUESTION_H
#define QUESTION_H

#include <iostream>
#include <fstream>
#include <string>
#include "DIVAlib\ColorMode.h"
#include "DIVAlib\Command.h"
#include "DIVAlib\Types.h"
#include "DIVAlib\UserInput.h"
#include "input_system.h"

class Question
{
public:
    Question();
	std::string pollQuestion(spltr::QuesState);
    bool validateData(spltr::QuesState, std::string&);

private:
    ColorMode curMode;
    std::string questions[5];
};

#endif // QUESTION_H
