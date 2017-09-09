
#include "./Headers/String.h"

// removes leading and trailing whitepace
String String::trim()
{
	try
	{
		int begin=0, end=0,i=0;
		for (i=0; i < internalString.size(); i++)
		{
			if (internalString.at(i) != SPACE )
			{
				begin=i;
				break;
			}
		}
		for (i=internalString.size()-1; i >= 0; i--)
		{
			if (internalString.at(i) != SPACE )
			{
				end=i+1;
				break;
			}
		}
		if (end > begin)
		{
			return substring(begin,end);
		}
		else
			throw new StringException();
	}
	catch(...)
	{
		return "";
	}
}


String &String::operator =(string &st)
{
	this->internalString = st ;
	return *this;
}

String &String::operator =(char *st)
{
	this->internalString = st;
	return *this;
}

ostream 
&operator<< ( ostream &output, String &Data )
{
	output << Data.toString() ;
	return output;
}

String
String::substring(int begin, int offset)
{
	try
	{
		return this->internalString.substr(begin,offset-begin);
	}catch(...)
	{
		return "";
	}
}

char *
String::toCharArray()
{
	char *returnArray = new char[ internalString.size() ];
	for (int i=0; i < internalString.size(); i++)
		returnArray[i]=internalString.at(i);
	return returnArray;
}

vector< String >
String::split(string regex)
{
	char *temptokenizer;
	temptokenizer = strtok( (char*)(internalString.c_str()), (char*)(regex.c_str()) );
	bool started=false;
	int location=0;
	/*
		tokenize the string to split it by the regualar expression above
		this doesn't actually evaluate your regular expression, it
		simply tokenizes by splitting by the value in regex
	*/
	vector< String > strings;
	
	
	while( temptokenizer != NULL )
	{
		String t = temptokenizer;
		//location+=t.length ();
		strings.push_back( t );

		temptokenizer = strtok( NULL, (char*)(regex.c_str()) );
	}
	//cout << "internal size " << strings.size() << endl;
	return strings;
}
