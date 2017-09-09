#ifndef #MYSQLHANDLER_CPP
#define #MYSQLHANDLER_CPP

void
Database_Handler::intialize()
{
	program_setup->memory_set = 1;
	int internal_results;
	int master_id = new_connection(STATUS_DEAD,MASTER);
//		disconnect((MYSQL*)inaddress,1);
	mysql_init((MYSQL*)get_address(master_id));
	if (!(mysql_real_connect((MYSQL*)get_address(master_id),program_setup->main_host, program_setup->main_user , program_setup->main_password , program_setup->main_database ,0,NULL,0)))
	{
		logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),errorcode[STALE_CONNECTION],error((MYSQL*)get_address(master_id)),(MYSQL*)get_address(master_id));
	}
	
	if (((!mydb && !maindirectory) || (strlen(mydb) == 0 && strlen(maindirectory) == 0)) && strlen(mypath) > 0)
	{
//		char *quickquery = new char[100+strlen(program_setup->executable_path)];
//		sprintf(quickquery,"select sysname from installations where path LIKE '%s%%'",program_setup->executable_path);

		// selecting executable_path% in a like statement is similar to allowing all subdirectories
		// where the login,createaccount,and accountmanager executables reside
		
		mysql_do_query(create_query((MYSQL*)get_address(master_id),false,"select sysname from installations where path LIKE '%s%%'",program_setup->executable_path););

		if (mysql_get_num_row_results() > 0)
		{
			program_setup->system_directory = program_setup->installation_directory;
			program_setup->system_directory += quicksysname;
			program_setup->system_directory += "/";

			program_setup->system_name = return_result((MYSQL*)inaddress,0);
			if (program_setup->check_status == 1)
			{
					
				tempString = "select status,msg from systems where sysname='";
				tempString += program_setup->script_name;
				tempString += "'";
				
				mysql_do_query(tempString);

				if (mysql_get_num_row_results() == 0)
					logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),"Severe error, cannot verify system status!");
				if ((atoi(return_result(0)) == 0)
				{
					tempString = "System is operational. Provided reason:<br>";
					tempString += return_result(0,1);
					logError(program_setup->system_name,"Script",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),quicksysname);

				}
				mysql_do_query("select sysname,status,control_panel,msg,show_link,link_name,script_link,image_link,allow_undo,total_errors,seconds_initialized,encrypt_data from systems");
				

				if (mysql_get_num_row_results() > 0)
				{
					for (int xj=0; xj<=mysql_get_num_row_results()-1; xj++)
					{
						systemMonitor->add_module(atoi(return_result(1).c_str()), atoi( return_result(2).c_str()),
						return_result().c_str(),return_result(3).c_str(),atoi(return_result(4).c_str() ),return_result(5).c_str(),
						return_result(6).c_str(),return_result(7).c_str(),atoi(return_result(8).c_str()),atoi(return_result(9).c_str()),
						atol(return_result(10).c_str()),atoi(return_result(11).c_str());
					}
				}
			}
		
			retreive_database((MYSQL*)get_master(),program_setup->log_database,
				&program_setup->log_username,
				&program_setup->log_password,
				&program_setup->log_host);
				
			program_setup->log_con_id = new_connection(STATUS_DEAD,SLAVE);
			
			create_connection((MYSQL*)get_address(program_setup->log_con_id),program_setup->log_host, program_setup->log_username , program_setup->log_password , program_setup->log_database);

		}else{
			htmlerror(errorcode[DATABASE_ERROR],FAILURE);
//				logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),errorcode[21]);

			}
	}

}

void
Database_Handler::create_connection(int connection_id,char *inhost, char *inmysql_user, char *inmysql_password, const char *inmysql_database)
{
	create_connection(MYSQL*)get_address(connection_id),inhost,inmysql_user,inmysql_password,inmysql_database);
}

void
Database_Handler::create_connection(char *inhost, char *inmysql_user, char *inmysql_password, const char *inmysql_database) // type 2
{
	create_connection((MYSQL*)get_master(),inhost,inmysql_user,inmysql_password,inmysql_database);
}
void
Database_Handler::create_connection(MYSQL* inaddress,char *inhost, char *inmysql_user, char *inmysql_password, const char *inmysql_database) // type 2
{
	if (connection_status((MYSQL*)inaddress) == STATUS_LIVE)
		individual_change_status((MYSQL*)inaddress,STATUS_DEAD);
//		mysql_disconnect((MYSQL*)inaddress,1);
	mysql_init((MYSQL*)inaddress);
	if (!(mysql_real_connect((MYSQL*)inaddress,inhost,inmysql_user,inmysql_password,inmysql_database,0,NULL,0)))
	{
		logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),errorcode[STALE_CONNECTION],error((MYSQL*)inaddress),(MYSQL*)inaddress);
	}else{
		add_connection((MYSQL*)inaddress);
	}

}

