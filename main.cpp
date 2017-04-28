/**************************************************
***             D.I.V.A. - Alpha                ***
*** Author:       Austin Herman                 ***
*** Contact: austin.herman@valvoline.com        ***
*** Started: 2/24/17         Finished: 3/24/17  ***
***     Updated:                                ***
*** 3/28/2017  Fixed colors and user input      ***
**************************************************/

#include <iostream>
#include <string>
#include "DIVAlib/ColorMode.h"
#include "DIVAlib/Command.h"
#include "DIVAlib/Types.h"
#include "finder.h"
#include "splitter.h"

void runTool(std::string, Command*);
void frontPage();
void displayAbout();

int main()
{
	bool run = true;
	std::string cur_color = "";
	Command* cur_cmd = new Command(MAIN);

	ColorMode cur_mode;
	//Input       u_inp;

	frontPage();

	while (run)
	{
		std::string n_inp = "";

		cur_mode.resetColor();
		std::cout << "Run a command: ";

		cur_mode.setColor("CYAN");
		std::cin.clear();
		getline(std::cin, n_inp);
		cur_mode.resetColor();

		//u_inp = n_inp;

		switch (cur_cmd->sendCommand(n_inp))
		{
		case 1:
		{
			std::cout << "Quitting DIVA..." << std::endl;
			run = false;
			break;
		}
		case 2: {displayAbout();                 break;}
		case 3: {cur_cmd->displayCommands();     break;}
		case 0: {runTool(n_inp, cur_cmd);        break;}
		}

		//u_inp.clrdata();
	}

	return 0;
}
void runTool(std::string s_inp, Command* s_cmd)
{
	if (s_inp == "run splitter")
	{
		Splitter spl;
		spl.run(s_cmd);
	}
	else if (s_inp == "run finder")
	{
		Finder fnd;
		fnd.run(s_cmd);
	}
	else
	{
		std::cout << "Unknown command" << std::endl;
	}
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
		"DIVA stands for Document Interchange Virtual Assistant.\n " <<
		"Created by Austin Herman, DIVA serves as a handy utility with various tools \n" <<
		"to help with the EDI support team's day to day tasks. It's main purpose as \n" << 
		"of now is a time killer as I have a lot of downtime. \n" <<
		"  The Line Splitter tool can split EDI documents based on a delimiter. \n" <<
		"  The Finder tool can search EDI documents for file data or library data. \n" <<
		"More information is available within each tool's about section (/about). \n"
		"\t-Austin Herman \n" <<
		"Any suggestions, comments, or bugs can be sent to austin.herman@valvoline.com,\n" <<
		"all feedback is welcome." <<
		std::endl;

	n_mode.resetColor();
}
