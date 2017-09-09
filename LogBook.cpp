

#include <iostream>
#include "cgiparser.h"
#include "retreive_variables.h"
#include "html.h"
using namespace std;

int main()
{
    try
    {
        cgiparser CGI;
        if (CGI.cgivalue("action") == "login")
            if (CGI.METHOD()==GET)
                display_login();
            else
                login(CGI);
        else
        {
            if (check_login() == true)
                display_main();
            else
                display_login();
        }
    }
    catch(Error er)
    {		
        er.PrintError();
        er.RecordError();
        return -1;

    }
    catch(...)
    {
        Error er("Fatal","Unknown",0,SCREEN);
        er.PrintError();
        return -1;
    }
    return 0;
}


void
display_login()
{
    html html_controls;
    html_controls.parsePage("/var/www/html/login.htm");
}


void
login(cgiparser &my_cgi)
{
    
    string username=my_cgi.form_data("username");
    string password=my_cgi.form_data("password");
    
}


void
display_login()
{
    html html_controls;
    html_controls.parsePage("/var/www/html/login.htm");
}