int
Database_Handler::execute_query(MYSQL* inaddress,string instring, bool disable_error)
{
	return execute_query((MYSQL*)inaddress,instring.c_str(),disable_error);
}

int
Database_Handler::execute_query(MYSQL* inaddress,const char *query,bool disable_error)
{
	if (inaddress == NULL)	logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),errorcode[INVALID_TYPE],(MYSQL*)inaddress);
	while (*query == 32) query++; 

	// remove space leading spaces

	if (connection_status((MYSQL*)inaddress) == STATUS_LIVE)
		logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),errorcode[STALE_CONNECTION],error((MYSQL*)inaddress),(MYSQL*)inaddress);
	if (!strncmp("select",lowercase(query),strlen("select")) )
		return select_query((MYSQL*)inaddress,query,disable_error);
	else if (!strncmp("insert",lowercase(query),strlen("insert")) )
		return insert_query((MYSQL*)inaddress,query,disable_error);
	else if (!strncmp("update",lowercase(query),strlen("update")) )
		return update_query((MYSQL*)inaddress,query,disable_error);
	else if (!strncmp("delete",lowercase(query),strlen("delete")) )
		return delete_query((MYSQL*)inaddress,query,disable_error);

	if (disable_error == false)
		logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),errorcode[INVALID_TYPE],(MYSQL*)inaddress);
	else
		return FAILURE;
}

int
Database_Handler::get_num_row_results(MYSQL* inaddress)
{
	if (num_rows((MYSQL*)inaddress) != FAILURE)
		return num_rows((MYSQL*)inaddress);
	else
		logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),errorcode[MYSQL_ABORT]);
}


int
Database_Handler::get_num_col_results(MYSQL* inaddress)
{
	if (num_columns((MYSQL*)inaddress) != FAILURE)
		return num_columns((MYSQL*)inaddress);
	else
		logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),errorcode[MYSQL_ABORT]);
}

string
Database_Handler::return_result(MYSQL* inaddress,int return_row_num, int return_row_col)
{
		return retreive_result((MYSQL*)inaddress,return_row_num,return_row_col);
}
// 0 equals total disconnect, severing all access to query elements
// 1 equals minimal disconnect, allowing access to query elements, but
// 	removing access to further queries.
// ===================================================================
void
Database_Handler::disconnect(MYSQL* inaddress,int type_disconnect)
{
	if (individual_change_status((MYSQL*)inaddress,type_disconnect) == FAILURE)
		logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),errorcode[MYSQL_ABORT]);
}

void
Database_Handler::full_disconnect()
{
	if (total_change_status(STATUS_DEAD) == FAILURE)
		logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),errorcode[MYSQL_ABORT]);
}

int
Database_Handler::status(MYSQL* inaddress)
{
	if (connection_status((MYSQL*)inaddress) == FAILURE)
		logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),errorcode[MYSQL_ABORT]);
	
	return connection_status((MYSQL*)inaddress);
}

// return an integer in case disable_error is true

int
Database_Handler::select_query(MYSQL* inaddress,const char *query, bool disable_error)
{
	if (connection_status((MYSQL*)inaddress) == STATUS_DEAD)
		logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),errorcode[STALE_CONNECTION],(MYSQL*)inaddress);	

	register int incrementi,incrementj;

	// from/FROM is the one of the basics of a mysql statement
	if (find(lowercase(query),"from") == FAILURE)
		if (disable_error == false)
			logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),errorcode[INVALID_QUERY],(MYSQL*)inaddress);
		else
			return FAILURE

	if (mysql_real_query((MYSQL*)inaddress,safe_sql(query).c_str(),safe_sql(query).length())
		if (disable_error == false)
			logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),errorcode[MYSQL_ABORT],error((MYSQL*)inaddress),(MYSQL*)inaddress,query);
		else
			return FAILURE;
