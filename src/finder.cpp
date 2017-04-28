#include "finder.h"

Finder::Finder()
{}
void Finder::run(Command* m_cmd)
{
	m_cmd->changeState(FINDER);

	displayFront();

	bool run = true;

	Search_System n_search;
	Input_System inp_sys;

	while (run)
	{
		newmode.resetColor();

		std::cout << "Enter a comand: ";
		std::string in = "";

		newmode.setColor("CYAN");
		getline(std::cin, in);
		newmode.resetColor();

		switch (m_cmd->sendCommand(in))
		{
			case 1: 
			{ 
				newmode.ovrd_color("YELLOW", "Ending finder."); 
				m_cmd->changeState(MAIN);
				return;
			}
			case 2: { displayAbout(); break;}
			case 3: { m_cmd->displayCommands(); break;}
			case 4: { displayManual(); break;}
			case 0: 
			{
				Finder_Input f_inp(in);

				switch (inp_sys.parseString(f_inp))
				{
					case 1: {newmode.ovrd_color("RED", "Error: Bad command"); break;}
					case 2: {newmode.ovrd_color("RED", "Error: Bad file name"); break;}
					case 3: {newmode.ovrd_color("RED", "Error: Bad setting name"); break;}
					case 4: {newmode.ovrd_color("RED", "Error: Bad query"); break;}
					case 5: {newmode.ovrd_color("RED", "Error: Unknown error"); break;}
					case 0:
					{
						if (inp_sys.getState() == FIND)
						{
							if (n_search.filesLoaded()) {
								n_search.findQuery(f_inp.getPackage());
							}
							else {
								newmode.ovrd_color("YELLOW", "No files loaded.");
							}
							break;
						}
						else if (inp_sys.getState() == LOAD) { n_search.loadFiles(f_inp.getPackage()); break; }
					}
				}
			}//end command case = 0
		}//end switch
	}
}
void Finder::displayFront()
{
	newmode.setColor("YELLOW");
	std::cout << "\t::Finder Tool::" << std::endl;
	std::cout << "To start, [load] an EDI document." << std::endl;
	std::cout << "For a list of commands, type /command." << std::endl;
	std::cout << "For instructions and syntaxing, type /manual." << std::endl;
	std::cout << "For a description of this tool, type /about." << std::endl;
	newmode.resetColor();
}
void Finder::displayAbout()
{
	newmode.setColor("YELLOW");
	std::cout <<
		"\t::Finder About::\n" <<
		"The Finder Tool allows you to[load] an EDI document and[find] various values\n" <<
		"in the document or it's document type. You can query definitons of \n" <<
		"elements / segments as defined by the X12 EDI standard or known elements / segments\n" <<
		"contained in the document you loaded.\n" <<
		"For example, if you want to examine an 810 invoice, simply load the 810, then\n" <<
		"search for any values you want by definition or file values.\n" <<
		"Acceptable Queries include :\n" <<
		"find N9                  *Returns the full line of all instances of the ISA\n" <<
		"                          segment*\n" <<
		"find CUR > USD           *will look only at the CUR segment for the USD value*\n" <<
		"find Date Time Reference *Will return the library definitions as defined by the\n" <<
		"                          X12 EDI standard*\n" <<
	std::endl;
	newmode.resetColor();
}
void Finder::displayManual()
{
	newmode.setColor("YELLOW");
	std::cout <<
		"\t::Finder Manual::\n" <<
		"The finder tool has two main commands - Load and Find.\n" <<
		"Load will load any EDI document and it's respective library\n" <<
		"Find will search the loaded document for a value or definition\n" <<
		"Both commands have settings the user may invoke, all settings must begin with\n" <<
		"the '-' character.\n" <<
		"\n" <<
		"load[filename] - [810 / 850]\n" <<
		"   .The load command's setting is MANDATORY - you will recieve an error \n" <<
		"    if it's not included.\n" <<
		"   .As of now, all documents to load must be.txt files.\n" <<
		"\n" <<
		"find[value / definition] - [(a)ll / (m)ore / (s)imple] - [#]\n" <<
		"   .The first setting in this example is the OPTIONAL display setting.\n" <<
		"    The SIMPLE setting is chosen by default.\n" <<
		"   .The second setting in this example is the OPTIONAL number to display\n" <<
		"    setting.This value must be a number > 0 and specifies how many\n" <<
		"    results to display.\n" <<
	std::endl;
	newmode.resetColor();
}
