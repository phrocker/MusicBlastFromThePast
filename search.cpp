#include <iostream>
#include <string>
#define USE_MYSQL 1
#include "cgiparser.h"
#include "cookies.h"
#include "html.h"
#include "hidden.h"
#include "rijndael_encryption.h"
#include "permissions.h"
#include "loginmanage.h"

using namespace std;

	cgiparser cgi_utilities;
    LoginManage login;
    DB_Handler database;
#include "functions.h"
#include "search.h"
int main()
{
    if (! check_ip() ) return 0;

    retreive_values variables;
    variables.parse_file(DATFILE);


    database.create_connection(variables.value("db_login"),variables.value("db_password"),variables.value("search_db"));


    string st;
    html html_controls;

    if (cgi_utilities.cgivalue("SESSIONID") == "")
	{
        login.logout();
		display_login("Please Log in");
	}
    else
	{
         if (cgi_utilities.METHOD()==POST )
        {
          if (login.check_session(cgi_utilities.cgivalue("SESSIONID"))==VALID)
           {
                    Search searchObject;
                    string rt = searchObject.SearchFor(cgi_utilities.form_data("search"),cgi_utilities.form_data("begin"),cgi_utilities.form_data("numRequest"));
                    html_controls.html_value("searchTerm",cgi_utilities.form_data("search"));
                    html_controls.html_value("search_results",rt);
           }
          else
           {
                        login.logout();
			            display_login("You have been logged out");
            }

	     }
	  
    }        

      if (cgi_utilities.METHOD() == POST)
	 {          
	  
	 }
      html_controls.parsePage("/home/marc/public_html/search.htm");
//    }while(st!="-1");
    
}
