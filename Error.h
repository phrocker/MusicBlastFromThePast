#ifndef ERROR_H
#define ERROR_H

enum ERROR_TYPE {DATABASE=0,FLATFILE,SCREEN,TERMINAL};

#include <string>
#include "html.h"
#include "lib.h"
using namespace std;

class Error
{
	public:
		Error(string,string,int,ERROR_TYPE = TERMINAL, string = "");
		Error(string a,string b,int c,string d)
		{
			Error(a,b,c,TERMINAL,d);
			/*
			ErrorName=a;
			ErrorType = b;
			errorNum=c;
			my_type=TERMINAL;
		//	if (query != NULL)
			localQuery = d;
			*/
		}
		void PrintScreen();
        void PrintError();
        void PrintTerminal();
	private:
		
		string ErrorName;
		string ErrorType;
		string localQuery;
		int errorNum;
		ERROR_TYPE my_type;
};

Error::Error(string type,string name,int number, ERROR_TYPE etype, string query)
{
	ErrorName=name;
	ErrorType = type;
	errorNum=number;
	my_type=etype;
//	if (query != NULL)
		localQuery = query;
	//else
		//localQuery="";
	// if type is 0 or 1, maybe we should record it here?
}


void
Error::PrintScreen()
{
	html html_controls;
	string printError="Type: " + ErrorType + "<br>" + ErrorName;
    	html_controls.htmlerror(printError,errorNum);
}


void
Error::PrintTerminal()
{
    cout << "Error: " << ErrorName << ". Type: " << ErrorType << endl;
	cout << "Query:" << localQuery <<endl;
}


void
Error::PrintError()
{
		string printError="Type: " + ErrorType + "<br>" + ErrorName;
	create_log("error %s ..... %s",printError.c_str(),localQuery.c_str());
    switch(my_type)
    {
        case SCREEN: PrintScreen();
            break;
        case TERMINAL: PrintTerminal();
            break;
    }

}



#endif
