#include "finder.h"

Finder::Finder()
{}
Finder::Finder(std::string filePath)
{
	filepath = filePath;
}
std::string Finder::run(Command* m_cmd)
{
	m_cmd->changeState(FINDER);

	displayFront();
	newmode.setColor("WHITE");

	bool run = true;
	ret_str = "NOSTRING";

	Search_System n_search;
	Input_System inp_sys;

	while (run)
	{

		newmode.resetColor();
		std::cout << "(Home)>(Finder) Enter a comand: ";
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
				run = false;
				break;
			}
			case 2: { displayAbout(); break;}
			case 3: { m_cmd->displayCommands(); break;}
			case 4: { displayManual(); break;}
			case 5: { filepath = m_cmd->getFilePath(); break; }
			case 0: 
			{
				if (filepath == "NOPATH") {
					newmode.ovrd_color("RED", "No file path specified.");
					break;
				}

				Finder_Input f_inp(in);
				f_inp.storeFilepath(filepath);

				if (inp_sys.parseString(f_inp) != 0){
					newmode.ovrd_color("RED", inp_sys.getError());
				}
				else
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

				//newmode.resetColor();
				break;
			}
			case -2: 
			{
				ret_str = in.substr(1, in.length() - 1); run = false; break;
			}//end command case = 0
		}//end switch
	}

	return ret_str;
}
void Finder::displayFront()
{
	newmode.ovrd_color("PURPLE", "\n\t::Finder Tool::");
	newmode.setColor("YELLOW");
	std::cout << "To start, "; newmode.ovrd_color("GREEN", "[load]", false); std::cout << " an EDI document." << std::endl;
	std::cout << "For a list of commands, type "; newmode.ovrd_color("GREEN", "/command", false); std::cout << "." << std::endl;
	std::cout << "For instructions and syntaxing, type "; newmode.ovrd_color("GREEN", "/manual", false); std::cout << "." << std::endl;
	std::cout << "For a description of this tool, type "; newmode.ovrd_color("GREEN", "/about", false); std::cout << ". \n" << std::endl;
	newmode.resetColor();
}
void Finder::displayAbout()
{
	newmode.ovrd_color("GREEN", "---------------------------------------------");
	newmode.ovrd_color("PURPLE", "About:");
	newmode.setColor("YELLOW");
	std::cout <<
		"The Finder Tool allows you to "; newmode.ovrd_color("GREEN", "[load]", false); std::cout << " an EDI document and "; 
		newmode.ovrd_color("GREEN", "[find]", false); std::cout << " values\n" <<
		"in the document or it's document type. You can query definitons of\n" <<
		"elements/segments as defined by the X12 EDI standard or known elements/segments\n" <<
		"contained in the document you loaded.\n" <<
		"For example, if you want to examine an 810 invoice, simply load the 810, then\n" <<
		"search for any values you want by their definition or values in the file.\n" <<
		"You must include certain settings when executing commands, please refer to the\n" <<
		"finder "; newmode.ovrd_color("GREEN", "manual", false); std::cout << " for info.\n" <<
		"Your file must be split by a newline character as well, if it's not you can\n" <<
		"use the "; newmode.ovrd_color("GREEN", "Splitter", false); std::cout << " tool.\n" <<
	std::endl;
	newmode.ovrd_color("GREEN", "---------------------------------------------");
	newmode.resetColor();
}
void Finder::displayManual()
{
	newmode.ovrd_color("GREEN", "---------------------------------------------");
	newmode.ovrd_color("PURPLE", "Manual:");
	newmode.setColor("YELLOW");
	std::cout <<
		"The finder tool has two main commands - "; newmode.ovrd_color("GREEN", "load", false); std::cout << " and "; 
		newmode.ovrd_color("GREEN", "find");
		newmode.ovrd_color("GREEN", "Load", false); std::cout << " will load any EDI document and it's respective library\n";
		newmode.ovrd_color("GREEN", "Find", false); std::cout << " will search the loaded document for a value or definition\n" <<
		"Both commands have "; newmode.ovrd_color("GREEN", "settings", false); std::cout << " the user may invoke, all settings must begin with\n" <<
		"the '"; newmode.ovrd_color("GREEN", "-", false); std::cout << "' character.\n\n" <<
		"load [filename] -[810/850/855/856/857] -[special character]\n" <<
		"   .The load command's library-type setting is MANDATORY\n"
		"   .The special character setting defines what delimiter splits each element\n" <<
		"    in your document. This is the '*' by default\n" <<
		"   .As of now, all documents to load must be .txt files.\n\n" <<
		"find [value / definition] -[(a)ll / (m)ore / (s)imple] -[#]\n" <<
		"   .The first setting in this example is the OPTIONAL display setting.\n" <<
		"    The SIMPLE setting is chosen by default.\n" <<
		"   .The second setting in this example is the OPTIONAL number to display\n" <<
		"    setting. This value must be a number > 0 and specifies how many\n" <<
		"    results to display.\n" <<
	std::endl;
	newmode.ovrd_color("GREEN", "---------------------------------------------");
	newmode.resetColor();
}