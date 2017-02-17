#include "stringFuncs.h"

char * getNameOfFile(char * fullPath)
{
	char* place = fullPath;
	char* lastPlace = place;
	while (*place != '\0')
	{
		if (*place == '\\')
		{
			lastPlace = place;
		}
		place++;
	}

	return lastPlace;
}

std::string getPathExceptEnding(char * fullPath)
{
	std::string editedPath(fullPath);
	editedPath = editedPath.substr(0, editedPath.find_last_of('.'));

	return editedPath;
}

bool containsSpecialName(const char * name)
{
	int numSpecialNames = sizeof(reservedNames) / sizeof(const char*);
	for (int i = 0; i < numSpecialNames; i++)
	{
		if (strstr(name, reservedNames[i]) != nullptr)
		{
			return true;
		}
	}

	return false;
}
