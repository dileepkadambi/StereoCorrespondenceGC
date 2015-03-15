#include "Utils.h"


std::string ENV_VARIABLE = "PROJECTPATH";

Utils::Utils()
{}

Utils::~Utils()
{}

void Utils::GetFullPathFromEnv(const char* partial_path, char* final_path)
{
  char* env_path, *first_pointer;
  first_pointer = final_path;
  env_path = getenv( ENV_VARIABLE.c_str() );                                                       
  while((*env_path) != NULL)
  {
	  *final_path= *env_path;
	  final_path++;
	  env_path++;
  }
  *final_path = '\0';
  final_path = first_pointer;
  if (final_path != NULL) {                                                                 
    std::strcat(final_path, partial_path);                                                                    
  }       
  
  return;                  
}