/*
	if (mysql_errno((MYSQL*)inaddress) != 0)
	{
		cout << query << "\n";
	cout << mysql_stat((MYSQL*)inaddress) << "\n" << mysql_error((MYSQL*)inaddress) << "\n"<< mysql_errno((MYSQL*)inaddress);
	exit(1);

	}
if (!strncmp("select password from mailchick",query,strlen("select password from mailchick")))
	{
	cout << query << "\n";
	cout << mysql_stat((MYSQL*)inaddress) << "\n" << mysql_error((MYSQL*)inaddress) << "\n"<< mysql_errno((MYSQL*)inaddress);
	exit(1);

	}
	*/
	if (mysql_store_result == 0)
	{
		if(zero_out((MYSQL*)inaddress) == -1)
			logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),errorcode[STALE_CONNECTION]);

		mysql_free_result(sqlData);

		return;

	}
	sqlData = mysql_store_result((MYSQL*)inaddress);
	if (!strncmp("select password from mailchick",query,strlen("select password from mailchick")))
	{
		cout << query; exit(1);
	}
	if (!sqlData)
	{
	cout << "Conten-type: text/html\n\n";
cout << mysql_stat((MYSQL*)inaddress) << "\n" << mysql_error((MYSQL*)inaddress) << "\n"<< mysql_errno((MYSQL*)inaddress);
	exit(1);
	}
	row = mysql_fetch_row(sqlData);
	if (!row)
	{
		if(zero_out((MYSQL*)inaddress) == -1)
			logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),errorcode[STALE_CONNECTION]);

		mysql_free_result(sqlData);

		return;

	}

//	int *firstarg = new int;
//	*firstarg = getaddress((MYSQL*)inaddress);
//	numrowresults[(int)*firstarg] = mysql_num_rows(sqlData);
//	numcolresults[(int)*firstarg] = mysql_num_fields(sqlData);

	if ( initiate_result((MYSQL*)inaddress, mysql_num_rows(sqlData), mysql_num_fields(sqlData) ) == FAILURE )
		logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),errorcode[STALE_CONNECTION]);

	for (incrementi = 0; incrementi <= num_rows((MYSQL*)inaddress)-1; incrementi++)
	{

		add_connection_result((MYSQL*)inaddress,incrementi,0,row[0]);
		for (incrementj = 1; incrementj+1 <= numcolresults[(int)*firstarg] && (numcolresults[(int)*firstarg])  > 0; incrementj++)
		{
//				if (row[incrementj] == NULL)
//					maddKey((int)*firstarg,incrementi,incrementj,"");
//				else
			add_connection_result((MYSQL*)inaddress,incrementi,incrementj,row[incrementj]);
//		maddKey((int)*firstarg,incrementi,incrementj,row[incrementj]);
		}
//		if (incrementi + 1 <= numrowresults[(int)*firstarg])
			row = mysql_fetch_row(sqlData);
//			if (!row) break;
		}
	delete  firstarg;

		cout <<"Content-type: text/html\n\n";
		cout << query << "\n" << mysql_stat((MYSQL*)inaddress) << "\n";
		while (1)
			{
				 row = mysql_fetch_row(sqlData);
				 if (row == NULL){ cout << "null\n"; break; }else{ cout <<"not null\n"; }
			}
		mysql_free_result(sqlData);
		return SUCCESS
}

void
Database_Handler::insert_query(MYSQL* inaddress, const char *query, bool disable_error)
{
	if (mysql_real_query((MYSQL*)inaddress, query,strlen(query)))
		if (disable_error == false)
			logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),errorcode[MYSQL_ABORT],error((MYSQL*)inaddress),(MYSQL*)inaddress,query);
		else
			return FAILURE;
	insertid[getaddress((MYSQL*)inaddress)] = mysql_insert_id((MYSQL*)inaddress);
	return SUCCESS;
}

int
Database_Handler::update_query(MYSQL* inaddress,const char *query, bool disable_error)
{
	if (mysql_real_query((MYSQL*)inaddress,query,strlen(query)))
		if (disable_error == false)
			logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),errorcode[MYSQL_ABORT],error((MYSQL*)inaddress),(MYSQL*)inaddress,query);
		else
			return FAILURE;
	return SUCCESS;
}

int
Database_Handler::delete_query(MYSQL* inaddress,const char *query, bool disable_error)
{
	if (mysql_real_query((MYSQL*)inaddress,query,strlen(query)))
		if (disable_error == false)
			logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),errorcode[MYSQL_ABORT],error((MYSQL*)inaddress),(MYSQL*)inaddress,query);
		else
			return FAILURE;
	return SUCCESS;
}

