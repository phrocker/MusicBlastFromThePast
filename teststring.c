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

	

int main()
{
    cout << "Content-type: text/html\n\n";
    cgiparser cgi_utilities;
    cout << "testing input. test parameter: " << cgi_utilities.cgivalue("test");
    cout << "testing input. encoded test parameter: " << cgi_utilities.encoded_cgivalue("test");
    return 0;
}    
