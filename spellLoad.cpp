//#include <id3/tag.h>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include <string>
#include <sstream>
#include "MetaPhone.h"
#include <stdio.h>
#include <vector>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

using namespace std;
#define REMOVE 1
#define UPDATE 2

#define USE_MYSQL 1
#include "utils.h"
#include "dbase.h"
#include "Query.h"
#include "result_set.h"
//#include "dbase.h"
#include "string_functions.h"
string metadata(string);

void show_locations();
string lowercase(string &st);
int traverse(string);
DB_Handler database;
void add_location();
void update_locations(int action);


string
metadata(string name)
{
	for (int i =0; i<name.length(); i++)
		if (name[i]=='.')
		{
			name[i]=' ';
			name[i+1]=' ';
			name[i+2]=' ';
			name[i+3]=' ';
		}
	string final="";
	char *nm=new char[name.length()+1];
	sprintf(nm,"%s",name.c_str());	
	char *pch;
	pch = strtok(nm," ");
	bool oknow=false;
	while(pch!=NULL)
	{
		
//		
		if (pch[0]=='-')
			oknow=true;
//		if (strcmp(pch,"mp3")!=0)

		if (oknow)
		{
			//cout << pch << endl;
			CString one,two;
			MString ss(pch);
			ss.DoubleMetaphone(one,two);
			final+=one.data+ " ";
		}
		if (pch[0]=='-') oknow=true;
		pch=strtok(NULL, " ");
	}
	delete [] nm;
	return final;
}
int main(int argc, char *argv[])
{	
    string db_user,db_password,db_database,directory="";
    db_user=db_password=db_database;
    cout << "Spell substation" << endl;
    cout << "************************" << endl;
    cout << "Enter database user: ";
    cin >> db_user;
    cout << "Enter database password: ";
    db_password=getpass();
    cout << endl << "Enter database name: ";
    cin >> db_database;
    cout << endl << "Enter location of spell files: ";
    cin >> directory;
    cout << "************************" << endl;
    cout << "updating spell database..." << endl;
	
	if (
        database.create_connection(db_user.c_str(),db_password.c_str(),db_database.c_str())
        )
        {
            Query query;
            query << "delete from words";
            database.execute_query(query.str());
            cout << traverse(directory) << " words added" << endl;;
			
        }   
        else
        { 
	    cout << "************************" << endl;
	    cout << "Unable to connect to local machine, please try again..." << endl;
    	}    

 
//}
return 0;

}
   

string lowercase(string &st)
{
	string tri;
	for (int i =0; i < st.length(); i++)
	{
		if (st[i] >= 65 && st[i] <=90)
			tri+=st[i]+32;
		else
			tri+=st[i];
	}
	return tri;
}

int grabWords( string filename)
{
	ifstream openFile(filename.c_str());
	string word = "";
	
	Query query;
	int wnum = 0;
	while(openFile >> word)
	{
		query.clear();
		query << "insert into words (word,soundex) values ('" + database.escape(word) + "',SOUNDEX('" + database.escape(word) + "'))";
		database.execute_query(query);
		wnum++;
	}
	return wnum;
}


int
traverse(string directory)
{
	DIR *traverses;
	
	traverses = opendir(directory.c_str());
	int total=0;
	if (traverses)
	{
		struct dirent *contents;
		while( (contents=readdir(traverses)) != NULL)
		{
			
			string name = contents->d_name;
			if (name[0]=='.' || name[1]=='.')
				continue;
			if (name.find(".",0) > 2000) //lowercase(name).find(".",0) > 2000)
			{
			
				total += traverse(directory+"/"+name);
			}
			else
			{
				
				string fname = directory+"/"+name;
 				
 				total +=grabWords(fname);
				
			}
//				cout << name << " -- mp3 " << endl;
		}
	}
	closedir(traverses);
	return total;
}




