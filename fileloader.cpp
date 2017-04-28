#include "fileloader.h"

FileLoader::FileLoader()
{
	//ctor
	_850library = "files/850LIBRARY.txt";
	_810library = "files/810LIBRARY.txt";
	ufile = "NOSTRING";
}
std::vector<Doc_Value> FileLoader::loadFile(Input_Package in)
{
	curColor.ovrd_color("YELLOW", "Loading File...");

	std::vector<Doc_Value>   file_data;

	std::string fileName = in.filename;
	std::string line = "";
	std::string SEG  = "";
	int			SEG_POS = 0;
	int         lineNum = 0;

	std::ifstream rfile(fileName.c_str());

	if (rfile.is_open())
	{
		while (getline(rfile, line))
		{
			Doc_Value   newVal;

			lineNum++;
			//line = removeTabs(line);

			SEG_POS = line.find("*", 0);

			//std::cout << "SEGMENT: [" << line.substr(0, SEG_POS) << "]" << std::endl;
			SEG = line.substr(0, SEG_POS);

			newVal.segment = SEG;
			newVal.seg_line_num = lineNum;

			newVal = storeElements(line, newVal);
			file_data.push_back(newVal);

		}

		rfile.close();
	}
	else {
		curColor.ovrd_color("RED", "Could not open file [" + fileName + "]");
	}

	return file_data;
}
std::vector<Doc_Value> FileLoader::loadLibrary(Input_Package in)
{
	curColor.ovrd_color("YELLOW", "Loading Library...");

	std::vector<Doc_Value> lib_data;

	std::string fileName = "";
	std::string fileType = in.filetype;

	if (fileType == "810") {
		fileName = _810library;
	}
	else if (fileType == "850") {
		fileName = _850library;
	}
	else 
	{
		curColor.ovrd_color("RED", "Bad File Type [" + fileType + "]");
		return lib_data;
	}

	std::string line = "";
	std::string SEG_REF = "";
	int         SEG_REF_POS = 0;
	std::string SEG = "";
	int         SEG_POS = 0;
	int         lineNum = 0;

	std::ifstream rfile(fileName.c_str());

	if (rfile.is_open())
	{
		while (getline(rfile, line))
		{
			Doc_Value   newVal;

			lineNum++;
			//line = removeTabs(line);

			SEG_REF_POS = line.find("*");

			//std::cout << "SEG REF: [" << line.substr(0, SEG_REF_POS) << "]" << std::endl;

			SEG_REF = line.substr(0, SEG_REF_POS);
			line.erase(0, SEG_REF_POS + 1);

			SEG_POS = line.find("*");

			//std::cout << "SEGMENT: [" << line.substr(0, SEG_POS) << "]" << std::endl;
			SEG = line.substr(0, SEG_POS);

			newVal.segment_ref = SEG_REF;
			newVal.segment = SEG;
			newVal.seg_line_num = lineNum;

			newVal = storeElements(line, newVal);
			lib_data.push_back(newVal);
		}
		rfile.close();
	}

	return lib_data;
}
Doc_Value FileLoader::storeElements(std::string& n, Doc_Value dv)
{
	int first = n.find("*");
	int last = 0;
	dv.ele_count = 0;
	bool run = true;

	while (run)
	{
		//first = n.find("*", first);
		last = n.find("*", first + 1);

		if (last == std::string::npos) 
		{
			last = n.length();

			if (n.substr(first + 1, last - first - 1) == ">") {
				break;
			}
			run = false;
		}

		//std::cout << "first/last: [" << first << ", " << last << "]" << std::endl;

		if (first == last - 1) {
			//std::cout << "Push back element: [NOELEMENT]" << std::endl;
			dv.elements.push_back("NOELEMENT");
		}
		else {
			//std::cout << "Push back element: [" << n.substr(first + 1, last - first - 1) << "]" << std::endl;
			dv.elements.push_back(n.substr(first + 1, last - first - 1));
		}

		dv.ele_count++;

		first = last;
	}

	return dv;
}
std::string FileLoader::removeTabs(std::string& n)
{
	while (true)
	{
		int tab_pos = n.find("\t", 0);

		if (tab_pos == std::string::npos) {
			break;
		}

		n.erase(tab_pos, 1);
	}
	return n;
}
