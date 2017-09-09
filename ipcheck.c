#include <iostream>
#include <string>
#include <fstream>
#include <sys/types.h>
#define USE_MYSQL 1
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
#include "ip_addy.h"
#include "functions.h"
#include "MetaPhone.h"
#include "spellcheck.h"
#define DATA "/home/phrocker/data_files/music.dat"
cgiparser cgi_utilities;	
    LoginManage login;
    #include "mm.h"
retreive_values variables;

#define FILE "guarding_all.p2p"

int main(int argc, char *argv[])
{
	variables.parse_file(DATA);
	string line="";
	double i=0;
	int j=0;
	Query query;
	Result_Set results(query.str());
	DB_Handler database;
	database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"));
	if (argc > 3 && !( (argc-1) %3))
	{
		
		ofstream add(FILE,ofstream::app);
		if (add)
		{
			for (int k = 1; k <= argc; k+=2)
			{
				add << argv[k] << ":" << argv[k+1] << "-" << argv[k+1] << endl;
			}
		}
		add.close();
	}
	else
	{
		cout << "usage:\n ./ipcheck <name> <ipaddress>" << endl;
	}
	ifstream blockFile(FILE);
	
	
	if(blockFile)
	{
		query << "delete from blockList";
		database.execute_query(query);
		while( getline(blockFile,line) )
		{
//			if (j++ == 2) break;
			string name=line.substr(0, line.length()-line.find(":")-1);
			string ipRange = line.substr(line.find(":")+1, line.length()-line.find(":"));
			
			string first = ipRange.substr(0,ipRange.find("-"));
			string second = ipRange.substr(ipRange.find("-")+1, ipRange.length()-ipRange.find("-")-1);
			/*
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
			*/
			query.clear();
			query << "insert into blockList (name,min,max) values('" << database.escape(name) << "'," << (unsigned long)inet_network(first.c_str()) << "," << (unsigned long)inet_network(second.c_str()) << ")";
			database.execute_query(query.str());
				i++;
//			blockList[t2]=true;
//			cout << endl;
		}
	}
	cout << i << " IP Addresses to database" << endl;
	blockFile.close();
}


