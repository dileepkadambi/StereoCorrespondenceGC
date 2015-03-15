#pragma once 

#include <stdio.h>
#include <stdlib.h>
#include <string>

//Class for all utility functions include Utility.h and Cpp in the projects when required. None of the interfaces need to be exported. 
//Add a new Project for Utilities if required and move it there later if Required.
class Utils
{
public:
	Utils();
	~Utils();
	void GetFullPathFromEnv(const char* partial_path, char* final_path);
};