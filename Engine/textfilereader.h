#pragma once
#ifndef _TEXTFILEREADER_H_
#define _TEXTFILEREADER_H_

class textfilereader
{
public:
	typedef struct UIinfo {
		string uiname;
		string filename;
		int pos_x;
		int pos_y;
		int size_x;
		int size_y;
	}UIinfo;

	textfilereader();
	~textfilereader();
	bool ReadFile(string);
	bool ReadUIFile(string);
	bool ReadItemFile(string);

	
	map<string, int> paramInt;
	map<string, float> paramFloat;
	map<string, bool> paramBool;

	vector<UIinfo> paramUI;

	map<string, int> ItemInt;
	map<string, float> ItemFloat;
	map<string, bool> ItemBool;
	vector<string> ItemNames;
private:


};

#endif