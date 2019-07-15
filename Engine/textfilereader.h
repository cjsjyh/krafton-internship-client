#pragma once
#ifndef _TEXTFILEREADER_H_
#define _TEXTFILEREADER_H_

class textfilereader
{
public:
	textfilereader();
	~textfilereader();
	bool ReadFile(char*, int per_line = 2);

	map<string, float> params;
private:


};

#endif