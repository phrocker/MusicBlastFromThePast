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
using namespace std;

#include "functions.h"
#include "MetaPhone.h"
#include "spellcheck.h"
#define DATA "/home/tritan/data_files/tritanMain.dat"
cookies cookieMonster;
cgiparser cgi_utilities;	
    LoginManage login;
retreive_values variables;

void
redirect(string id)
{
	cout << "Content-type: text/html\n\n";
	cout << "<html><head><meta HTTP-EQUIV=\"REFRESH\" content=\"0; url=http://tritan.binarystream.net/T_" << id << ".DHTML/tritan/\">";
	cout << "</head><body><center>You should be redirected now. If you aren't, <a href=\"http://tritan.binarystream.net/T_" << id << ".DHTML/tritan/\">click here.</a></center></body></html>"; 
}
int main()
{
	
	if (! check_ip() ) return 0;
    //permissions permission("1");
    variables.parse_file(DATA);
 	string ip = getEnv("REMOTE_ADDR");
 	Query query;
	DB_Handler database;
	database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"),variables.value("db_host"));		
	
if (cookieMonster.cookieEntry("tritan_cookie_id")!="" && cookieMonster.cookieEntry("tritan_id")!="")
{
	query << "select id from html_sessions where id='" <<  cookieMonster.cookieEntry("tritan_id") << "' and cookieID='" <<  cookieMonster.cookieEntry("tritan_cookie_id") << "' and validtime >=" << time(NULL);
	Result_Set results(query.str());
	if (database.execute_query(query,results))
	{
			string randi="";
			if (results.number_rows() > 0)
			{
				redirect(cookieMonster.cookieEntry("tritan_id"));
			}
			else
			{
				string_functions sf;
				string cookie_id=sf.random_string(15);
				string id=sf.random_string(15);
				query.clear();
				query << "insert into html_sessions (id,cookieID,validtime,ip) values('" << id << "','" << cookie_id << "','" << time(NULL)+3600 << "','" << getEnv("REMOTE_ADDR") << "')";
				database.execute_query(query);
				//cout << "Content-type: text/html\n\n";
				cookieMonster.create_cookie("tritan_cookie_id",cookie_id.c_str(),"","/",".binarystream.net");
				cookieMonster.create_cookie("tritan_id",id.c_str(),"","/",".binarystream.net");
				redirect(id);
			}
		}
}
else
{
	string_functions sf;
	string cookie_id=sf.random_string(15);
	string id=sf.random_string(15);
	query << "insert into html_sessions (id,cookieID,validtime,ip) values('" << id << "','" << cookie_id << "','" << time(NULL)+3600 << "','" << getEnv("REMOTE_ADDR") << "')";
	database.execute_query(query);
	//cout << "Content-type: text/html\n\n";
	cookieMonster.create_cookie("tritan_cookie_id",cookie_id.c_str(),"","/",".binarystream.net");
	cookieMonster.create_cookie("tritan_id",id.c_str(),"","/",".binarystream.net");
	redirect(id);
}
		return 0;
	 	
}
