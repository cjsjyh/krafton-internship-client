#pragma once
#ifndef _TEXTFILEREADER_H_
#define _TEXTFILEREADER_H_

class textfilereader
{
public:
	textfilereader();
	~textfilereader();
	bool ReadFile(string);

	
	map<string, int> paramInt;
	map<string, float> paramFloat;
	map<string, bool> paramBool;
private:


};

#endif