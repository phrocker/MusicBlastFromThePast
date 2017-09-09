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
#define BACKPAGE "/home/tritan/public_html/universal.html"
#include "functions.h"
#include "MetaPhone.h"
#include "spellcheck.h"
#define DATA "/home/tritan/data_files/tritanMain.dat"
cookies cookieMonster;
cgiparser cgi_utilities;	
    LoginManage login;
retreive_values variables;
bool validate_session(string id)
{
//cout << "Content-type: text/html\n\n";
//	cout << cookieMonster.cookieEntry("tritan_id") << "  d " << cookieMonster.cookieEntry("tritan_cookie_id")
//	cout << "<br>" << getEnv("HTTP_COOKIE");
if (cookieMonster.cookieEntry("tritan_cookie_id")!="" && cookieMonster.cookieEntry("tritan_id")!="")
{
	
	 Query query;
	DB_Handler database;
	database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"),variables.value("db_host"));		
	if (id!=cookieMonster.cookieEntry("tritan_id")) return false;
	
	query << "select id from html_sessions where id='" <<  cookieMonster.cookieEntry("tritan_id") << "' and cookieID='" <<  cookieMonster.cookieEntry("tritan_cookie_id") << "' and validtime >=" << time(NULL);
	Result_Set results(query.str());
	if (database.execute_query(query,results))
	{
			string randi="";
			if (results.number_rows() > 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
}else{

	return false;
}
}
int main()
{
	
	if (! check_ip() ) return 0;
    //permissions permission("1");
    variables.parse_file(DATA);
 	string ip = getEnv("REMOTE_ADDR");

	// /tritan/tritan.cgi?u=$1&a=$2
	if (cgi_utilities.METHOD()==GET )
	{
		if (validate_session(cgi_utilities.cgivalue("u")))
		{
	//		if (cgi_utilities.cgivalue("a")=="")
			{
				html html_controls;
//				html_controls.html_value("message",message);
				html_controls.parsePage("/home/tritan/public_html/index.html");
			}
	//					cout << "Content-type: text/html\n\n";
	//	cout << "<html><body><center>More information to come about tritan</center></body></html>";

		}
		else
		{
			cout << "Content-type: text/html\n\n";
		cout << "<html><body><center><a href=\"/cgi-bin/locate_tritan.cgi\">Please Re-Enter Tritan</a></center></body></html>";
		}
	}
	else
	{
		cout << "Content-type: text/html\n\n";
		cout << "<html><body><center>Not authorized</center></body></html>";
	}


		return 0;
	 	
}
