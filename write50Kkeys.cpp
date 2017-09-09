#include <iostream>
#include <fstream>
#include <string>
#define USE_MYSQL 1
#include "html.h"
#include "dbase.h"
#include "Query.h"
#include "result_set.h"

#include "md5.h"
using namespace std;

#include "string_functions.h"


int main()
{
    DB_Handler database;
    database.create_connection("testUser","","test");

    Query query;


	string_functions sf;    
    cout << "Preparing to insert into key database" << endl;
   
    for (int i =0; i <50000; i++)
    {
      
//    string_functions sf;
//        system("sleep .5s");
//      string ran = sf.random_string(25)
	string ran="";
	char r=0;
	ifstream frand("/dev/urandom");
	/*
	while (1)
	{
		frand.get(r);
		if ((r > 0x2F && r <= 0x39) || ((r|0x20) >= 0x61 && (r|0x20) <=0x7A))
		 ran+=r;
		if(ran.length()==8) break;
	}
		*/
		
	ran = sf.random_string( 10 );
	string md5s="";
	while (1)
	{
		frand.get(r);
		if ((r > 0x2F && r <= 0x39) || ((r|0x20) >= 0x61 && (r|0x20) <=0x7A))
		 md5s+=r;
		if(md5s.length()==50) break;
	}		
	frand.close();

        query.clear();
        query << "insert into `keys` (`sid`,`kid`) values('" << ran  << "','" << MD5String((char*)md5s.c_str()) << "');";
//        cout << query.str() << endl;
        database.execute_query(query);
    }
    cout << "Finished adding 50,000 keys" << endl;
    return 0;
}
