#include <iostream>
#include <string>
#define USE_MYSQL 1 // tell the system that we are using MySQL
#include "cgiparser.h"
#include "cookies.h"
#include "html.h"
#include "loginmanage.h"


#include "dbase.h"
#include "Query.h"
#include "result_set.h"
#include "time_functions.h"


using namespace std;
#include "ip_restrict.h"

/**
*   MAIN LOGIN FILE. USE LOGIN.DAT FROM FUNCTIONS.H\DATFILE
*   LOGIN IS BASED ON SEVERAL FACTORS: COOKIES, DATABASE, ETC
*   A RANDOM LOGIN ID IS CREATED. ITS HASH WILL BE USED AS THE
*   SESSION ID IN URL AND COOKIE. ALL URLS *MUST* CONTAIN THE 
*   SESSIONID CGI VALUE. IF IT IS NOT THERE, LOG THAT USER OUT
*   BASED UPON HIS OR HER COOKIE. NORMALLY IP ADDRESSES WOULD BE USED
*   BUT UNFORTUNATELY, DUE TO NAT, WE MAY HAVE ONE ADDRESS FOR HUNDRES
*   OR THOUSANDS OF INDIVIDUALS. CHECK_IP() BELOW CALLS A ROUTINE TO CHECK
*   IP ADDRESS BLOCKS PER GETENV("REMOTE_ADDR")
**/
cgiparser cgi_utilities;


#include "functions.h"
int main()
{


    if (! check_ip() ) return 0;

    LoginManage login;

    
    
    retreive_values variables;
    variables.parse_file(DATFILE);

    if (variables.value("disable_login")=="1")
    {
        string ad = "Login deactivated, contact admin at ";
        ad += variables.value("admin_email");
     	display_login(ad);
        return 0;
    }

    //DB_Handler database;
    //database.create_connection(variables.value("db_login"),variables.value("db_password"),variables.value("database"),variables.value("host"));

   if (cgi_utilities.METHOD()==GET )
    {
	if (cgi_utilities.cgivalue("action")=="HOME")
	{
	   	html html_controls; 
		retreive_values variables;
	    variables.parse_file(DATFILE);
	
		DB_Handler database;
		
		database.create_connection(variables.value("db_login"),variables.value("db_password"),variables.value("database"));
											
		Result_Set results;
		
		Query query;
	
	
		
		query << "select kid,sid from `keys` order by rand() limit 0,1";
		
	
	
		if (database.execute_query(query,results))
		{
			if (results.number_rows() > 0)
			{
				html_controls.html_value("special",results[0]);
				html_controls.html_value("sid",results[1]);
			}
			else
				html_controls.html_value("special","NAN");
		}
		else
			html_controls.html_value("special","NAN");
	
	 	//   html_controls.html_value("message",message);
		add_bottom(html_controls);
		html_controls.dump_core("/home/marc/public_html/home.html",1);
	}

	else
     if (cgi_utilities.cgivalue("action")=="create")
	   {
/*
	   	if (cgi_utilities.form_data(
		Query query;
		Result_Set results();
		query.
		database.create_connection(variables.value("db_login"),variables.value("db_password"),variables.value("database"),variables.value("host"));

*/  
        if (variables.value("allow_create")=="1")
        {
	   	html html_controls; 
		add_bottom(html_controls);
		html_controls.parsePage("/home/marc/public_html/adduser.html");
        }
        else
            display_login( variables.value("allow_create") );
		
	   }
	   else if (cgi_utilities.cgivalue("SESSIONID") == "")
		{
            login.logout();
			display_login("Please Log in");
		}
		
        else
           if (login.check_session(cgi_utilities.cgivalue("SESSIONID"))==VALID)
                      main_panel(cgi_utilities.cgivalue("SESSIONID"),login,variables);
	   
           else
            {
                        login.logout();
			            display_login("You have been logged out");
            }
	  
    }
    
    else
    {
    	
   		if (cgi_utilities.cgivalue("action")=="LOGIN")
		{
			
            string usl =login.login(cgi_utilities.form_data("username"),cgi_utilities.form_data("password"));

	           if (usl.length()==0)
                display_login("Please log in with proper credentials");
            else
			{
				string secret_phrase = cgi_utilities.form_data("verify");
				string user_phrase = cgi_utilities.form_data("verifyUser");
				DB_Handler database;
	
				database.create_connection(variables.value("db_login"),variables.value("db_password"),variables.value("database"));
														
				Result_Set results;
				
				Query query;
				
								
				query << "select kid from `keys` where kid = '" << secret_phrase << "' and sid = '" << user_phrase << "'";
				
				
				
				if (database.execute_query(query,results))
				{
					if (results.number_rows() > 0)
					{
						main_panel(usl,login,variables);
					}
					else
						display_login("Please log in with proper credentials");
				}
				else
					display_login("Please log in with proper credentials");
					
                
			}
		}	
        else if (cgi_utilities.cgivalue("action")=="create")
	   {
        if (variables.value("allow_create") == "1")
        {
    	   	if (cgi_utilities.form_data("username").length() > 0)
    		{
	      		if (!login.user_exist(cgi_utilities.form_data("username")))
    			{
	    			string group_id = "";
    				if (cgi_utilities.form_data("password")!=cgi_utilities.form_data("password2"))
    				{
        				html html_controls; 
    					html_controls.html_value("username",cgi_utilities.form_data("username"));
    					html_controls.html_value("message","Passwords do not match");
							add_bottom(html_controls);
		              html_controls.parsePage("/home/phrocker/public_html/adduser.html");
    				}
    				else if ( ( group_id = login.verifyACID( cgi_utilities.form_data("acid") ) ) == "")
    				{
	    				
	    				display_login("Account cannot be created");
    				}
   	   				else
   	   				{
	   	   				login.destroyACID( cgi_utilities.form_data("acid") );
	   				login.create_account(cgi_utilities.form_data("username"),cgi_utilities.form_data("password"),group_id);
					display_login("Account has been created<br>You may now log in");
					//html_controls.html_value("message","Account has been created");
					// create account
				}
			}
			else
			{html html_controls; 
			
				html_controls.html_value("username",cgi_utilities.form_data("username"));
				html_controls.html_value("message","Account already exists");
						add_bottom(html_controls);
		html_controls.parsePage("/home/phrocker/public_html/adduser.html");
			}
        }    
        else
        {   
            html html_controls; 
			
				html_controls.html_value("username",cgi_utilities.form_data("username"));
				html_controls.html_value("message","Account already exists");
						add_bottom(html_controls);
		html_controls.parsePage("/home/phrocker/public_html/adduser.html");
            }
			
		}
		else
		{
		html html_controls; 
			
				html_controls.html_value("message","Enter a username");
						add_bottom(html_controls);
			html_controls.parsePage("/home/marc/public_html/adduser.html");
		}

	   	

	   }
	   else	
  	        display_login("You have been logged out");


    }
    return 0;  
}
