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
 if (! check_ip() ) return 0;
    //permissions permission("1");
    
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
	    		if (cgi_utilities.cgivalue("action")=="password")
			{
				html html_controls;
				html_controls.html_value("urlsess",cgi_utilities.cgivalue("SESSIONID"));
				html_controls.parsePage("/home/phrocker/public_html/mm/password.html");
			}
			else
				settings(cgi_utilities.cgivalue("SESSIONID"),login);
				
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
	    		if (cgi_utilities.cgivalue("action")=="password")
			{
		
				if (login.change_password(cgi_utilities.form_data("password"),cgi_utilities.form_data("password2")))
				{
					//html_controls.html_value("message","Password changed");	
		                        login.logout();
					display_login("Password Changed<br>Please login again");

				}
				else
				{
						html html_controls;
					html_controls.html_value("message","Passwords not equal<br>Password not changed");	
				html_controls.html_value("urlsess",cgi_utilities.cgivalue("SESSIONID"));
				html_controls.parsePage("/home/phrocker/public_html/mm/password.html");
				}
			}
			else
				settings(cgi_utilities.cgivalue("SESSIONID"),login);
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
