#ifndef LOGINMANAGE_HPP
#define LOGINMANAGE_HPP

#include <string>
#include "retreive_values.h"
#include "md5.h"
#include "dbase.h"
#include "Query.h"
#include "result_set.h"
#include "permissions.h"
#include "cookies.h"
//#include "time_management.h"
#include "time_functions.h"
#include "string_functions.h"
#include "env_functions.h"
#define DAT_LOCATION "/home/marc/data_files/login.dat"

using namespace std;
/*  TABLE DEFINITION
    log_id -- randomid
    account_id -- user's id
    ip -- user's ip address
    connect_date
    connect_time
    hashid -- hash of log_id
    
*/
enum SESSIONS{ VALID=1,MYSQL_ERROR=2,NONE=3,INVALID=4,NOACCT=5,NOACTIVE=6 };
class LoginManage
{
public:
	LoginManage(string = "");
	~LoginManage();
	int check_session(string);
	string login(string,string); // username and password
	
    void logout(string hid="")
    {
      if (hid.length() > 0 )
            archive_sessions(hid);

//        cookieMonster.create_cookie("session_id","");
  //      cookieMonster.create_cookie("account_id","");
			cookieMonster.clear_cookies();
    }
	//void delete_session(const char *inipaddress);

	//void get_site(const char *insitevalue);

    struct usr
    {
        string session_id; // session id
        string hash_id; // hash of session_id
        string group_id;
        string group_name;
        string group_hash;
        string group_key;
        string account_id; // cookie acct_id should be hash of this	
	   string urlSESSION;
        string username;
    } user;
    permissions permission;

    string
    get_hash(int gid, string workgroup)
    {
        int d = 1234567890;
        char *dd= new char[sizeof(gid)*8];
        sprintf(dd,"%i",d);
        string hsh = MD5String((char*)workgroup.c_str());
        string temp="";
        for ( int i =0; i < hsh.length(); i++)
        {
            temp+=hsh[i];
            if (i < strlen(dd))
                temp+=dd[i];        
        }
        delete [] dd;
        string fhash=MD5String((char*)temp.c_str());
        return fhash;
    }

    string
    group_key()
    {
        if (user.group_key.length() > 0)
            return user.group_key;
        if (user.group_hash.length() == 0)
            return "";
        query << "select key from keys where group_hash='" << user.group_hash << "'";
        Result_Set results(query.str());
        if (database.execute_query(query,results))
        {
            user.group_key=results[0];
            return results[0];
        }
        else
            return "";
   
    }
    
    bool user_exist(string);
    bool create_account(string,string,string);
    bool change_password(string,string);
    string verifyACID(string);
    void destroyACID(string);

private:
    retreive_values login_variables;
    
    DB_Handler database;
    cookies cookieMonster;
//    Time_Management mytime;
    time_functions mytime;
    string site;
    Query query;    
	void archive_sessions(string);
	void archive_sessions_by_acID(string);
//	bool acceptable_site(const char *insite);
//	bool set_variables;

};

//  void update_session(const char *inipaddress,const char *scriptname,const char *actiontitle, const char *actiontable,const char *actiontype, const char *actiondescription, int undo);
//	void update_session(const char *inipaddress,const char *scriptname,const char *actiontitle, const char *actiontable,const char *actiontype, const char *actiondescription);


/*
	functions no longer in use. originally used for loggin
		void addaction(string myid, const char *myscript, const char *mytitle, const char *mytable,const char *mytype, const char *mydescription, const char *myip, int force_undo)
void clearactions();
*/


/*
	 greatly shortened using create_query function
	*/

    // loginmanage contains no database specific information
    // Prior to this update, (MYSQL*)address[0] was used
    // now, if the programmer wishes to use this login 
    // management system, he/she must also add this database
    // under the user of program_setup->main_user for 
    // security reasons

	// 0.01 -- need to work on setvariables.
	// it is unfinished, insecure, and dirty
LoginManage::LoginManage(string st)
{
    site=st;
    user.group_key="";
    login_variables.parse_file(DAT_LOCATION);
    database.create_connection(login_variables.value("db_user"),login_variables.value("db_password"),login_variables.value("database"),login_variables.value("host"));
    
    //database.create_connection("root","password","test");
    // DO SOMETHING HERE
}
	// removed all deletes. Consolidated memory a tad
	// may remove this 
