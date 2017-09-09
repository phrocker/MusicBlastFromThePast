//#include <id3/tag.h>
#include <iostream>
//#include <id3.h>
#include <dirent.h>
#include <string>
#include <sstream>
#include "MetaPhone.h"
#include <stdio.h>

using namespace std;


#define USE_MYSQL 1
#include "dbase.h"
#include "Query.h"
#include "result_set.h"
//#include "dbase.h"
#include "string_functions.h"
string metadata(string);
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

DB_Handler database;

int total;
void
traverse(string directory)
{
	
//	cout << directory << " -- directory " << endl;
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
//			cout << name.substr(name.length()-5,4) << endl; // == ".mp3") cout << name << endl;
//			cout << name.find(".mp3",0) << endl;
//			if (name.find(".mp3",0) > 2000) //  || name.find("MP3") > 2000)
//			cout << lowercase(name) << endl;
//			if (name.find(".",0) > 2000) //
            if (lowercase(name).find(".dat",0) > 2000)
			{
//				if (name.find(".MP3") < 2000)	
//					continue;
	//			cout << name << endl;
			
				traverse(directory+"/"+name);
			}
			else
			{
				total++;
 				string fname = directory+"/"+name;
			//	ID3_Frame *comment = myTag.Find(ID3FID_COMMENT);
				//ID3_Frame *usertext = myTag.Find(ID3FID_USERTEXT);
//				ID3_Frame *band = myTag.Find(ID3FID_BAND);
				Query query;
				string db = directory+"/";
				string_functions st;
		        ifstream f(fname.c_str());
                string words="";
                while(f>>words)
                {
    CString one,two;
    MString ss(words);
    ss.DoubleMetaphone(one,two);
				query << "insert into words (word,metaphone) values('" << database.escape(words) << "','" << database.escape(one.data) << "')";
				cout << query.str() << endl;
				database.execute_query(query);				
            }    
				
			//	cout << query.str() << endl;

				
			}
//				cout << name << " -- mp3 " << endl;
		}
	}
	closedir(traverses);
}

int main(int argc, char *argv[])
{	
	total=0;
	database.create_connection("marc","password","test");
//    cout << 	myFrame->GetField(ID3FN_TEXT)->GetRawText() << endl;
	traverse("/windows/g/Programming/spell/"); 
//	cout << metadata("J Geils Band - what if god was one of us.mp3");
	cout << total << " Total words were added" << endl;
 
//}
return 0;

}
