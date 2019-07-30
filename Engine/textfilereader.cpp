#include "stdafx.h"

#include "UIinfo.h"

#include "textfilereader.h"

textfilereader::textfilereader()
{
	
}

textfilereader::~textfilereader()
{

}

bool textfilereader::ReadFile(string fname)
{
	fstream in(fname);
	string line;
	int pos;
	if (!in.is_open())
	{
		std::cout << "Failed to open file" << endl;
		return false;
	}

	while (getline(in, line))
	{
		vector<string> fields;
		if (line == "")
			break;
		
		while ((pos = line.find(',')) >= 0)
		{
			fields.push_back(line.substr(0, pos));
			line = line.substr(pos + 1);
			
		}

		if (fields.size() < 3)
		{
			cout << "[WARNING] INCORRECT DATASHEET FORMAT!" << endl;
			return false;
		}

		if (fields[2] == "int")
			paramInt.insert(make_pair(fields[0], stoi(fields[1])));
		else if (fields[2] == "float")
			paramFloat.insert(make_pair(fields[0], stof(fields[1])));
		else if (fields[2] == "bool")
		{
			if (fields[2] == "true")
				paramBool.insert(make_pair(fields[0], true));
			else
				paramBool.insert(make_pair(fields[0], false));
		}
		else
			return false;
	}
	return true;
}

bool textfilereader::ReadUIFile(string fname)
{
	fstream in(fname);
	string line;
	int pos;

	if (!in.is_open())
	{
		std::cout << "Failed to open file" << endl;
		return false;
	}

	while (getline(in, line))
	{
		UIinfo* temp = new UIinfo;
		for (int i = 0; i < 6; i++)
		{
			if (i != 0)
				getline(in, line);
			vector<string> fields;
			if (line == "")
				break;

			while ((pos = line.find(',')) >= 0)
			{
				fields.push_back(line.substr(0, pos));
				line = line.substr(pos + 1);
			}
			fields.push_back(line);

			
			switch (i)
			{
			case 0:
				temp->uiname = fields[1];
				break;
			case 1:
				temp->filename = "./data/UI/" + fields[1];
				break;
			case 2:
				temp->pos_x = stoi(fields[1]);
				break;
			case 3:
				temp->pos_y = stoi(fields[1]);
				break;
			case 4:
				temp->size_x = stoi(fields[1]);
				break;
			case 5:
				temp->size_y = stoi(fields[1]);
				break;
			}
		}
		//temp->pos_x -= temp->size_x / 2;
		//temp->pos_y -= temp->size_y / 2;
		paramUI.push_back(temp);
	}
	return true;
}

bool textfilereader::ReadItemFile(string fname)
{
	fstream in(fname);
	string line;
	int pos;
	if (!in.is_open())
	{
		std::cout << "Failed to open file" << endl;
		return false;
	}

	while (getline(in, line))
	{
		vector<string> fields;
		if (line == "")
			break;
		else if (line[0] == ',')
			continue;

		while ((pos = line.find(',')) >= 0)
		{
			fields.push_back(line.substr(0, pos));
			line = line.substr(pos + 1);

		}

		if (fields.size() < 3)
		{
			cout << "[WARNING] INCORRECT DATASHEET FORMAT!" << endl;
			return false;
		}

		if (fields[0] == "ITEM_NAME")
		{
			ItemNames.push_back(fields[1]);
			continue;
		}

		if (fields[2] == "int")
			ItemInt.insert(make_pair(fields[0], stoi(fields[1])));
		else if (fields[2] == "float")
			ItemFloat.insert(make_pair(fields[0], stof(fields[1])));
		else if (fields[2] == "bool")
		{
			if (fields[2] == "true")
				ItemBool.insert(make_pair(fields[0], true));
			else
				ItemBool.insert(make_pair(fields[0], false));
		}
		else
			return false;
	}
	return true;
}