char
*Database_Handler::escape(MYSQL* inaddress,const char *invariable)
{
	to = new char[strlen(invariable)*2+1];
	mysql_real_escape_string((MYSQL*)inaddress,to, invariable, strlen(invariable));
	return to;
}

int
Database_Handler::getaddress(MYSQL* inaddress)
{
	register int i;
	for (i = 0; i <= mysqlentries-1; i++)
	{
		if ((MYSQL*)addressbook[i] == (MYSQL*)inaddress)
			break;
	}
	return i;
}

unsigned int
Database_Handler::num_affected(MYSQL* inaddress)
{
	return mysql_affected_rows((MYSQL*)inaddress);
}


int
Database_Handler::count_query(MYSQL* inaddress,string restquery,...)
{
	va_list lst; 
	return count_query((MYSQL*)inaddress,restquery.c_str(),lst);
}

int
Database_Handler::count_query(MYSQL* inaddress,const char *restquery,...)
{
	va_list lst;
	string myQuery = "SELECT COUNT(*) FROM ";
	myQuery+= create_query((MYSQL*)inaddress,false,restquery,lst);
	  
//	char *myquery = new char[strlen("SELECT COUNT(*) FROM ") + strlen(restquery) + 5];
//	sprintf(myquery,"SELECT COUNT(*) FROM %s",create_query((MYSQL*)inaddress,false,restquery,lst));
	va_list lst; 
	string mysquery = create_query((MYSQL*)inaddress,false,myQuery.c_str(),lst);
	if (mysql_real_query((MYSQL*)inaddress,mysquery.c_str(),mysquery.size()) // see how many rows will be returned
		logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),errorcode[MYSQL_ABORT],error((MYSQL*)inaddress),(MYSQL*)inaddress);

//	delete [] myquery;
	sqlData = mysql_store_result((MYSQL*)inaddress);
	row = mysql_fetch_row(sqlData);
	if (!row)
	{
		mysql_free_result(sqlData);
		return 0;
	}
	while (1)
	{
		row = mysql_fetch_row(sqlData);
		if (row == NULL) break;
	}
	mysql_free_result(sqlData);
	return atoi(row[0]);
}




unsigned int
Database_Handler::changed(MYSQL* inaddress)
{
	return mysql_affected_rows((MYSQL*)inaddress); // maybe this should be used in a structure instead
}

void
Database_Handler::logError(const char *app, char *type, char *method, char *ipaddress, char *host, char *path, char *query_string, char *info, char *mysql_error,MYSQL* inaddress,const char *mysql_query)
{
	if (inaddress == NULL)
		dump_error(app,type,method,ipaddress,host,path,query_string,info,mysql_error,NULL,mysql_query);
	else
		dump_error(app,type,method,ipaddress,host,path,query_string,info,mysql_error,(MYSQL*)inaddress,mysql_query);
	exit(0);
}

void
Database_Handler::logError(const char *app, char *type, char *method, char *ipaddress, char *host, char *path, char *query_string, char *info,MYSQL* inaddress)
{
	dump_errorw(app,type,method,ipaddress,host,path,query_string,info,"",(MYSQL*)inaddress,"");
	exit(0);
}

void
Database_Handler::dump_errorw(const char *app, const char *type, const char *method, const char *ipaddress, const char *host, const char *path, const char *query_string,const char *info, char *mysql_error, MYSQL* inaddress, const char *inquery)
{
	if (inaddress != NULL)
	{
		if ((int)(MYSQL*)inaddress == (int)&error_mysql)
		{
			string generalerror = errorcode[ERROR_ERROR];
			generalerror += "<br>";
			generalerror += mysql_error;  
			htmlerror(generalerror->c_str(),1);
			exit(1);
		}
	}
if (!host) host=ipaddress;
	char *commaslash = "','";
	if (strlen(inquery) > 0)
		inquery = mysql_escape(&error_mysql,inquery);
	else
		inquery = errorcode[QUERY_ERROR];
	if (strlen(mysql_error) > 0)
	{
		char *ininfo;
		ininfo = new char[strlen(info) + strlen(mysql_error) + 20];
		sprintf(ininfo,"%s<br>%s",info,mysql_error);
		memmove(mysql_error,ininfo,strlen(ininfo));

	}

	create_query((MYSQL*)get_address(program_setup->log_con_id),"insert into %S (app,type,time_stamp,request_method,ipaddress,host,script_name,path,query_string,info,mysql_error,myqsql_query) VALUES ('%s','%s','%d','%s','%s','%s','%s','%s','%s','%s','%s','%s')",program_setup->error_table,app,type,time_stamp(),method,ipaddress,host,app,path,query_string,info,mysql_erropr,inquery);
/*
	*squery << "INSERT into " << myerror_table << " (app, type,time,date,request_method,ipaddress,host,script_name,path,query_string,info,mysql_error,mysql_query) VALUES ('" << app << commaslash << type << "', NOW(''),NOW(''),'"  << method << commasla
		*squery << ipaddress << commaslash << host << commaslash << app << commaslash << path << commaslash << query_string << commaslash << info << commaslash << mysql_escape(&error_mysql,mysql_error) << commaslash << inquery << "')" << ends;
*/
//	mysql_do_query(&error_mysql,squery->str());
//	delete squery;
	if (strlen(mysql_error) == 0)
		mysql_error = (char*)info;
	htmlerror(mysql_error,mysql_insert_id(&error_mysql));
}

