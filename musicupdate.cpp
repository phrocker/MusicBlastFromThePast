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
 if (! check_ip() ) return 0;
    //permissions permission("1");
    retreive_values variables;
    variables.parse_file(DATFILE);
   if (cgi_utilities.METHOD()==GET )
    {
		if (cgi_utilities.cgivalue("SESSIONID") == "")
		{
            login.logout();
            display_login("Please Log in");
		}else
		{
           if (login.check_session(cgi_utilities.cgivalue("SESSIONID"))==VALID)
            {
                        				html html_controls; 

                Query query;
           	    DB_Handler database;
                database.create_connection(variables.value("db_login"),variables.value("db_password"),variables.value("database"));

                query.clear();

                
                query << "select song,id,album,genre,artist,decade from MSUsic where updated='0'  LIMIT 0,1";
                  Result_Set results(query.str());

//                Result_Set results(query.str());
                
                if (database.execute_query(query.str(),results))                
                {
                    if (results.number_rows() > 0)
                    {
                        string song="";
                        for (int i=0; i < results["song"].length(); i++)
                            {
                                if (results["song"].at(i) == '.')
                                    break;
                                else
                                {
                                    if (i==0 && results["song"].at(i)==32)
                                    {}
                                    else
                                        song+=results["song"].at(i);
                                }    
                            }    
                        int decade=atoi(results[5].c_str());
                        if (decade > 1999)
                            decade-=2000;
                        else if (decade > 1900)
                            decade-=1900;
                        html_controls.html_value("id",results[1]);
                        html_controls.html_value("title",song);
                        html_controls.html_value("album",results[2]);
                        html_controls.html_value("genre",results[3]);
                        html_controls.html_value("artist",results[4]); 
                        html_controls.html_value("u_artist",cgi_utilities.encoded_cgivalue(results[4]));                              
                        html_controls.html_value("year",decade);
                    }    
                }    
                                        html_controls.html_value("SESSIONID",cgi_utilities.cgivalue("SESSIONID"));
		              html_controls.parsePage("/home/phrocker/public_html/musicupdate.html");

            }
           else
            {
                login.logout();
    			display_login("You have been logged out");
            }

	  }
	  
    } 
    else
    {
  		if (login.check_session(cgi_utilities.cgivalue("SESSIONID"))==VALID)
            {
	   				html html_controls; 

                Query query;
           	    DB_Handler database;
                database.create_connection(variables.value("db_login"),variables.value("db_password"),variables.value("database"));
                int updated=1;
                if (cgi_utilities.form_data("skip") == "true")
                {
                    updated=2;
                                    query << "update MSUsic set genre='" << database.escape(cgi_utilities.form_data("genre")) << "' , decade=" << atoi(cgi_utilities.form_data("year").c_str()) << ",updated='" << updated << "' where id=" << database.escape(cgi_utilities.form_data("id")) << " and updated='0'";
                }    
                else                
                query << "update MSUsic set genre='" << database.escape(cgi_utilities.form_data("genre")) << "' , decade=" << atoi(cgi_utilities.form_data("year").c_str()) << ",updated='" << updated << "' where artist='" << database.escape(cgi_utilities.form_data("u_artist")) << "' and updated='0'";
//                cout << query.str() << " " << cgi_utilities.form_data("u_artist") << endl;
                database.execute_query(query.str());

                query.clear();

                
                query << "select song,id,album,genre,artist,decade from MSUsic where updated='0'  LIMIT 0,1";
                  Result_Set results(query.str());//                Result_Set results(query.str());
                
                if (database.execute_query(query.str(),results))                
                {
                    if (results.number_rows() > 0)
                    {
                        string song="";
                        for (int i=0; i < results["song"].length(); i++)
                            {
                                if (results["song"].at(i) == '.')
                                    break;
                                else
                                {
                                    if (i==0 && results["song"].at(i)==32)
                                    {}
                                    else
                                        song+=results["song"].at(i);
                                }    
                            }    
                        int decade=atoi(results[5].c_str());
                        if (decade > 1999)
                            decade-=2000;
                        else if (decade > 1900)
                            decade-=1900;
                        html_controls.html_value("id",results[1]);
                        html_controls.html_value("title",song);
                        html_controls.html_value("album",results[2]);
                        html_controls.html_value("genre",results[3]);
                        html_controls.html_value("artist",results[4]);
                        html_controls.html_value("u_artist",cgi_utilities.encoded_cgivalue(results[4]));       
                        html_controls.html_value("year",decade);
                    }    
                }    
                                        html_controls.html_value("SESSIONID",cgi_utilities.cgivalue("SESSIONID"));
		              html_controls.parsePage("/home/phrocker/public_html/musicupdate.html");
            }
           else
            {
                        login.logout();
			display_login("You have been logged out");
            }
}
    //}
    return 0;
}
