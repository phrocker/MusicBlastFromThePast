#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#define USE_MYSQL 1
#include "cgiparser.h"
#include "cookies.h"
#include "html.h"
#include "loginmanage.h"
#include "ip_restrict.h"
#include "string_functions.h"
#include <sys/types.h> 
#include <sys/wait.h> 
#include <unistd.h>  
using namespace std;
#include "lib.h"
#include "functions.h"
#include "MetaPhone.h"
#include "spellcheck.h"
#define DATA "/home/phrocker/data_files/music.dat"
cgiparser cgi_utilities;	
    LoginManage login;
    #include "mm.h"
retreive_values variables;


	ifstream openFile;

ifstream 
&GetFileRef(string filepath);

void
streamData(ifstream &,string filesize,string = "NOT ALLOWED TO DOWNLOAD MUSIC.mp3");

int main()
{
	int pid, status;
    /*
     * Get a child process.
     */
    if ((pid = fork()) < 0)
    {
	    create_log("Error forking!");
        exit(1);
    }
      if (pid == 0) {
	   create_log("Child process created for %s at PID %i",getEnv("REMOTE_ADDR"),pid);
	string def = "/windows/f/mymusico/Beethoven - Ode to Joy.mp3";
	string fs="1061276";
	if (! check_ip() ) return 0;
	    //permissions permission("1");
 	variables.parse_file(DATA);
	if (cgi_utilities.cgivalue("STREAMID")!="")
	{
		Query query;
		
	    DB_Handler database;
		database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"));
		if (cgi_utilities.cgivalue("a")!="q")
		{
		query << "select timestamp,location,filename,filesize from stream where streamID='" << cgi_utilities.cgivalue("STREAMID") << "' and IP ='" << getEnv("REMOTE_ADDR") << "' order by queueNum asc";		Result_Set results(query.str());
		if (database.execute_query(query,results))
		{
			if (results.number_rows() > 0)
			{
				string filename= results["location"] + results["filename"];
//				cout << "Content-type: text/html\n\n";
//				cout << filename << endl;
//				return 0;
				streamData(GetFileRef(filename),results["filesize"],results["filename"]); 
			}
			else
				return 0;
		}
		else
		{
			cout << "Content-type: text/html\n\n";
			cout << "Not a valid stream";
			return 0;
		}
		}
		else
		{
			bool start=false;
			int loc=0;
			query << "select timestamp,location,filename,filesize,queueNum from stream where streamID='" << cgi_utilities.cgivalue("STREAMID") << "'  order by queueNum asc limit 0,1";		
			Result_Set results(query.str());
			Result_Set result(query.str());
		//	cout << "Content-type: text/html\n\n";
			while(1)
			{
				if (start)
				{
					query.clear();
					query << "select timestamp,location,filename,filesize,queueNum from stream where streamID='" << cgi_utilities.cgivalue("STREAMID") << "' and queueNum >= " << ++loc << "  order by queueNum asc limit 0,1";
				}
				if (database.execute_query(query,results))
				{
					if (results.number_rows() > 0)
					{
						/*
						query.clear();
						query << "select count(*) as cnt from stream where streamID='" << cgi_utilities.cgivalue("STREAMID") << "'";
						if (database.execute_query(query,result))
						{
							if (atoi(result[0].c_str()) >1)
							{
								query.clear();
								query << "delete from stream where queueNum=" << results[4] << " and streamID='" << cgi_utilities.cgivalue("STREAMID") << "'";
								
								cout << query.str();
								database.execute_query(query);
							}
						}
						*/
						string filename= results["location"] + results["filename"];
						streamData(GetFileRef(filename),results["filesize"],results["filename"]);
						
						//sleep(5);
						start=true;
						loc = atoi(results[4].c_str() );
					}
					else
					{	
//						streamData(GetFileRef(def),fs); 						
									cout << "Content-type: text/html\n\n";
			cout << "Not a valid stream";
			return 0;

				}
				}
				else
				{
								cout << "Content-type: text/html\n\n";
			cout << "Not a valid stream";
			return 0;

					}
			}
		}
		create_log("child process exiting");
	}
	else
	{
					cout << "Content-type: text/html\n\n";
			cout << "Not a valid stream";
			return 0;

	}
	}
	else
	{
		create_log("Parent process created for %s at PID %i",getEnv("REMOTE_ADDR"),pid);
		wait(NULL);
		create_log("Parent process exiting");
	}
	
	return 0;
}

ifstream 
&GetFileRef(string filepath)
{
	openFile.open(filepath.c_str(), ios::in|ios::binary|ios::ate );
	return openFile;
}

void
streamData(ifstream &charlie,string filesize,string filename)
{
			cout << "Content-type: mpeg/audio\n";
//		cout << "Content-Length: " << filesize << "\n";
		cout << "Content-Disposition: attachment ; filename= " << filename << "\n\n";
		long size=0;
	   size = charlie.tellg();


					//while (charlie)
					//	cout << charlie;
					
					int div = size; // eights
					int fsize= atoi ( filesize.c_str() );
//					if ((fsize/(1024*1024)) > 10)
					{
						div = fsize/(1024);
					}
					char *buffer;
					buffer= new char[div];
					//char buffer[size];
                    charlie.seekg(0);
                    
  				for (int i =0; i<= size; i+=div)
                    {
         				if ((size-i) < div)
         				{
         				charlie.read(buffer,(size-i));
         					
         					cout.write(buffer,(size-i));
         					break;
                         }  				
         				else
         				{
    				       charlie.read(buffer,div);
    				       	cout.write(buffer,div);
    				    }
					    


				
                	}	
                     delete [] buffer;			
					charlie.close();

}
