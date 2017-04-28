#include "search_system.h"

Search_System::Search_System()
{
	//ctor
	file_loaded = false;
	library_loaded = false;
}
void Search_System::loadFiles(Input_Package in_pack)
{
	if (in_pack.filetype != "NOFILETYPE") 
	{
		lib_data.clear();
		lib_data = f_loader.loadLibrary(in_pack);
		lib_data_size = lib_data.size();
		library_loaded = true;
	}
	if (in_pack.filename != "NOFILENAME") 
	{
		file_data.clear();
		file_data = f_loader.loadFile(in_pack);
		file_data_size = file_data.size();
		file_loaded = true;
	}

	to_return.clear();

}
void Search_System::clear(std::string to_clear)
{
	if (to_clear == "library") {
		lib_data.clear();
		library_loaded = false;
	}
	else if (to_clear == "file") {
		file_data.clear();
		file_loaded = false;
	}
	else if (to_clear == "all") {
		lib_data.clear();
		library_loaded = false;
		file_data.clear();
		file_loaded = false;
	}
}
bool Search_System::filesLoaded()
{
	return library_loaded || file_loaded;
}
void Search_System::findQuery(Input_Package in_pack)
{
	if (in_pack.q_count == 0) 
	{
		curColor.ovrd_color("RED", "No query given. Retry Find command");
		return;
	}
				//Check the FILE data for the query
	if (!checkValue(in_pack, file_data, file_data_size, "FILE"))
	{
		//std::cout << "Couldn't find as value - checking library." << std::endl;
					//Check the LIBRARY data for the query
		if (!checkValue(in_pack, lib_data, lib_data_size, "LIBRARY"))
		{
			curColor.ovrd_color("RED", "Could not find query as value or in library.");
			return;
		}
		else
		{
			//std::cout << "Found in library, finding as value." << std::endl;
			//Find the FILE data given the library data that was found
			fillFileData();
		}
	}
	else
	{
		//std::cout << "Found as value, finding in library." << std::endl;
		//Find the LIBRARY data given the file data that was found
		fillLibraryData();
	}

	//std::cout << "Storing File Line." << std::endl;
	storeFileLine();

	//std::cout << "Storing Library Definition." << std::endl;
	storeLibraryDefinition();

	displayFound(in_pack);

	curColor.ovrd_color("YELLOW", "Done Searching.");
	curColor.ovrd_color("GREEN", "*********************************");
	to_return.clear();
}
bool Search_System::checkValue(Input_Package in, std::vector<Doc_Value>& data, int data_size, std::string type)
{
	Data_Type seg_typ;
	Data_Type ele_typ;

	if (type == "LIBRARY")
	{
		seg_typ = LIBRARY_SEGMENT;
		ele_typ = LIBRARY_ELEMENT;
	}
	else if (type == "FILE")
	{
		seg_typ = FILE_SEGMENT;
		ele_typ = FILE_ELEMENT;
	}

	//std::cout << "Segment type: " << seg_typ << " Element type: " << ele_typ << std::endl;

	int num_to_display = in.settings.display_num;
	bool found = false;

	//AMOUNT OF QUERIES TO SEARCH FOR (either 1 or 2)
	//indexed as 1 = 0, 2 = 1

	//AMOUNT OF FILE SEGMENTS
	int i = 0;
	while (i < data_size)
	{
		Found_Value new_val;

		int j = 0;
		//std::cout << in.m_query[0] << " == " << data[i].segment << std::endl;
		//FOUND SEGMENT
		if (in.m_query[0] == data[i].segment)
		{
			if (in.q_count == 1)
			{
				found = storeValues(new_val, seg_typ, data[i], i, -1);

				//std::cout << "1 query: Breaking" << std::endl;
				j = data[i].ele_count;
			}
			else {
				new_val.type = seg_typ;
				//std::cout << "Found segment: " << in.m_query[0] << " = " << data[i].segment << std::endl;
			}
		}
		//AMOUNT OF ELEMENTS IN SEGMENT
		while (j < data[i].ele_count)
		{
			//std::cout << in.m_query[0] << " == " << data[i].elements[j] << std::endl;
			if (new_val.type == seg_typ && in.q_count > 1)
			{
				//CHECK NEXT QUERY
				if (in.m_query[1] == data[i].elements[j])
				{
					//chained query element found
					found = storeValues(new_val, ele_typ, data[i], i, j);
					break;
				}
			}
			else if (in.m_query[0] == data[i].elements[j])
			{
				//FOUND ELEMENT
				found = storeValues(new_val, ele_typ, data[i], i, j);
				break;
			}
			j++;
		}

		if (in.q_count > 1 && new_val.found_element == "") {
			curColor.ovrd_color("RED", "Could not find " + in.m_query[0] + " > " + in.m_query[1]);
		}

		i++;

		if (found)
		{
			//std::cout << "Segment or element was found, storing" << std::endl;

			to_return.push_back(new_val);

			found = false;

			num_to_display--;
		}

		if (num_to_display == 0 || i >= data_size) 
		{
			if (to_return.size() > 0) 
			{
				found = true;
				//std::cout << "No more to display, breaking" << std::endl;
				break;
			}
		}
	}//end while i < data size

	return found;
}
bool Search_System::storeValues(Found_Value& n_val, Data_Type in_type, Doc_Value f_val, int seg_indx, int ele_indx)
{
	std::string printType = "";
	n_val.type = in_type;

	if (n_val.type == FILE_SEGMENT || n_val.type == FILE_ELEMENT)
	{
		n_val.found_seg = f_val.segment;
		n_val.file_indx.seg_index = seg_indx;

		if (ele_indx != -1) 
		{
			n_val.found_element = f_val.elements[ele_indx];
			n_val.file_indx.el_index = ele_indx;
		}

		printType = "FILE";

		//std::cout << "Type: " << printType << " Stored (" << seg_indx << ", " << ele_indx << ")[" << n_val.found_seg << ", " << n_val.found_element << "]" << std::endl;
		return true;
	}
	else if (n_val.type == LIBRARY_SEGMENT || n_val.type == LIBRARY_ELEMENT)
	{
		n_val.found_lib_seg	= f_val.segment;
		n_val.lib_indx.seg_index = seg_indx;

		if (ele_indx != -1) 
		{
			n_val.found_lib_element = f_val.elements[ele_indx];
			n_val.lib_indx.el_index = ele_indx;
		}

		n_val.found_seg = f_val.segment_ref;
		printType = "LIBRARY";

		//std::cout << "Type: " << printType << " Stored (" << seg_indx + 1 << ", " << ele_indx + 1 << ") [" << f_val.segment_ref << ", " << n_val.found_lib_seg << ", " << n_val.found_lib_element << "]" << std::endl;
		return true;
	}
	else 
	{
		n_val.file_seg_element_count = 0;
		n_val.found_seg = "";
		n_val.found_element = "";
		n_val.found_lib_seg = "";
		n_val.found_lib_element = "";
		n_val.lib_indx.seg_index = -1;
		n_val.lib_indx.el_index = -1;
		n_val.file_indx.seg_index = -1;
		n_val.file_indx.el_index = -1;
		n_val.full_line = "";
		n_val.library_definition = "";
		printType = "NONE";

		//std::cout << "Type: " << printType << " reset." << std::endl;
		return false;
	}

	return false;
}
void Search_System::fillFileData()
{
	//std::cout << "Filling in the file data" << std::endl;

	int return_size = to_return.size();

	for (int i = 0; i < return_size; i++)
	{
		if (to_return[i].type == NONE) {
			//std::cout << "No return type with return size = " << return_size << std::endl;
			continue;
		}
		for (int j = 0; j < file_data_size; j++)
		{
			//std::cout << "Comparing: " << to_return[i].found_seg << " = " << file_data[j].segment << std::endl;
			if (to_return[i].found_seg == file_data[j].segment){
				storeFileData(i, file_data[j].segment, j);
				break;
			}
		}
		//no segment was found
		if (to_return[i].file_indx.seg_index == -1) {
			//std::cout << "SEG: " << to_return[i].found_seg << " not found in file." << std::endl;
			to_return[i].found_seg = "NO SEG IN FILE";
			continue;
		}
	}
}
void Search_System::storeFileData(int i, std::string found_seg, int seg_indx)
{
	if (to_return[i].lib_indx.el_index != -1)
	{
		int element_index = to_return[i].lib_indx.el_index;
		to_return[i].file_indx.el_index = element_index;
		
		if (file_data[seg_indx].ele_count < element_index + 1) {
			to_return[i].found_element = "NOELEMENT";
		}
		else {
			to_return[i].found_element = file_data[seg_indx].elements[element_index];
		}
	}

	to_return[i].found_seg = found_seg;
	to_return[i].file_indx.seg_index = seg_indx;

	//std::cout << "[FILE DATA] Storing: SEG=" << to_return[i].found_seg << " ELE=" << to_return[i].found_element << " at (" << to_return[i].file_indx.seg_index << ", " << to_return[i].lib_indx.el_index << ")" << std::endl;
}
void Search_System::fillLibraryData()
{
	int return_size = to_return.size();

	for (int i = 0; i < return_size; i++)
	{
		if (to_return[i].type == NONE) {
			//std::cout << "No return type with return size = " << return_size << std::endl;
			continue;
		}
		for (int j = 0; j < lib_data_size; j++)
		{
			//std::cout << "Comparing: " << to_return[i].found_seg << " = " << lib_data[j].segment_ref << std::endl;
			if (to_return[i].found_seg == lib_data[j].segment_ref)
			{
				storeLibraryData(i, lib_data[j].segment, j);
				break;
			}
		}
	}
}
void Search_System::storeLibraryData(int i, std::string found_seg, int seg_indx)
{
	//std::cout << "In library Data, storing: (" << i << ", " << found_seg << ", " << seg_indx << ")" << std::endl;
	if (to_return[i].file_indx.el_index != -1) 
	{
		int element_index = to_return[i].file_indx.el_index;
		to_return[i].lib_indx.el_index = element_index;
		to_return[i].found_lib_element = lib_data[seg_indx].elements[element_index];
	}

	to_return[i].found_lib_seg = found_seg;
	to_return[i].lib_indx.seg_index = seg_indx;

	//std::cout << "[LIB DATA] Stored: SEG=" << to_return[i].found_lib_seg << " ELE=" << to_return[i].found_lib_element << " at (" << to_return[i].lib_indx.seg_index << ", " << to_return[i].lib_indx.el_index << ")" << std::endl;
}
void Search_System::storeFileLine()
{
	int return_size = to_return.size();

	for (int i = 0; i < return_size; i++)
	{
		if (to_return[i].file_indx.seg_index != -1)
		{
			Doc_Value cur_line = file_data[to_return[i].file_indx.seg_index];

			//std::cout << "Storing segment of full line: " << cur_line.segment << std::endl;
			to_return[i].full_line = cur_line.segment;

			std::string elementPiece = "";

			for (int j = 0; j < cur_line.ele_count; j++)
			{
				elementPiece = "*" + cur_line.elements[j];
				to_return[i].full_line += elementPiece;
				to_return[i].file_seg_element_count++;
			}
			//std::cout << "Full line: " << to_return[i].full_line << std::endl;
		}
		else 
		{
			if (to_return[i].found_seg == "NO SEG IN FILE") 
			{
				//std::cout << "FILE DOES NOT CONTAIN SEG:" << to_return[i].found_lib_seg << std::endl;
				to_return[i].full_line == "File does not contain segment";
				to_return[i].found_seg == "File does not contain segment";
			}
			else {
				//std::cout << "Invalid segment index " << to_return[i].file_indx.seg_index << " of returned element number " << i << std::endl;
			}
		}
	}
}
void Search_System::storeLibraryDefinition()
{
	int return_size = to_return.size();

	for (int i = 0; i < return_size; i++)
	{
		if (to_return[i].lib_indx.seg_index != -1)
		{
			Doc_Value cur_line = lib_data[to_return[i].lib_indx.seg_index];

			//std::cout << "Storing segment of Library definition: " << cur_line.segment << std::endl;
			to_return[i].library_definition = cur_line.segment;

			std::string elementPiece = "";

			for (int j = 0; j < cur_line.ele_count; j++)
			{
				elementPiece = "*" + cur_line.elements[j];

				to_return[i].library_definition += elementPiece;
			}
			//std::cout << "Full line: " << to_return[i].library_definition << std::endl;
		}
		else {
			//std::cout << "Invalid segment index " << to_return[i].lib_indx.seg_index << " of returned element number " << i << std::endl;
		}
	}
}
void Search_System::displayFound(Input_Package in)
{
	curColor.setColor("YELLOW");

	std::cout << "Query '" << in.m_query[0];
	if (in.q_count > 1) {
		std::cout << " > " << in.m_query[1];
	}
	std::cout << "' returned " << to_return.size() << " result(s): " << std::endl;
	curColor.ovrd_color("GREEN", "---------------------------------");

	switch (in.settings.display_type)
	{
		case ALL:
		{
			for (int i = 0; i < to_return.size(); i++)
			{
				std::cout << "Line: " << to_return[i].full_line << std::endl;
				std::cout << "Library Definition: " << to_return[i].library_definition << std::endl;
				std::cout << "Library Segment/Element: " << to_return[i].found_lib_seg << ", " << to_return[i].found_lib_element << std::endl;
				std::cout << "In File Segment/Element: " << to_return[i].found_seg << ", " << to_return[i].found_element << std::endl;

				std::cout << "FILE Line " << to_return[i].file_indx.seg_index;
				if (to_return[i].type == FILE_ELEMENT || to_return[i].type == LIBRARY_ELEMENT){
					std::cout << ", Position " << to_return[i].file_indx.el_index + 1;
				}
				std::cout << std::endl;
				curColor.ovrd_color("GREEN", "---------------------------------");
			}
			break;
		}
		case MORE:
		{
			for (int i = 0; i < to_return.size(); i++)
			{
				if (to_return[i].type < 2) {
					std::cout << "Line: " << to_return[i].full_line << std::endl;
				}
				else {
					std::cout << "Library Definition: " << to_return[i].library_definition << std::endl;
				}

				std::cout << "Library Segment/Element: " << to_return[i].found_lib_seg << ", " << to_return[i].found_lib_element << std::endl;
				std::cout << "In File Segment/Element: " << to_return[i].found_seg	   << ", " << to_return[i].found_element	 << std::endl;

				std::cout << "FILE Line " << to_return[i].file_indx.seg_index;
				if (to_return[i].type == FILE_ELEMENT || to_return[i].type == LIBRARY_ELEMENT) {
					std::cout << ", Position " << to_return[i].file_indx.el_index + 1;
				}
				std::cout << std::endl;
				curColor.ovrd_color("GREEN", "---------------------------------");
			}
			break;
		}
		default:
		{
			//Data_Type{ FILE_SEGMENT, FILE_ELEMENT, LIBRARY_SEGMENT, LIBRARY_ELEMENT, NONE}
			for (int i = 0; i < to_return.size(); i++)
			{
				if (to_return[i].type < 2) {
					std::cout << "File Line: " << to_return[i].full_line << std::endl;
				}
				else {
					std::cout << "Library Line: " << to_return[i].library_definition << std::endl;
				}

				std::cout << "Line : (" << to_return[i].file_indx.seg_index + 1;
				if (to_return[i].type == FILE_ELEMENT || to_return[i].type == LIBRARY_ELEMENT) {
					std::cout << "," << to_return[i].file_indx.el_index + 1;
				}
				std::cout << ")" << "\t Element Count: " << to_return[i].file_seg_element_count <<std::endl;
				curColor.ovrd_color("GREEN", "---------------------------------");

			}//end for return size
			break;
		}//end default case
	}//end switch display type

	curColor.resetColor();
}
