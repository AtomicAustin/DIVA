#include "splitter.h"

Splitter::Splitter()
{
    //ctor
	cur = spltr::O_FN;
	data.sfn = "NOSTRING";
}
Splitter::Splitter(std::string filePath)
{
	filepath = filePath;
	nQues.setFilePath(filePath);
	cur = spltr::O_FN;
	data.sfn = "NOSTRING";
}
std::string Splitter::run(Command* in_cmd)
{
	std::string ret_str = "NOSTRING";

    displayFront();

	m_cmd = in_cmd;
    m_cmd->changeState(SPLITTER);

	ret_str = pollQuestion();

	m_cmd->changeState(MAIN);

	return ret_str;
}
std::string Splitter::pollQuestion()
{
	while (cur < 5)
	{
		std::string user_answer = nQues.pollQuestion(cur);

		switch (m_cmd->sendCommand(user_answer))
		{
			case -2: { return user_answer.substr(1, user_answer.length() - 1); }
			case 1: { curMode.ovrd_color("YELLOW", "Ending Splitter"); return "NOSTRING"; }
			case 2: { clearData(); cur = spltr::O_FN; continue; }
			case 3: { previousState(); continue; }
			case 4: { displayAbout(); continue; }
			case 5: { m_cmd->displayCommands(); continue; }
			case 6: { filepath = m_cmd->getFilePath(); nQues.setFilePath(filepath); continue; }
		}


		/*if (checkCommand(user_answer))
		{
			if (m_cmd->getCommandCode() == 1) 
			{
				curMode.ovrd_color("YELLOW", "Ending Splitter");
				return "NOSTRING";
			}
			else if (m_cmd->getCommandCode() == -2){
				return user_answer.substr(1, user_answer.length() - 1);
			}
			else {
				continue;
			}
		}*/
		user_answer = stripSpaces(user_answer);

		if (nQues.validateData(cur, user_answer))
		{
			placeData(user_answer);
			nextState(); 
		}
		
		if (cur == spltr::DONE) {
			if (!split(data)) {
				curMode.ovrd_color("RED", "Splitter could not process.");
			}
			clearData();
			//nextState();
		}
	}
}
bool Splitter::checkCommand(std::string u_answer)
{
	switch (m_cmd->sendCommand(u_answer))
	{
		case -2: { return true; }
		case 1: { return true; }
		case 2: { clearData(); cur = spltr::O_FN; return true; }
		case 3: { previousState(); return true; }
		case 4: { displayAbout(); return true; }
		case 5: { m_cmd->displayCommands(); return true; }
	}

	return false;
}
void Splitter::nextState()
{
	//O_FN, DELIMITER, REPLACE, YN, S_FN, DONE
	switch (cur)
	{
		case 0: { cur = spltr::DELIMITER; break; }
		case 1: { cur = spltr::REPLACE; break; }
		case 2: { cur = spltr::YN; break; }
		case 3: { cur = spltr::S_FN; break; }
		case 4: { cur = spltr::DONE; break; }
		case 5: { cur = spltr::O_FN; break; }
	}
}
void Splitter::previousState()
{
	//O_FN, DELIMITER, REPLACE, YN, S_FN, DONE
	switch (cur)
	{
		case 2: { cur = spltr::DELIMITER; break; }
		case 3: { cur = spltr::REPLACE; break; }
		case 4: { cur = spltr::YN; break; }
		case 5: { cur = spltr::S_FN; break; }
		case 0: { cur = spltr::DONE; break; }
		case 1: { cur = spltr::O_FN; break; }
	}
}
void Splitter::clearData()
{
	cur = spltr::O_FN;
	data.ofn = "";
	data.delim = NULL;
	data.rplwth = NULL;
	data.sfn = "NOSTRING";
}
void Splitter::placeData(std::string u_data)
{
	switch (cur)
	{
		case spltr::O_FN :
		{
			data.ofn = u_data;
			break;
		}
		case spltr::DELIMITER :
		{
			if (u_data == "\\n") {
				data.delim = '\n';
			}
			else {
				data.delim = *u_data.c_str();
			}
			break;
		}
		case spltr::REPLACE :
		{
			if (u_data == "\\n") {
				data.rplwth = '\n';
			}
			else {
				data.rplwth = *u_data.c_str();
			}
			break;
		}
		case spltr::YN :
		{
			if (u_data != "y") {
				std::cout << "YN answer: " << u_data << std::endl;
				cur = spltr::S_FN;
			}
			break;
		}
		case spltr::S_FN :
		{
			data.sfn = u_data;
			break;
		}
	}
}
spltr::QuesState Splitter::getState()
{
	return cur;
}
bool Splitter::split(spltr::Data inp)
{
	curMode.ovrd_color("GREEN", "**********************");

	std::string line;
	std::string finalLine = "";
	std::string newline;
	std::ifstream rfile(stripSpaces(filepath) + stripSpaces(inp.ofn));
	std::ofstream sfile;

	if (inp.sfn != "NOSTRING") {
		sfile.open(stripSpaces(filepath) + stripSpaces(inp.sfn));
	}

	if (rfile.is_open())
	{
		while (getline(rfile, line))
		{
			if (inp.delim != '\n')
			{
				for (int i = 0; i < line.length() + 1; i++)
				{
					if (line[i] == inp.delim) {
						line[i] = inp.rplwth;
					}
				}
			}
			else{
				if (!line.empty()) {
					line.back() = inp.rplwth;
				}
				else {
					line = inp.rplwth;
				}
			}

			finalLine += line;
		}

		if (sfile.is_open())
		{
			sfile << finalLine;
			sfile.close();
		}

		rfile.close();

		curMode.ovrd_color("YELLOW", insertLinenums(finalLine));

		curMode.ovrd_color("GREEN", "**********************");

		return true;
	}
	else {
		curMode.ovrd_color("RED", "Could not open read file");
	}

	curMode.ovrd_color("GREEN", "**********************");

	return false;
}
std::string Splitter::insertLinenums(std::string to_print)
{
	int linenum = 1;
	std::string oldPrint = to_print;
	int j = 0;
	std::string sln = "";

	sln = std::to_string(linenum) + ".\t";
	to_print.insert(j, sln);
	j += sln.length();

	for (int i = 0; i < oldPrint.length(); i++)
	{
		if (oldPrint[i] == '\n')
		{
			linenum++;
			sln = std::to_string(linenum) + ".\t";

			to_print.insert(j + 1, sln);

			j += sln.length();
		}
		j++;
	}

	return to_print;
}
void Splitter::displayFront()
{
	curMode.ovrd_color("PURPLE", "\n\t::Line Splitter::");
	
	curMode.setColor("YELLOW");
	std::cout << "For a list of commands, type "; curMode.ovrd_color("GREEN", "/command", false); std::cout << "." << std::endl;
	std::cout << "For a description of this tool, type "; curMode.ovrd_color("GREEN", "/about", false); std::cout << ". \n" << std::endl;
	curMode.resetColor();
}
void Splitter::displayAbout()
{
	curMode.ovrd_color("GREEN", "---------------------------------------------");
	curMode.ovrd_color("PURPLE", "About:");
	curMode.setColor("YELLOW");
	std::cout <<
		"The Line Splitter Tool is used for EDI documents that need to be broken up into " <<
		"lines for better readability. \n" <<
		"To use the splitter, enter the name of the file you want to split, what \n" <<
		"character you want to split it on (the 'delimiter') and what to replace the \n" <<
		"delimiter with. If you wish to save the file, enter 'yes' and enter a name for\n" <<
		"the new file. If you don't save the file, the program will only display the end\n" <<
		"result.  \n" <<
		"Currently, escape sequences are not supported as a delimiter or a character to\n" <<
		"replace with BESIDES the newline character '\\n' \n" <<
	std::endl;
	curMode.resetColor();
	curMode.ovrd_color("GREEN", "---------------------------------------------");
}