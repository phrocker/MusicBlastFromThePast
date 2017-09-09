#include <iostream>

//#include <stdlib.h>

#include "cgiparser.h"
#include "retreive_values.h"
using namespace std;
#include <cstdlib>
/*
       string environmentVariables[ 24 ] = {                 
          "COMSPEC", "DOCUMENT_ROOT", "GATEWAY_INTERFACE",   
          "HTTP_ACCEPT", "HTTP_ACCEPT_ENCODING",             
          "HTTP_ACCEPT_LANGUAGE", "HTTP_CONNECTION",         

          "HTTP_HOST", "HTTP_USER_AGENT", "PATH",            

          "QUERY_STRING", "REMOTE_ADDR", "REMOTE_PORT",      

          "REQUEST_METHOD", "REQUEST_URI", "SCRIPT_FILENAME",

          "SCRIPT_NAME", "SERVER_ADDR", "SERVER_ADMIN",      

          "SERVER_NAME","SERVER_PORT","SERVER_PROTOCOL",     

          "SERVER_SIGNATURE","SERVER_SOFTWARE" };  
*/
int main()
{    
    cout << "Content-type: text/html\n\n";
    cout << "Test Page" << endl;
    cgiparser cgi_handler;
    retreive_values catchs;
    catchs.parse_file("doggy.txt");
/*
for ( int i = 0; i < 24; i++ )  
    if (getenv( environmentVariables[ i ].data() ))
        cout << environmentVariables[ i ]               << " "                            << getenv( environmentVariables[ i ].data() )              <<  endl;
*/
    cout << catchs.value("print_totem") << endl;
  if (catchs.value("print_totem") == "yes")
  cout << cgi_handler.cgivalue("totem") << endl; 
    return 0;
}