LoginManage::~LoginManage()
{
}
//enum SESSIONS{ VALID=1,MYSQL_ERROR=2,NONE=3,INVALID=4,NOACCT=5,NOACTIVE=6 };
// 1 means there is a valid session. Anything above is an error
// 2 is a mysqlerror ; this has pretty much been eradicated
// 3 means there is no session
// 4 is an invalid session
// 5 user account does not exist ; added 5 so we can tell whether a user account exists or not.
// 6 The account has been deactived for one reason or another; status tells reason. 

bool
LoginManage::user_exist(string username)
{
	query.clear();
	query << "select username from users where username='" << username << "'";
	Result_Set results(query.str());
	if (database.execute_query(query,results))
	{
		if (results.number_rows() >0)
			return true;
		else
			return false;
	}
	else
		return false;
}

string
LoginManage::verifyACID( string acid )
{
	query.clear();
	query << "select group_id,inviteID from invitations where inviteID ='" << acid << "'";
	Result_Set results(query.str());
	if (database.execute_query(query,results))
	{
		if (results.number_rows() > 0)
			return results[0];
		else
			return "";
			
	}
	else
		return "";
	
}

void LoginManage::destroyACID( string acid )
{
	query.clear();
	query << "delete from invitations where inviteID ='" << acid << "'";
	database.execute_query(query);
	
}

bool
LoginManage::create_account(string username,string password,string group_id)
{
		string_functions sF;
		string newid=sF.random_string(15);

	query.clear();
	query << "insert users (account_id,group_id,username,password,session_length) values('" << newid << "','" << group_id << "','" << username << "','" << password << "',60)";
	
	if (database.execute_query(query))
	{
	
			return true;
	}
	else
		return false;
}

bool
LoginManage::change_password(string password1,string password2)
{

	if (password1!=password2 || password1.length()== 0) return false;
	query.clear();
	query << "update users set password='" << password1 << "' where account_id='" << user.account_id << "'";
	
	if (database.execute_query(query))
	{
	
			return true;
	}
	else
		return false;
}

