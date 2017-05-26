#include "question.h"

Question::Question()
{
    questions[0] = "Enter [File name]: ";
    questions[1] = "Enter [Delimiter]: ";
    questions[2] = "[Replace] delimiter with: ";
    questions[3] = "Save to a new file? ";
    questions[4] = "Enter a file name to [save] to: ";
	filepath = "NOPATH";
}
Question::Question(std::string filePath)
{
	questions[0] = "Enter [File name]: ";
	questions[1] = "Enter [Delimiter]: ";
	questions[2] = "[Replace] delimiter with: ";
	questions[3] = "Save to a new file? ";
	questions[4] = "Enter a file name to [save] to: ";

	filepath = filePath;
}
std::string Question::pollQuestion(spltr::QuesState cur)
{
	std::string new_answer = "";

	curMode.resetColor();
	std::cout << "(Home)>(Splitter) " + questions[cur];

	curMode.setColor("CYAN");
	getline(std::cin, new_answer);
	curMode.resetColor();

	return new_answer;
}
bool Question::validateData(spltr::QuesState cur, std::string& u_data)
{
    if(cur == spltr::O_FN || cur == spltr::S_FN)
    {
        if(u_data == "")
        {
            curMode.ovrd_color("RED", "No string for file name.");
        }
        else
        {
            if(u_data.length() > 4)
            {
                if(u_data.substr(u_data.length() - 4) != ".txt"){
                    u_data += ".txt";
                }
            }
            else{   u_data += ".txt";}

//			std::cout << u_data << std::endl;

            if(cur == spltr::O_FN)
            {
                std::ifstream try_file(stripSpaces(filepath) + stripSpaces(u_data));

                if(try_file.is_open())
                {
                    try_file.close();
                    return true;
                }
                else{
                    curMode.ovrd_color("RED", "Filename [" + stripSpaces(filepath) + "][" + stripSpaces(u_data) + "]" + " failed.");
                }
            }
            else{
                return true;
            }
        }
    }
    else if(cur == spltr::YN)
    {
		u_data = makelower(u_data);

        if(u_data == "y" || u_data == "yes")
        {
            u_data = "y";
            return true;
        }

        if(u_data == "n" || u_data == "no")
        {
            u_data = "n";
            return true;
        }
        curMode.ovrd_color("RED", "Invalid answer.");
    }
    else if(cur == spltr::DELIMITER || cur == spltr::REPLACE)
    {
        if(u_data.substr(0,1) == "\\")
        {
            if(u_data.length() == 2){
                return true;
            }
        }
        else
        {
            if(u_data.length() == 1){
                return true;
            }
        }

		curMode.ovrd_color("RED", "Invalid character.");
    }

    return false;
}
void Question::setFilePath(std::string n_path)
{
	filepath = n_path;
}