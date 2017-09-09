//#include <id3/tag.h>
#include <iostream>
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

void reload_words();
string lowercase(string &st);
void traverse(string);
DB_Handler database;
int total;

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
    string db_user,db_password,db_database;
    db_user=db_password=db_database;
    cout << "Spelling substation" << endl;
    cout << "************************" << endl;
    cout << "Enter database user: ";
    cin >> db_user;
    cout << "Enter database password: ";
    db_password=getpass();
    cout << endl << "Enter database name: ";
    cin >> db_database;
    cout << "************************" << endl;
    cout << "Database will now be analyzed..." << endl;
	total=0;
	if (
        database.create_connection(db_user.c_str(),db_password.c_str(),db_database.c_str())
        )
        {
                int choice=0;
                do
                {
                    system("clear");
                    cout << "Enter menu option" << endl;
                    cout << " 1) Reload words into database" << endl;
                    cout << " 2) Exit" << endl;
                    cin >> choice;
                    switch(choice)
                    {
                        case 1: 
                            reload_words();
                            break;
                        default: break;                                                      
                    };    
                }while(choice != 2); 
		
        }   
        else
        { 
	    cout << "************************" << endl;
	    cout << "Unable to connect to local machine, please try again..." << endl;
    	}    

 
//}
return 0;

}

void
reload_words()
{
    string path="";
    cout << "Enter full path to directory you wish to extrapolate words" << endl;
    cin >> path;
    traverse(path);
   	cout << total << " Total words added" << endl;
   	       	total=0;
   	cout << "Press any key to continue";
    getchar();
    getchar();

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

void
traverse(string directory)
{
	DIR *traverses;
	traverses = opendir(directory.c_str());
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
			
				traverse(directory+"/"+name);
			}
			else
			{
 				string fname = directory+"/"+name;
				
				ifstream file_reader;
				file_reader.open(fname.c_str());
				string word;
				Query query;
				while(file_reader>>word)
				{
					total++;
					CString one,two;
					MString ss(word.c_str());
					ss.DoubleMetaphone(one,two);
					query.clear();
					query << "insert into words (word,metaphone) values('" << database.escape(word) << "','" << database.escape(one.data) << "')";
					database.execute_query(query);					
				}
				file_reader.close();
				
			}
//				cout << name << " -- mp3 " << endl;
		}
	}
	closedir(traverses);
}

