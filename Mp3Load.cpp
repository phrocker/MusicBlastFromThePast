//#include <id3/tag.h>
#include <iostream>
//#include <id3.h>
#include <id3/tag.h>
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
void traverse(string);
DB_Handler database;
void add_location();
void update_locations(int action);
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
    cout << "Music substation" << endl;
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
                    cout << " 1) Add location to database" << endl;
                    cout << " 2) Remove location from database" << endl;
                    cout << " 3) Update location in database" << endl;
                    cout << " 4) Analyze location in database" << endl;
                    cout << " 5) View locations in database" << endl;
                    cout << " 6) Exit" << endl;
                    cin >> choice;
                    switch(choice)
                    {
                        case 1: 
                            add_location();
                            break;
                        case 2: 
                            show_locations();
                            update_locations(REMOVE);
                            break;
                        case 3: 
                            show_locations();
                            update_locations(UPDATE);
                            break;
                        case 4:
                            break;
                        case 5:
                            show_locations();
                            break;
                        default: break;                                                      
                    };    
                }while(choice != 6); 
		
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
add_location()
{
    string path="";
    cout << "Enter full path to directory you wish to add" << endl;
    cin >> path;
    traverse(path);
   	cout << total << " Total mp3 files were added" << endl;
   	       	total=0;
   	cout << "Press any key to continue";
    getchar();
    getchar();

}    

void
update_locations(int action)
{
    int locnumber;
    cout << "Enter number for location you wish to";
    if (action==REMOVE)
      cout << " delete" << endl;
    else
     cout << " update" << endl;
    cin >> locnumber;
    Query query;
    query << "select musiclocation,reloadable from musicsettings limit " << locnumber-1 << ",1";
    Result_Set results(query.str()); 
    database.execute_query(query.str(),results);
    query.clear();
    if (results.number_rows() > 0)
    {
    if (action==REMOVE)
    {

        query << "delete from music where location='" << results[0] << "/'";
        database.execute_query(query.str());
    }
    else
    {
        query << "delete from music where location='" << results[0] << "/'";
        database.execute_query(query.str());
        traverse(results[0]);
       	cout << total << " Total mp3 files were added" << endl;
       	total=0;
    }        
}    
   	cout << "Press any key to continue";
    getchar();
    getchar();


}   
void
show_locations()
{
    Query query;
    query << "select musiclocation,reloadable from musicsettings";
    Result_Set results(query.str()); 
    database.execute_query(query.str(),results);
    int loc=0;
    if (results.number_rows() > 0)
        do
        {
            cout << "************************" << endl;
            cout << "Location number: " << ++loc << endl;
            cout << "Location: " << results[0] << endl;
            cout << "Updatable: " << results[1] << endl;
        }while( results++ );
    else
        cout << "There are no locations in the database" << endl;	    
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
				total++;
 				string fname = directory+"/"+name;
				ID3_Tag myTag(fname.c_str());
				ID3_Frame *album = myTag.Find(ID3FID_ALBUM);

				Query query;
				string db = directory+"/";
				string_functions st;
		
				query << "insert into music (location,filename,song,artist,album,filesize) values('" << database.escape(db) <<  "','" << database.escape(name) << "','";

				//get the artists's name
				int dash = name.find("-",0);
				string artist="";
				if (dash < 2000)
				{
					artist=name.substr(0,dash-1);
				//	query << artist;
					name = name.substr(dash+1,name.length()-(dash-1));

				}
				dash = name.find(".mp3",0);
				if (dash < 2000)
				{
					name = name.substr(0,dash);
				}
				query << database.escape(name) << "','" << database.escape(artist);

				query << "','";
				if (album)
				{
					stringstream s;
					
				s	 <<  album->GetField(ID3FN_TEXT)->GetRawText();
					string sd = s.str();
					query << database.escape(sd);
//					query <<t);
				}
				query << "','" << myTag.GetFileSize();
				query << "')";
			//	cout << query.str() << endl;
				database.execute_query(query);
				
			}
//				cout << name << " -- mp3 " << endl;
		}
	}
	closedir(traverses);
}
