#ifndef RETVALUES_H
#define RETVALUES_H

//#include "vector_list.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "HashTable.h"
#include "hash.h"
//#include "retreive_values.h"

using namespace std;

class retreive_values
{

public:
	retreive_values(){myHash.Set(&stoi,20,"");}
	~retreive_values(){}
	void parse_file(const char *file_location);
	string value(const char *v){ return myHash[v].GetData(); }
	void setValue(const char *v, string s)
	{
		myHash[v]=s;
	}
private:
	void get_variable_value(char *file_line);
    HashTable<string,string> myHash;
};


void 
retreive_values::parse_file(const char *file_location)
{
	ifstream filestream;
	filestream.open(file_location);

	char file_line[500]; // buffer size of 500
	if (filestream && filestream.good())
	{
		// while data exists in file, and stream is present
		while(filestream.good() && !filestream.eof() )
		{
			filestream.getline(file_line,499);
//			if (filestream.eof() || !filestream.good() || filestream.bad() ) break;
			if (file_line)
			{
				// line read, data provided...continue;
				get_variable_value(file_line);
			}
		}
	}
	filestream.close();
}

// CODE ROT HAS MADE THIS FUNCTION INCOMPREHENSIBLE
void
retreive_values::get_variable_value(char *file_line)
{
	if (strlen(file_line) == 0) return;
	char *traverse_line = file_line;
	char *copy_incase="";
	bool in_quote = false;
	for (int i = 0; i<=strlen(file_line)-1; i++)
	{
		if ( *traverse_line == 34 || *traverse_line == 39)
		{
			!in_quote ? in_quote = true : in_quote = false;
		}
		else
		if ( *traverse_line == 47  && *(traverse_line+1) == 47  && !in_quote)	
		{
			if (i == 0) return;
			copy_incase = new char[strlen(file_line)-strlen(traverse_line)];
			strncpy(copy_incase,file_line,strlen(file_line)-strlen(traverse_line));
			copy_incase[strlen(file_line)-strlen(traverse_line)] = '\0';
			break;
		}
		if (*traverse_line == 92)
		{
			switch ( *(traverse_line+1) )
			{
				case 110: *(traverse_line+1)=10; *traverse_line=3; break;
				case 116: *(traverse_line+1)=9; *traverse_line=3; break;
			}
		}
		*traverse_line++;
	}
	if (strlen(copy_incase) > 0)
	{
		traverse_line = file_line = copy_incase;
	}
	else
	{
			traverse_line = file_line;
	}
	char *variable_name="",*variable_value="";
	bool use_quote=false;
	bool use_apostrophe=false;
	while (*traverse_line)
	{

		if ((*traverse_line == 61 || *traverse_line == 32) && strlen(variable_name) == 0)
		{
			variable_name = new char [ strlen(file_line)- strlen(traverse_line) +1];
			strncpy(variable_name,file_line, strlen(file_line)- strlen(traverse_line));
			variable_name[strlen(file_line)-strlen(traverse_line)] = '\0';
			file_line = traverse_line;
			continue;
		}
		if (strlen(variable_name) > 0 && strlen(variable_value) == 0 && (*traverse_line >= 33 && *traverse_line <= 126) && *traverse_line != 61)
		{
			if (*traverse_line == 34)
			{
					use_quote=true;
					traverse_line++;
			}
			else if (*traverse_line == 39)
			{
					use_apostrophe=true;
					traverse_line++;
			}

			file_line = traverse_line;
			register int count=0;
			variable_value = new char [ strlen(file_line) +1];
			while (*traverse_line)
				{

					if (*traverse_line== 34 && use_quote)
					{
						// what is 34???
						break;
					}
					else
					if (*traverse_line== 39 && use_apostrophe)
					{
						// what is 39???
						break;
					}
					else
					if ( (*(traverse_line+1) > 32 && *(traverse_line+1) < 127 ) && *(traverse_line) == 92)
					{
							variable_value[count] = *(traverse_line+1);
							count++;
							traverse_line++;
					}
					else
					if ( (*traverse_line >= 32 && *traverse_line <= 126) && *traverse_line != 34 && *traverse_line != 39 || *traverse_line == 10 || *traverse_line == 9)
					{
						variable_value[count]=*traverse_line;
						count++;
					}
					traverse_line++;
				}
			variable_value[count] = '\0';
			break;
		}
		traverse_line++;
	}
//	add_value(variable_name,variable_value);
        myHash[variable_name]=variable_value;
//		cout << variable_name << " equals " << myHash[variable_name].GetData() << endl;
		// add with debug declaration
		// debug declaration should be in universal file

	delete [] variable_name,variable_value;
	if (strlen(copy_incase) > 0) delete [] copy_incase;
}


#endif

