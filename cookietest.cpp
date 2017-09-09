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
#define DATA "/home/phrocker/data_files/music.dat"
cgiparser cgi_utilities;	
    LoginManage login;
    #include "mm.h"

int main()
{
	cookies cookieMonster;
	cookieMonster.create_cookie("cookie",cgi_utilities.cgivalue("SESSIONID"));
	cout << "Content-type: text/html\n\n";
	cout << cgi_utilities.cgivalue("SESSIONID") << endl;
	cout << " " << cookieMonster.cookieEntry("cookie") << " " << cookieMonster.cookieEntry("account_id");
	cout <<  " " << getEnv("REMOTE_ADDR");
			
    return 0;
}
