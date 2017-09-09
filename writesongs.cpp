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
using namespace std;

#define LOWERCASE(c) (char)(c|0x20)
#define UPPERCASE(c) (char)(c&0xdF)

#include "functions.h"
#include "MetaPhone.h"
#include "spellcheck.h"
#define DATA "/home/phrocker/data_files/music.dat"
cgiparser cgi_utilities;	
    LoginManage login;
    #include "mm.h"
retreive_values variables;

int main()
{
    retreive_values variables;
    variables.parse_file(DATFILE);

                Query query;
           	    DB_Handler database;
                database.create_connection(variables.value("db_login"),variables.value("db_password"),variables.value("database"));

                query.clear();

                
                query << "select song,id,album,genre,artist,decade,updated,filesize,location,downloads from MSUsic where updated='1'";
                  Result_Set results(query.str());

//                Result_Set results(query.str());
                
             if (database.execute_query(query.str(),results))                
             {
                if (results.number_rows() > 0)
                {
                    ofstream writeMusic("music.sql");
                    do
                    {
                        string song="";
			string artist="";
			string genre="";
			for (int j=0; j < results["artist"].length(); j++)
			{
				 if (results["artist"].at(j) == '&')
                                 {
					artist+=" And ";
                                }
                                else
                                {
                                    if (j==0 && results["artist"].at(j)==32)
                                    {}
                                    else
                                        artist+=LOWERCASE(results["artist"].at(j));
                                }  
			}
                        for (int i=0; i < results["song"].length(); i++)
                            {
                                if (results["song"].at(i) == '.')
                                    break;
				else if (results["song"].at(i)== '&')
				{
					song+=" And ";
				}
                                else
                                {
                                    if (i==0 && results["song"].at(i)==32)
                                    {}
                                    else
                                        song+=LOWERCASE(results["song"].at(i));
                                }    
                            }
			for (int k=0; k < results["genre"].length(); k++)
			{
				genre+=UPPERCASE(results["genre"].at(k));				
			}
                        writeMusic << "insert into MSUsic (id,song,album,genre,artist,decade,updated,filesize,location,downloads,filename)";
                        writeMusic << "VALUES(" << results["id"] << ",'" << song << "','" << results["album"] << "','" << genre << "','" << artist  << "'," << results["decade"];
                        writeMusic << ",1,0,'location',0,'default');" << endl;


                    }while( results++ );   
                    writeMusic.close(); 
                }
            }        
    return 0;
}