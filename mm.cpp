#include <iostream>
#include <string>
#include <sstream>
#define USE_MYSQL 1
#include "cgiparser.h"
#include "cookies.h"
#include "html.h"
#include "loginmanage.h"
#include "ip_restrict.h"
using namespace std;

#include "functions.h"
	
	
cgiparser cgi_utilities;	
    LoginManage login;
    #include "mm.h"
int main()
{
  //  html html_controls;

//    display_dle("marc","lskjdflsdj",database);
  //  return 0;



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
	    		if (cgi_utilities.cgivalue("action")=="ctrl")
				settings(cgi_utilities.cgivalue("SESSIONID"),login);
			else if (cgi_utilities.cgivalue("action")=="messages")
				message_main(cgi_utilities.cgivalue("SESSIONID"),login,cgi_utilities.cgivalue("MESS_ACTION"),cgi_utilities.cgivalue("MESS_ID"),variables);
			else
				main_panel(cgi_utilities.cgivalue("SESSIONID"),login,variables);
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
	    		if (cgi_utilities.cgivalue("action")=="ctrl" && cgi_utilities.cgivalue("ID")=="IP")
				settings(cgi_utilities.cgivalue("SESSIONID"),login);
			else if (cgi_utilities.cgivalue("action")=="messages")
				message_main(cgi_utilities.cgivalue("SESSIONID"),login,cgi_utilities.cgivalue("MESS_ACTION"),cgi_utilities.cgivalue("MESS_ID"),variables);
			else
				main_panel(cgi_utilities.cgivalue("SESSIONID"),login,variables);
            }
           else
            {
                        login.logout();
			display_login("You have been logged out");
            }
}
    //}
    return 0;
/*
	string d="dd";
	cout << "Content-type: text/html\n\n";
	cookies cookieMonster;
	cout << "cookie ist" <<  cookieMonster.cookieEntry("login") << endl;

	if (cgi_utilities.cgivalue("action")=="login")
	{
        	verify_login(cgi_utilities);
	}
    	else
    	{
		verify_login(cgi_utilities);
    	}
    return 0; 
*/   
}
