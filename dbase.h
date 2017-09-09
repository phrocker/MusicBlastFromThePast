#ifndef DBASE_H
#define DBASE_H

#include <iostream>
#include <string>
#include "Error.h"

#ifdef USE_MYSQL
    #include <mysql/mysql.h>
#endif

#include "string_functions.h"
#include "Query.h"
#include "result_set.h"
using namespace std;

enum CONN_TYPE {INACTIVE=0,ACTIVE};

class DB_Handler
{
    public:
        DB_Handler(){ status=INACTIVE; }
        ~DB_Handler(){ disconnect(); }
    	bool create_connection(string,string,string,string,string);

        bool execute_query(Query input,Result_Set &rs){ return execute_query(input.str(),rs); }
        bool execute_query(Query input){ return execute_query(input.str()); }
        bool execute_query(string,Result_Set &);
        bool execute_query(string);

        bool select_query(string, Result_Set &);

        bool general_query(string);

        unsigned long insert_ID()
        {
            return mysql_insert_id(&CONNECTION);
        }

        string error(string);
    
    	string create_query(char *instring,...);

        string escape(string);
	bool disconnect();
    private:
        CONN_TYPE status;
       #ifdef USE_MYSQL
            #include "mysql_dbase.h"	
       #endif
};
       #ifdef USE_MYSQL
            #include "mysql_dbase.hh"	
       #endif




#endif
