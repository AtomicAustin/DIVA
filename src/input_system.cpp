#include "input_system.h"

//0 - good
//1 - bad command
//2 - bad file name
//3 - bad setting name
//4 - bad query
//5 - unknown error

Input_System::Input_System()
{
	//ctor
	err_code = 0;
}
int Input_System::parseString(Finder_Input& inp)
{
	err_code = 0;
	err_str = "";

	std::string full_str = inp.getfullStr();

	if (isCommand(full_str))
	{
		full_str = stripSpaces(full_str);
		getSetting(full_str, inp);
		full_str = stripSpaces(full_str);

		if (err_code == 0)
		{
			switch (cur_state)
			{
			case fndr::FIND: {getQuery(full_str, inp); break;}
			case fndr::LOAD: {getFilename(full_str, inp); break;}
			}
		}
	}

	return err_code;
}
bool Input_System::isCommand(std::string &in)
{
	int last = in.find(" ", 0);
	std::string n = makelower(in.substr(0, last));

	if (n == "find")
	{
		in.erase(0, last + 1);
		cur_state = fndr::FIND;
		//std::cout << "Find command found. String left: [" << in << "]" << std::endl;
		return true;
	}
	else if (n == "load")
	{
		in.erase(0, last + 1);
		cur_state = fndr::LOAD;
		//std::cout << "Load command found. String left: [" << in << "]" << std::endl;
		return true;
	}
	else 
	{
		err_code = 1;
		in.erase(0, last + 1);
		//std::cout << "[ " << n << "] is not a valid command." << std::endl;
	}
	return false;
}
void Input_System::getSetting(std::string& in, Finder_Input& inp)
{
	//std::cout << "in = [" << in << "]" << std::endl;

	int first = in.find("-", 0);

	if (first == std::string::npos) {
		return;
	}

	while (true) 
	{
		if (first != 0)
		{
			if (first == std::string::npos)
			{
				err_str = in + " Does not contain a valid setting";
				//curColor.ovrd_color("RED", in + " Does not contain a valid setting");
				return;
			}
			if (in[first - 1] != ' ')
			{
				first = in.find("-", first + 1);

				if (first == std::string::npos) 
				{
					err_str = in + " Does not contain a valid setting";
					//curColor.ovrd_color("RED", in + " Does not contain a valid setting");
					return;
				}
			}
			else {
				break;
			}
		}
		else {
			break;
		}
	}

	if (first != std::string::npos)
	{
		//std::cout << "First '-' at : " << first << std::endl;

		int last = in.find(" ", first);

		if (last == std::string::npos) {
			last = in.length() - 1;
		}
		//std::cout << "Last ' ' at : " << last << std::endl;

		std::string setting = in.substr(first + 1, last - first);

		//std::cout << "Setting found [" << setting << "]" << std::endl;

		//std::cout << "Erasing [" << in.substr(first, last - first + 1) << "]" << std::endl;
		in.erase(first, last - first + 1);

		if (!validSetting(makelower(stripSpaces(setting)), inp))
		{
			err_code = 3;
			err_str = "Unrecognized setting: [" + setting + "]";
			//curColor.ovrd_color("RED", "Unrecognized setting: [" + setting + "]");
		}

		//if (cur_state == fndr::FIND) {
			getSetting(in, inp);
		//}
	}
	else {
		curColor.ovrd_color("YELLOW", "No settings found.");
	}
}
bool Input_System::validSetting(std::string& in, Finder_Input& inp)
{
	switch (cur_state)
	{
		case fndr::FIND:
		{
			for (int i = 0; i < d_setting_size; i++)
			{
				if (in == disp_setting[i]) 
				{
					if (in.substr(0, 1) == "a") 
					{
						inp.storeSetting(fndr::ALL);
						in = "a";
					}
					else if (in.substr(0, 1) == "m") 
					{
						inp.storeSetting(fndr::MORE);
						in = "m";
					}
					else if (in.substr(0, 1) == "s") 
					{
						inp.storeSetting(fndr::SIMPLE);
						in = "s";
					}
					return true;
				}
			}
			if (isInt(in)) 
			{
				int num = std::stoi(in);
				
				if (num < 0) {
					inp.storeSetting(num);
				}
				else 
				{
					err_str = num + " must be greater than zero";
					//curColor.ovrd_color("RED", num + " must be greater than zero");
					break;
				}

				return true;
			}

			err_str = "Invalid find setting [" + in + "]";
			//curColor.ovrd_color("RED", "Invalid find setting [" + in + "]");
			break;
		}
		case fndr::LOAD:
		{
			if (isInt(in))
			{
				for (int i = 0; i < l_setting_size; i++)
				{
					if (in == load_setting[i]){
						inp.storeFileType(in);
						return true;
					}
				}
				err_str = "Invalid load setting [" + in + "]";
			}
			else
			{
				for (int i = 0; i < s_char_size; i++)
				{
					if (in == spec_char[i]) {
						inp.storeDelimiter(in);
						return true;
					}
				}
				err_str = "Invalid delimiter [" + in + "]";
			}

			break;
		}
	}

	return false;
}
void Input_System::getQuery(std::string &in, Finder_Input& inp)
{
	inp.setPackCount(0);
	std::string to_store = in;

	while (true)
	{
		int first = in.find(">", 0);

		if (first != std::string::npos)
		{
			to_store = stripSpaces(to_store.substr(0, first));
			in.erase(0, first + 1);

			//std::cout << "Storing [" << to_store << "]" << std::endl;
			//std::cout << "String left: " << in << std::endl;

			if (to_store != "*" || to_store != ">") 
			{
				inp.storeQuery(to_store);
			}
			else 
			{
				err_code = 4;
				err_str = to_store + " is an invalid character, please use a different query.";
				//curColor.ovrd_color("RED", to_store + " is an invalid character, please use a different query.");
			}
		}
		else 
		{
			to_store = in;
			//std::cout << "Storing [" << to_store << "]" << std::endl;
			to_store = stripSpaces(to_store);
			break;
		}
	}

	if (to_store != "*" || to_store != ">") 
	{
		inp.storeQuery(to_store);
	}
	else 
	{
		err_code = 4;
		err_str = to_store + " is an invalid character, please use a different query.";
		//curColor.ovrd_color("RED", to_store + " is an invalid character, please use a different query.");
	}
}
void Input_System::getFilename(std::string &in, Finder_Input& inp)
{
	//std::cout << "File name to store: [" << in << "]" << std::endl;

	if (in.length() > 4)
	{
		if (in.substr(in.length() - 4, 4) != ".txt") {
			in += ".txt";
		}
	}
	else {
		in += ".txt";
	}

	//in = inp.getPackage().filepath + in;

	std::ifstream tryfile(stripSpaces(inp.getPackage().filepath) + stripSpaces(in));

	if (tryfile.is_open())
	{
		inp.storeFileName(in);
		tryfile.close();
	}
	else
	{
		err_code = 2;
		err_str = "Could not open file with name: [" + inp.getPackage().filepath + "][" + in + "]";
		//curColor.ovrd_color("RED", "Could not open file with name: [" + in + "]");
	}
}
std::string Input_System::getError()
{
	return err_str;
}
std::string Input_System::stripSpaces(std::string in)
{
	if (in == "") {
		return "";
	}
	//std::cout << "Before strip [" << in << "]" << std::endl;
	//erase all spaces before and after "in"
	int first;
	for (int i = 0; i < in.length(); i++) {
		if (in[i] != ' ') 
		{
			first = i;
			break;
		}
	}
	
	if (first != 0) {
		in.erase(0, first);
	}

	for (int i = in.length() - 1; i >= 0; i--)
	{
		if (in[i] != ' ') 
		{
			first = i + 1;
			break;
		}
	}

	if (first != in.length()) {
		in.erase(first, in.length() - first);
	}

	//std::cout << "After strip [" << in << "]" << std::endl;

	return in;
}
bool Input_System::isInt(std::string &in)
{
	if (in.empty() || ((!isdigit(in[0])) && (in[0] != '+'))) {
		return false;
	}

	char * p;
	strtol(in.c_str(), &p, 10);

	return (*p == 0);
}
fndr::INPUT_STATE Input_System::getState()
{
	return cur_state;
}
Splitter_Input Input_System::breakup(std::string b_in)
{
	Splitter_Input s_inp;

	while (true)
	{
		size_t pos = b_in.find(" ");

		if (pos != std::string::npos)
		{
			if (b_in != " ") {
				s_inp.store(b_in.substr(0, pos));
			}

			b_in.erase(0, pos + 1);
		}
		else if (pos == std::string::npos && b_in != "")
		{
			s_inp.store(b_in);
			break;
		}
		else {
			break;
		}
	}

	return s_inp;
}