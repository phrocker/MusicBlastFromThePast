#include <iostream>
#include <string>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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
#define DATA "/home/phrocker/data_files/music.dat"
cgiparser cgi_utilities;	
    LoginManage login;
    #include "mm.h"
retreive_values variables;



int main()
{
	variables.parse_file(DATA);
	string line="";
	double i=0;
	int j=0;
	Query query;
	Result_Set results(query.str());
	DB_Handler database;
	database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"));
	if(blockFile)
	{
		if( getline(blockFile,line) )
		{
//			if (j++ == 2) break;
			string Name=line.substr(0, line.length()-line.find(":")-1);
			string ipRange = line.substr(line.find(":")+1, line.length()-line.find(":"));
			
			octet t1,t2;
			string first = ipRange.substr(0,ipRange.find("-"));
			string second = ipRange.substr(ipRange.find("-")+1, ipRange.length()-ipRange.find("-")-2);
			t1.i = (char)atoi( first.substr(0,first.find(".") ).c_str() );
			first=  first.substr(first.find(".")+1, first.length()-first.find(".") );
			t1.j = (char)atoi( first.substr(0,first.find(".") ).c_str() );
			first=  first.substr(first.find(".")+1, first.length()-first.find(".") );
			t1.k = (char)atoi( first.substr(0,first.find(".") ).c_str() );
			first=  first.substr(first.find(".")+1, first.length()-first.find(".") );
			t1.l = (char)atoi( first.substr(0,first.find(".") ).c_str() );
			cout << (unsigned long)inet_network(second.c_str()) << endl;
			cout << (unsigned long)inet_network("255.255.255.255") << endl;
			
			t2.i = (char)atoi( second.substr(0,second.find(".") ).c_str() );
			second=  second.substr(second.find(".")+1, second.length()-second.find(".") );
			t2.j = (char)atoi( second.substr(0,second.find(".") ).c_str() );
			second=  second.substr(second.find(".")+1, second.length()-second.find(".") );
			t2.k = (char)atoi( second.substr(0,second.find(".") ).c_str() );
			second=  second.substr(second.find(".")+1, second.length()-second.find(".") );
			t2.l = (char)atoi( second.substr(0,second.find(".") ).c_str() );
			query.clear();
			query << "insert into blockList (name,min,max) values('" << name << "'," << (unsigned long)inet_network(first.c_str()) << "," << (unsigned long)inet_network(second.c_str()) << ")";
			database.executeQuery(query.str());
				i++;
//			blockList[t2]=true;
//			cout << endl;
		}
	}
	cout << i << " IP Addresses to database" << endl;
	blockFile.close();
}