void
Database_Handler::retreive_database(MYSQL* inaddress,
	const char *database_name,
	char &username,
	char &password,
	char &host )
	{

	string quickquery = "select * from dbaseEntries from dbname='";
	quickquery += program_setup->database_name;
	quickquery +="'";


	mysql_do_query((MYSQL*)inaddress,quickquery);

	if (mysql_get_num_row_results((MYSQL*)inaddress) == 0)
	{
		logError(program_setup->system_name,"MySQL",getenv("REQUEST_METHOD"),getenv("REMOTE_ADDR"),getenv("REMOTE_HOST"),getenv(path_method),getenv("QUERY_STRING"),"Severe error! Cannot run interactive setup!");
	}


	host = return_result((MYSQL*)inaddress,0,1);
//	host = new char[strlen(return_result((MYSQL*)inaddress,0,1)) +2];
//	strcpy(host,return_result((MYSQL*)inaddress,0,1));

	username = return_result((MYSQL*)inaddress,0,2);
//	username = new char[strlen(return_result((MYSQL*)inaddress,0,2)) +2];
//	strcpy(username,return_result((MYSQL*)inaddress,0,2));

	password =return_result((MYSQL*)inaddress,0,3);
//	password = new char[strlen(return_result((MYSQL*)inaddress,0,3)) +2];
//	strcpy(password,return_result((MYSQL*)inaddress,0,3));

}
// returns the address to mysql 
// added so that if one uses oracle
// postgresql, he/she can easily write
// execute_query( log_query() instead of 
// knowing the structure of the database
MYSQL*
Database_Handler::log_query()
{
	return (MYSQL*)get_address(program_setup->log_con_id);
}

void
Database_Handler::define_filename(const char *filename, const char *inscriptname)
{
	if (strlen(program_setup->executable_path) > 0) return;
	
	program_setup->executable_path= new char[ (strlen(getenv(path_method))-strlen(filename))+5];

	strncpy(program_setup->executable_path,getenv(path_method),(strlen(getenv(path_method))-strlen(filename)));
	program_setup->executable_path[(strlen(getenv(path_method))-strlen(filename))] = '\0';

	program_setup->script_name = new char[strlen(inscriptname)+1];
	strcpy(program_setup->script_name,inscriptname);
}

// a convenience function
// a VERY simple form of sprintf. Will add functionality later
// however, it has been shown, in my benchmarks that it is probably
// just as if not more efficient and definitely more advantageous
// to simply use a string and append to the string
string create_query(MYSQL* inaddress, bool execute, char *instring,...)
{
	if (inaddress == NULL) inaddress = (MYSQL*)get_master()
	string query_handler = "";
	const	char *get;
	unsigned long int iget = 0;
	double dget = 0.0;
	string sget = "";
	va_list ap;
	va_start(ap, instring);
	char *to_string = "";
	to = new char[strlen(invariable)*2+1];

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
					mysql_real_escape_string((MYSQL*)inaddress,to_string, get, strlen(get));
					query_handler += to_string;
//					*query_handler << to_string;
					delete [] to_string
					get = "";
					break;
				case 99 :
					get = va_arg(ap,const char *);
					to_string = new char[ 3 ];
					mysql_real_escape_string((MYSQL*)inaddress,to_string, *get, 3);
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
					query_handler +=dget;
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
	
//	string return_string = query_handler->str();
	//delete query_handler;
	if (execute == false) return query_handler;
	
	execute_query((MYSQL*)inaddress, query_handler.c_str());
	return "";

}

#endif