int
LoginManage::check_session(string inid)
{
///    //cout << "Content-type: text/html\n\n";
  //  //cout << "yo";
    //return VALID;
	// get the username from the user's ip address
	////cout << "ok till here" << endl;
    if (cookieMonster.cookieEntry("session_id").length()==0 || cookieMonster.cookieEntry("account_id").length() == 0)
	{
	//cout << "Content-type: text/html\n\n";
	//cout << "shizzle" << endl;
        return INVALID;
}
    if (cookieMonster.cookieEntry("session_id")=="")
    { 
//cout << "Content-type: text/html\n\n";
	//cout << "shizzle" << endl;
        return INVALID;
    }
//	if (MD5String((char*)inid.c_str()) != cookieMonster.cookieEntry("session_id"))
    //{
//       //cout << "Content-type: text/html\n\n";
  //      //cout << "slj";
	//	return INVALID;
//    }
  //  else
    //{
//        //cout << "Content-type: text/html\n\n";
  //      //cout << "asdgasgslj";
//    }
	query.clear();
    query << "select account_id,session_id,hashid,time_stamp,urlSESSION from sessions where hashid='" << cookieMonster.cookieEntry("account_id") << "'";
//    //cout << "Content-type: text/html\n\n";
  //  //cout << query.str() << endl;
    Result_Set results(query.str());
   //     //cout << "cracka";
    if (database.execute_query(query,results))
    {
//        //cout << "Content-type: text/html\n\n";
        
    	if (results.number_rows() == 0)
        {

		//cout << "Content-type: text/html\n\n" << query.str() << "results " << results.number_rows();
            return NONE; // there was no session for that ipaddress
        }else
	   {
		if (results.number_rows() > 1)
		{
			//cout << "Content-type: text/html\n\n";
	//cout << "shizzle2" << endl;
			archive_sessions(cookieMonster.cookieEntry("account_id"));
			return INVALID;
}
//            //cout << "we have rows";
		    	// this is an invalid session. basically
	   // the username returned does not match
    	// the username already set.
//		cookieMonster.create_cookie("login","true");

	   // same as above. the username does not match
    	// the username in the user's cookie
        if (MD5String((char*)results[1].c_str()) != cookieMonster.cookieEntry("account_id"))
	   {
//        //cout << "yo" << endl;
//cout << "Content-type: text/html\n\n";
	//cout << "shizzle3" << endl;
	   	archive_sessions(cookieMonster.cookieEntry("account_id"));
        	return INVALID;
		}	   	

	   // invalid session because the log id on file
    	// does not match log id in cookie

	   // invalid session because random id on file
    	// does not match log id in cookie

//	   if (MD5String((char*)results[2].c_str()) != cookieMonster.cookieEntry("random_id"))
  //  		return INVALID;

        if (MD5String((char*)results[0].c_str()) != cookieMonster.cookieEntry("session_id") || results[4] != inid)
	   {
        //cout << "yo" << endl;
	   		archive_sessions(cookieMonster.cookieEntry("account_id"));
            return INVALID;
		  }

	   // get current session time stamp
        long session_timestamp = atol (results[3].c_str()) ;

	   // if current session time stamp with expected session length is
    	// less than the current time stamp, the session must be invalid
	
		query.clear();
          query << "select account_id,session_length,group_id,username from users where account_id='" << results[0] << "' and site='" << site << "'";

          results.SetQuery(query.str());
          if (!database.execute_query(query,results))
        {

             return MYSQL_ERROR;
        }
        if (results.number_rows() > 0)
        {
            user.username=results[3];
            user.account_id=results[0];
            user.group_id=results[2];
        }
        else
        {
		//cout << "351" << endl;
	        return INVALID;
        }
		query.clear();
		query << "select status,name, group_id from workgroups where group_id='" << results[2] << "'";
      //  //cout << query.str();

		Result_Set group_check(query.str());
		database.execute_query(query,group_check);
    //    //cout << group_check[0] << " " << group_check.number_rows();
//			         return INVALID;
        if (group_check.number_rows()==0)
{
//cout << "Content-type: text/html\n\n";
	//cout << "shizzle4" << endl;
            return INVALID;
}
		if (group_check[0] == "INACTIVE")
    {
//        //cout << "yo" << endl;
			return NOACTIVE;
        }
    	user.group_name=group_check[1];
//    string wname=group_check[1];
        user.group_hash=get_hash(atoi( group_check[2].c_str() ),user.group_name);


		permission.Load_Permissions(results[2]);

	     if (results.number_rows() == 0)
    		{
//        //cout << "yacco" << endl;
        	   	return NOACCT; // user account does not exist
	     }else{
        	 // get expected session length in minutes; change to seconds
            int session_length = atoi (results[1].c_str()) * 60; // TO GET MINUTES
		  if ((long)time(NULL) >= session_timestamp+session_length)
		  	{

				archive_sessions(cookieMonster.cookieEntry("account_id"));
//        //cout << "y2o" << endl;
				return INVALID;
			}
//            	if ( ( session_timestamp + session_length) < programEssentials->time_stamp() ) return INVALID;

                    //user.session_length=atoi(results[2]);
                
//			myvariables.account_id = atoi( return_result(0) );

//			setvariables(mysql_return_result(0));
            }
        }
        
		query.clear();
		query << "update sessions set time_stamp=" << time(NULL) << " where hashid='" << cookieMonster.cookieEntry("account_id") << "'";
		
		database.execute_query(query);
        return VALID;
    }
//        //cout << "y23525o" << endl;
	return MYSQL_ERROR;

}

