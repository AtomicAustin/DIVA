/**************************************************
***             D.I.V.A. - Alpha                ***
*** Author:       Austin Herman                 ***
*** Contact: austin.herman@valvoline.com        ***
*** Started: 2/24/17         Finished: 3/24/17  ***
***     Updated:                                ***
*** 3/28/2017  Fixed colors and user input      ***
*** 5/08/2017  Added < command			        ***
*** 5/08/2017 added filepath                    ***
**************************************************/

#include <iostream>
#include <string>
#include "ColorMode.h"
#include "Command.h"
#include "Types.h"
#include "finder.h"
#include "splitter.h"
#include <fstream>

std::string runTool(std::string, Command*);
void frontPage();
void displayAbout();

int main()
{
	bool run = true;
	std::string cur_color = "";
	Command* cur_cmd = new Command(MAIN);

	ColorMode cur_mode;

	frontPage();

	std::string n_inp = "NOSTRING";
	std::string filepath = "files/";

	while (run)
	{
		cur_mode.resetColor();
		
		if (n_inp == "NOSTRING")
		{
			n_inp = "";

			std::cout << "(Home) Run a command: ";

			cur_mode.setColor("CYAN");
			std::cin.clear();
			getline(std::cin, n_inp);
			cur_mode.resetColor();
		}

		switch (cur_cmd->sendCommand(n_inp))
		{
			case 1:
			{
				std::cout << "Quitting DIVA..." << std::endl;
				run = false;
				break;
			}
			case 2: { displayAbout(); break;}
			case 3: { cur_cmd->displayCommands(); break;}
			case 4: { filepath = cur_cmd->getFilePath(); break; }
			case 0: { n_inp = runTool(makelower(stripSpaces(n_inp)), cur_cmd); continue;}
			//case -1: { break; }
		}

		n_inp = "NOSTRING";
	}

	return 0;
}
std::string runTool(std::string s_inp, Command* s_cmd)
{
	if (s_inp.substr(0, 3) == "run")
	{
		s_inp = stripSpaces(s_inp.substr(3, s_inp.length() - 3));

		if (s_inp == "splitter") {
			Splitter spl(s_cmd->getFilePath());
			return spl.run(s_cmd);
		}
		else if (s_inp == "finder")
		{
			Finder fnd(s_cmd->getFilePath());
			return fnd.run(s_cmd);
		}
		else {
			std::cout << s_inp << " -- Unknown tool" << std::endl;
		}
	}
	else{
		std::cout << s_inp << " -- Unknown command" << std::endl;
	}

	return "NOSTRING";
}
void frontPage()
{
	ColorMode n_mode;
	n_mode.setColor("PURPLE");
	std::cout << "******************************************************" << std::endl;
	std::cout << "**\t      __     ___      _       __     \t    **" << std::endl;
	std::cout << "**\t     /|||   |||||    |||  /| |||\\\t    **" << std::endl;
	std::cout << "**\t    //|||    |||     ||| //| |||\\\\\t    **" << std::endl;
	std::cout << "**\t   ///|||    |||     |||///  |||\\\\\\\t    **" << std::endl;
	std::cout << "**\t  ///_|||   _|||_    |||//   |||_\\\\\\\t    **" << std::endl;
	std::cout << "**\t ||||||/    |||||    |||/    |||||\\\\\\\t    **" << std::endl;
	std::cout << "** \tDocument Interchange Virtual Assistant\t    ** " << std::endl;
	std::cout << "** \tAuthor: Austin Herman\t\t\t    **" << std::endl;
	std::cout << "** \taustin.herman@valvoline.com\t\t    **" << std::endl;
	std::cout << "******************************************************" << std::endl;

	n_mode.setColor("YELLOW");
	std::cout << "\n | For a list of commands, type /command | \n" << std::endl;

	n_mode.setColor("WHITE");
}
void displayAbout()
{
	ColorMode n_mode;
	n_mode.setColor("YELLOW");
	std::cout <<
		"DIVA stands for Document Interchange Virtual Assistant.\n" <<
		"Created by Austin Herman, DIVA serves as a handy utility with various tools \n" <<
		"to help with the EDI support team's day to day tasks. It's main purpose as \n" << 
		"of now is a time killer. \n" <<
		"The Line Splitter tool can split EDI documents based on a delimiter. \n" <<
		"For example, replace all '~' characters with the newline character to read\n" <<
		"EDI documents.\n" <<
		"The Finder tool allows the user to search for a value within an EDI document \n" <<
		"or find a value at a segment in an EDI document. It returns all available \n" <<
		"information about the value if found. \n" <<
		"More information is available within each tool's about section (/about). \n"
		"Any suggestions, comments, or bugs can be sent to austin.herman@valvoline.com,\n" <<
		"all feedback is welcome." <<
		std::endl;

	n_mode.resetColor();
}
