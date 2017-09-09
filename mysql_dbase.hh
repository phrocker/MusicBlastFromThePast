#ifndef MYSQL_DBASE_hh
#define MYSQL_DBASE_hh
#include <stdarg.h>
enum DATABASE_ERROR{UNKNOWN=0,SELECT=1,GENERAL=2};
bool
DB_Handler::create_connection(string user,string password,string database="",string host="localhost",string port="0")
{
	if (status==ACTIVE)
		disconnect(); // force disconnect	
	mysql_init(&CONNECTION); // initialize the connection prior to opening a table
	try
	{
		if (!(mysql_real_connect(&CONNECTION,host.c_str(),user.c_str(),password.c_str(),database.c_str() ,(unsigned)atoi(port.c_str()),NULL,0)))
			throw( Error("Database",error("Could not create connection"),UNKNOWN) );
        else

					
		status=ACTIVE;
		return true;
	}
	catch(Error er)
	{
		er.PrintError();
		disconnect(); // run disconnect in case memory was allocated
		return false;
	}
	
}

bool
DB_Handler::disconnect()
{
//    cout << "could the problem be here?"  << endl;
    if (status==ACTIVE)
    	mysql_close(&CONNECTION);
	status=INACTIVE;
	return true;
}

bool
DB_Handler::execute_query(string my_query,Result_Set &results)
{
    const char *query = my_query.c_str();
    if (status!=ACTIVE)
    {
        Error er("Database","Attempting Query on stale or inactive connection",0,my_query);
        er.PrintError();
        return false;
    }
    try
    {
        if (valid_query(my_query,"select"))
    		return select_query(my_query,results);
        else    
            throw ( Error("Query","Query currently not supported",SELECT,my_query) );
    }
    catch(Error er)
    {
		er.PrintError();
		return false;
    }
}

bool
DB_Handler::execute_query(string my_query)
{
    const char *query = my_query.c_str();
    if (status!=ACTIVE)
    {
        Error er("Database","Attempting Query on stale or inactive connection",0,my_query);
		
        er.PrintError();
        return false;
    }
    try
    {
    	if (valid_query(my_query,"insert"))
	   	   return general_query(my_query);
    	else if (valid_query(my_query,"update"))
	      	return general_query(my_query);
    	else if (valid_query(my_query,"delete"))
    		return general_query(my_query);
        else
            throw ( Error("Query","Invalid query selection",SELECT,my_query) );
    }
    catch(Error er)
    {
		
		er.PrintError();
		return false;
    }
     /*
	else if (valid_query(my_query,"insert"))
		return insert_query(my_query);
	else if (valid_query(my_query,"update"))
		return update_query(my_query);
	else if (valid_query(my_query,"delete"))
		return delete_query(my_query);
    */


}



/*
        QUERY TYPES. FIRST IS SELECT QUERY
*/

bool
DB_Handler::select_query(string query,Result_Set &result)
{

    if (!contains(query,"from"))
    {
        throw( Error("Query","Invalid select query: " + query,SELECT) );
        return false;
    }
      

	if (mysql_real_query(&CONNECTION,query.c_str(),query.length()))
    {
        throw( Error("Query",error("Could Not Execute Query")+query,SELECT) );
        return false;
    }


	result.freeData();

	result.sqlData = mysql_store_result(&CONNECTION);
	
	if ( !result.SetActive() )
	{
		throw( Error("Result","Unable to activate result set",SELECT) );
		return false;
	}
	
   	if (!result.captureSize())
	{
		throw( Error("Result","Error while capturing result size",SELECT) );
	 return false;
	}
    	return true;
}

bool
DB_Handler::general_query(string query)
{
	if (mysql_real_query(&CONNECTION, query.c_str(),query.length()))
    {
		cout << query << endl;
		throw( Error("Query",error("Invalid Query"),GENERAL) );
        return false;
    }
    return true;
}



string 
DB_Handler::error(string err)
{
	if (mysql_error(&CONNECTION)[0] != '\0')
	   err =mysql_error(&CONNECTION);
    
	return err;
}



string 
DB_Handler::create_query(char *instring,...)
{
	string query_handler = "";
    //char *to;
	const	char *get;
	unsigned long int iget = 0;
	double dget = 0.0;
	string sget = "";
	va_list ap;
	va_start(ap, instring);
	char *to_string = "";
//	to = new char[strlen(invariable)*2+1];

//	return to;

	while (*instring)
	{
		if (*instring == 37)
		{
			switch (*(instring+1))
			{
				case 83 :
					get = va_arg(ap,const char *);
					query_handler += get;
//					*query_handler << get;
					get = "";
					break;
				case 115 :
					get = va_arg(ap,const char *);
					to_string = new char[ strlen(get)*2+1 ];
					mysql_real_escape_string(&CONNECTION,to_string, get, strlen(get));
					query_handler += to_string;
//					*query_handler << to_string;
					delete [] to_string;
					get = "";
					break;
				case 99 :
					get = va_arg(ap,const char *);
					to_string = new char[ 3 ];
					mysql_real_escape_string(&CONNECTION,to_string, get, 3);
					query_handler += to_string;
//					*query_handler << to_string;					
					get = "";
					break;
				case 105:
					iget = va_arg(ap,unsigned long int);
					query_handler += iget;
					//query_handler << iget;
					iget = 0;
					break;
				case 100:
					dget = va_arg(ap,double);
                    char *to= new char[sizeof(double)*8];
                    sprintf(to,"%d.2f",dget);
					query_handler += to;
                    delete [] to;
					//query_handler << dget;
					dget = 0;
					break;
			}
			instring= instring+2; continue;
		}
		query_handler += *instring;
//		*query_handler << *instring;
		instring++;
	}
		va_end(ap);
	


	
	return query_handler;

}

string
DB_Handler::escape(string m)
{
   	char *to = new char[strlen(m.c_str())*2+1];
	mysql_real_escape_string(&CONNECTION,to,m.c_str(), strlen(m.c_str()));
//    cout << "here we are" << endl;
//cout << m << " " << to <<  endl;
    m = to;
//cout << m << " " << to <<  endl;
  //  cout << "here we are" << endl;
delete [] to;
	return m;
}

#endif