// returning an empty string means we have an invalid username/password or workgroup
// is inactive
string
LoginManage::login(string inusername, string inpassword)
{
	

//	//cout << "Content-type: text/html\n\n";
//	//cout << "something" << endl;
	query.clear();
	query << "select account_id,group_id from users where username='" << inusername << "' and password='" << inpassword << "' and site ='"
	<< site << "'";
	Result_Set results;

	results.SetQuery(query.str());
	database.execute_query(query,results);
    if (results.number_rows() == 0 )
        return "";
		user.account_id=results[0];
        user.group_id=results[1];
	query.clear();
	query << "select status,name,group_id from workgroups where group_id='" << results[1] << "'";
	Result_Set group_check(query.str());
	database.execute_query(query,group_check);
//	//cout << "something5" << endl;
	if (group_check[0] == "INACTIVE")
		return "";
	user.group_name=group_check[1];
//    string wname=group_check[1];
    user.group_hash=get_hash(atoi( group_check[2].c_str() ),user.group_name);
	permission.Load_Permissions(results[1]);
	if (results.number_rows()==0)
		return "";
//	//cout << "something5" << endl;

	query.clear();
	query << "select session_id,hashid,urlSESSION from sessions where account_id='" << results[0] << "'";
	////cout << query << endl;
	results.SetQuery(query.str());
//	//cout << "something this way" << endl;
	database.execute_query(query,results);
//	//cout << "something this way comes " << results[2] << endl;
	if (results.number_rows() > 0)
	{
	if ( check_session(results[2]) == VALID)
		{
			return results[2];
		}
		else
		{
			archive_sessions( results[1] );
		}
	}
	archive_sessions_by_acID(user.account_id);
////cout << "something5" << endl;
		string_functions sF;
		string session_id=sF.random_string(15);
	
			

	
//	//cout << "something5" << endl;
	user.session_id=session_id;
	user.hash_id=MD5String((char*)session_id.c_str());
	user.urlSESSION=MD5String((char*)user.hash_id.c_str());
	query.clear();
	query << "insert into sessions (session_id,hashid,urlSESSION,account_id,time_stamp) values ('" << session_id << "','" << user.hash_id << "','" << user.urlSESSION << "','" << user.account_id << "'," << time(NULL) << ")";
	database.execute_query(query);
    cookieMonster.create_cookie("session_id",MD5String((char*)user.account_id.c_str()),"","","");
//    cookieMonster.create_cookie(s,user.hash_id);
    cookieMonster.create_cookie("account_id",user.hash_id,"","","");
	return user.urlSESSION;
		/*
	
	create_query(true,"select last_ip from %S where account_id='%s'",program_setup->system_name,myvariables.account_id);

	if (strlen(mysql_return_result(0)) < 1)
	{
		valid_session = false;
	}
	else
	{
		string lastIP = mysql_return_result(0);
			
		create_query(true,"select ip,log_id from sessions where ip='%s'",lastIP.c_str());
		
		if (get_num_row_results() == 0)
		{
			valid_session = false; // user logged out properly
		}
		else
		{
			valid_session = true;

			string logID = mysql_return_esult(0,1);
		
			create_query(true,"update sessions set ip ='%s',randomid='%s' where log_id='%s'",getenv("REMOTE_ADDR"),randid.c_str(),logID);
/*
			create_query(true,"insert into actions (username,randomid,action_timestamp,script,title,description,ip,site) VALUES('%s','%s','%l','login','Login','User re-logged in with id random userid %s and ip %s..','%s','%s')",inusername,randid.c_str(),programEssentials->time_stamp(),randid.c_str(),lastIP,getenv("REMOTE_ADDR"),program_setup->site);
//
			create_cookie("log_id",mylog_id,"","","");

			fastint = atoi(mylog_id);

		}
	
	}
	

	create_query(true,"update %S set last_visit=NOW(''),last_ip='%s' where account_id='%s'",program_setup->system_name,getenv("REMOTE_ADDR"),myvariables.account_id);

	if (valid_session == false)
	{
		archive_sessions(inusername); // apparently the user has logged in recently. Archive his/her session

		create_query(true,"insert into sessions (account_id,ip,session_timestamp,randomid) VALUES('%s','%s','%l','%s')",myvariables.account_id,getenv("REMOTE_ADDR"),programEssentials->time_stamp(),randid.c_str());

		int new_log_id = insert_id();

		create_query(true,"insert into actions (account_id,randomid,action_timestamp,script,title,description,ip) VALUES('%s','%s','%l','login','Login','user logged in..',"..",'%s')",myvariables.account_id,randid.c_str(),programEssentials->time_stamp(),getenv("REMOTE_ADDR"));

		create_cookie("log_id",new_log_id,"","","");

		return new_log_id;
	}else{ 
		return fastint;
	}	
	*/
}


/*
	deletes the user's sessions
*/




// private functions
void
LoginManage::archive_sessions_by_acID(string accountID)
{

    query.clear();
    query << "select session_id,account_id,hashid from sessions where account_id='" << accountID << "'";
    Result_Set results(query.str()); 
        if (!database.execute_query(query,results)) return;
    query.clear();
    query << "delete from sessions where account_id='" << accountID << "'";
    if (!database.execute_query(query)) return;

    do
	{

        query.clear();
        query << "insert into " << login_variables.value("ARCHIVE_TABLE") << " (session_id,account_id,hashid)"<<        query << " VALUES('" << results[0] << "','" << results[1] << "','" << results[2] << "')";     
        if (!database.execute_query(query)) return;


	}while (results++);
    cookieMonster.create_cookie("session_id","","","","");
//    cookieMonster.create_cookie(s,user.hash_id);
    cookieMonster.create_cookie("account_id","","","","");
}

void
LoginManage::archive_sessions(string hashID)
{

    query.clear();
    query << "select session_id,account_id,hashid from sessions where hashid='" << hashID << "'";
    Result_Set results(query.str()); 
    if (!database.execute_query(query,results)) return;
    query.clear();
    query << "select session_id,account_id,hashid from sessions where account_id='" << results[1] << "'";
    if (!database.execute_query(query,results)) return;
    query.clear();
    query << "delete from sessions where account_id='" << results[1] << "'";
    if (!database.execute_query(query)) return;

    do
	{

        query.clear();
        query << "insert into " << login_variables.value("ARCHIVE_TABLE") << " (session_id,account_id,hashid)"<<        query << " VALUES('" << results[0] << "','" << results[1] << "','" << results[2] << "')";     
        if (!database.execute_query(query)) return;


	}while (results++);
    cookieMonster.create_cookie("session_id","","","","");
//    cookieMonster.create_cookie(s,user.hash_id);
    cookieMonster.create_cookie("account_id","","","","");
}





