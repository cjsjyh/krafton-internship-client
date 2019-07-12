#include "textfilereader.h"

textfilereader::textfilereader()
{

}

textfilereader::~textfilereader()
{

}

bool textfilereader::ReadFile(char* fname, int per_line)
{
	FILE* fp;
	char line[200];
	char parameter_name[50];
	float parameter_value;

	cout << "In file reader" << endl;
	fp = fopen(fname, "r");

	if (fp == NULL)
		return false;

	while (fgets(line, sizeof(line), fp) != NULL)
	{
		int count = sscanf(line, "%s %f", parameter_name, &parameter_value);
		if (count != 2)
		{
			cout << "Error! 2 params not inserted properly" << endl;
			return false;
		}
		params.insert(make_pair(string(parameter_name,strlen(parameter_name)), parameter_value));
	}

	for (auto it = params.begin(); it != params.end(); it++) {
		cout << "key : " << it->first << " " << "value : " << it->second << '\n';
	}

	return true;
}