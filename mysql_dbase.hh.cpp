#ifndef MYSQL_DBASE_hh
#define MYSQL_DBASE_hh
bool
DB_Handler::create_connection(string user,string password,string database="",string host="localhost",string port="0")
{
	if (status==ACTIVE)
		disconnect(); // force disconnect	

	mysql_init(&CONNECTION); // initialize the connection prior to opening a table
	try
	{
		if (!(mysql_real_connect(&CONNECTION,host.c_str(),user.c_str(),password.c_str(),database.c_str() ,(unsigned)atoi(port.c_str()),NULL,0)))
			throw( Error("Database","Could not create connection",-1) );
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
	mysql_close(&CONNECTION);
	status=INACTIVE;
	return true;
}
#endif

