#pragma once

#ifndef STRING_FUNCS_H
#define STRING_FUNCS_H

#include <string>

const char* const reservedNames[] =
{
	"CM", "WP", "PL", "BB", "WPEnd", "VM", "Z"
};


char* getNameOfFile(char* fullPath);

std::string getPathExceptEnding(char* fullPath);

bool containsSpecialName(const char* name);



#endif