/* -- previously used for logging actions
	adds an action to the table 'actions'. We also have to make
	sure the number of actions does not exceed program_setup->num_actions_contain

	
	void
LoginManage::get_site(const char *insitevalue)
{
// usage of multiple sites
	if (program_setup->site_functionality == 1)
	{
		if (acceptable_site(insitevalue) == true)
		{
				program_setup->site = insitevalue;
		}
		else
		{
			program_setup->site = program_setup->main_site;
		}

	}
}
	
void
LoginManage::addaction(const char *myid, const char *myscript, const char *mytitle, const char *mytable,const char *mytype, const char *mydescription, const char *myip, int force_undo)
{
	// get the number of current actions
	in num_actions =	count_query("actions where account_id=%s'",myvariables.account_id);
	// if the number of current actions is greater than the maximum
	// allowed, then we must delete some actions
	if (num_actions >= program_setup->num_actions_contain)
	{
		int temp_limit;
		for (int i = num_actions; i >=program_setup->num_actions_contain; i=i-1)
		{
			if (i == program_setup->num_actions_contain)
				temp_limit = 1;
			else
				temp_limit = i-program_setup->num_actions_contain;

			// unfortunately, if actions fall on the same EXACT time, they may be inadvertently deleted.
			// the amount of actions remaining should be checked after each delete, but if actions are
			// on the same time, they are most likely system actions, and not user actions, therefore
			// CAN be deleted.
			// =========================================================================================
			
			create_query(true,"select action_timestamp from actions where account_id='%s' order by action_timestamp asc limit %i",myvariable.account_id,temp_limit);

			create_query(true,"delete from actions where account_id='%s' and action_timestamp='%s'",myvariables.account_id,mysql_return_result(0));


		}
	}
	// insert action
	create_query(true,"insert into actions(account_id,randomid,action_timestamp,title,table,type,description,ip,undo) values('%s','%s','%l','%s','%s','%s','%s','%s','%s','%i')",myvariables.account_id,myid.c_str(),programEssentials->time_stamp(),myscript,mytitle,mytable,mytype,mydescription,getenv("REMOTE_ADDR"),undo);
}


	delete all of the user's actions

void
LoginManage::clearactions()
{
	create_query(true,"delete from actions where account_id='%s'",myvariables.account_id);
}
*/


/* -- previously used for logging actions
void
LoginManage::update_session(const char *inipaddress,const char *scriptname,const char *actiontitle, const char *actiontable,const char *actiontype,const char *actiondescription)
{
	unpdate_session(inipaddress,scriptname,actionttitle, actiontable,actiondescription,0); // default is no undo
}

void
LoginManage::update_session(const char *inipaddress,const char *scriptname,const char *actiontitle, const char *actiontable,const char *actiontype, const char *actiondescription,int undo)
{
	create_query(true,"select username,randomid from sessions where ip='%s'",inipaddress);

	if (get_num_row_results() > 0)
	{
		// there were no results and therefore the user has probably not logged in

		string userName=mysql_return_result(0),userID=mysql_return_result(0,1);

	
	
		create_query(true,"update sessions set session_timestamp='%l' where ip='%s'",programEssentials->time_stamp(),inipaddress);

		addaction(inusername,myid,scriptname,actiontitle,actiontable,actiontype,actiondescription,getenv("REMOTE_ADDR"),undo);

	}
}

bool
LoginManage::acceptable_site(const char *insite)
{
	create_query(true,"select * from allowedsites where site='%s'",programEssentials->lowercase(insite));

	if (get_num_row_results() == 0)
		return false;
// there may not be any point for the following. Unless we want to double check ourselves
// later
	if (strlen(myvariables.account_id) > 0 && strlen(myvariables.password) > 0 && strlen(myvariables.username) > 0)
	{
		create_query(true,"select password from %S where account_id='%s'",program_setup->system_name,myvariables.username,programEssentials->lowercase(insite));

		if (get_num_row_results() == 0)
			return false; // could not find user account on this site
		else
			if (!strcmp(mysql_return_result(0),myvariables.password))
				return true; // valid user account password
			else
				return false;

	}

}
*/
#endif
