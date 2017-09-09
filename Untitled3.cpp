#ifndef MYSQLHANDLER_HPP
#define MYSQLHANDLER_HPP

public:
	// MUST BE CALLED
	void initialize();

	void create_connection(char *inhost, char *inmysql_user, char *inmysql_password, const char *inmysql_database) // type 2
	void create_connection(MYSQL* inaddress,char *inhost, char *inmysql_user, char *inmysql_password,const  char *inmysql_database);

	int execute_query(int connection_id,const char *query, bool disable_error = false){ return 	execute_query((MYSQL*)get_address(connection_id),query,disable_error); }
	int execute_query(int connection_id,string instring, bool disable_error = false){ return execute_query((MYSQL*)get_address(connection_id),intring.c_str(),disable_error); }
	int execute_query(const char *query, bool disable_error = false){ return execute_query((MYSQL*)get_master(),query,disable_error); }
	int execute_query(string instring, bool disable_error = false){ 	return execute_query((MYSQL*)get_master(),instring.c_str(),disable_error); }
	int execute_query(MYSQL* inaddress, const char *query, bool disable_error = false);
	int execute_query(MYSQL* inaddress, string instring, bool disable_error = false);
	
	int get_num_row_results(int connection_id) return get_num_row_results( (MYSQL*)get_address(connection_id) );
	int get_num_row_results(MYSQL* inaddress=NULL);
	
	int get_num_col_results(int connection_id) return get_num_col_results( (MYSQL*)get_address(connection_id) );
	int get_num_col_results(MYSQL* inaddress=NULL);
	
	string return_result(int connection_id, int return_row_num, int return_row_col = 0) return return_result((MYSQL*)get_address(connection_id),return_row_num,return_row_col);
	string return_result(MYSQL* inaddress,int return_row_num, int return_row_col = 0);

	void disconnect(int connection_id,int type_disconnect = STATUS_DEAD) return disconnect((MYSQL*)get_address(connection_id),type_disconnect);
	void disconnect(MYSQL* inaddress,int type_disconnect = STATUS_DEAD);
	
	char *error(int connection_id){ return error((MYSQL*)get_address(connection_id)); }
	char *error(MYSQL* inaddress);

	unsigned int num_affected(int connection_id){ return changed_rows((MYSQL*)get_address(connection_id)); }
	unsigned int num_affected(MYSQL* inaddress){ return changed_rows((MYSQL*)inaddress); }

	void define_filename(const char *filename, const char *inscriptname);


	int count_query(int connection_id,const char *restquery,...){va_list lst; return mysql_count_query((MYSQL*)get_address(connection_id),restquery,lst);}
	int count_query(int connection_id,string restquery,...){va_list lst; return mysql_count_query((MYSQL*)get_address(connection_id),restquery,lst);}
	int count_query(MYSQL* inaddress,const char *restquery,...);
	int count_query(MYSQL* inaddress,string restquery,...);

	unsigned int status(int connection_id){ return status((MYSQL*)get_address(connection_id)); }
	unsigned int status(MYSQL* inaddress);
	

	void logError(const char *app, char *type, char *method, char *ipaddress, char *host, char *path, char *query_string, char *info,MYSQL* inipaddress);
	void logError(const char *app, char *type, char *method, char *ipaddress, char *host, char *path, char *query_string, char *info, char *mysql_error="",MYSQL* inipaddress=NULL,const char *mysql_query= "");

	void dump_error(const char *app, const char *type, const char *method, const char *ipaddress, const char *host, const char *path, const char *query_string,const char *info, char *mysql_error="", MYSQL* inaddress = NULL,const char *inquery = "");

	string create_query(bool execute, char *instring,...){ va_list lst; return create_query(NULL,execute,instring,lst); }
	string create_query(int connection_id,bool execute, char *instring,...){va_list lst;  return create_query((MYSQL*)get_address(connection_id),execute,instring,lst); }
	string create_query(MYSQL* inaddress,bool execute, char *instring,...);
private:

	char *mypath,*clienterror;

	MYSQL_RES *sqlData;
	MYSQL_ROW row;



	int select_query(MYSQL* inaddress,const char *query,bool disable_error);
	int update_query(MYSQL* inaddress,const char *query,bool disable_error);
	int insert_query(MYSQL* inaddress,const char *query,bool disable_error);
	int delete_query(MYSQL* inaddress,const char *query,bool disable_error);

	void full_disconnect();

	void adjust_connections();

	char *to;

	void htmlerror(char *reportError, int errornum);

#endif

