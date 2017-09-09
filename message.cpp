/**
* UNFINISHED. SIMPLY ADDS LOG MESSAGE TO TABLE.  THE
* REMAINING CODE, WHICH HAS BEEN SHOWN TO WORK IS AVAILABLE
* IN WORK.CPP AND CAN BE TAILORED TO ONE'S NEEDS. ENCRYPTION
* AND PERMISSIONS ARE USED BELOW. SQLSTRUCT.SQL SHOULD PROVIDE
* A PRELIMINARY IDEA OF THE SQL STRUCTURE. NOTE SOME HAS CHANGED
* AND DUE TO SYSTEM DOWNTIME, THE MOST RECENT SQL DUMPS WERE NOT
* CAPUTURED. HOWEVER, SET ERRORS WHEN CALLING ERROR.H TO SCREEN
* INSTEAD OF TERMINAL
* 		Error(string,string,int,ERROR_TYPE = TERMINAL);
*           -becomes-
*       Error("Type","Name",000,SCREEN);
*
* NOTE: HIDDEN MESSAGES ARE IMPLEMENTED IN WORK.CPP AND SHOULD
* BE EASY TO MIGRATE IN HERE. THEY CAN BE USED AS SUCH <HID>TEXT</HID>
* NOTE, HOWEVER, THAT A WORKGROUP SHOULD BE DEFINED FOR THE HID TEXT
* SEARCHING CAN BE DONE HERE, AS PER SEARCH.H AND THE EXAMPLES IN SEARCH.CPP
**/
#include <iostream>
#include <string>
#define USE_MYSQL 1
#include "cgiparser.h"
#include "cookies.h"
#include "html.h"
#include "dbase.h"
#include "Query.h"
#include "result_set.h"
#include "hidden.h"
#include "rijndael_encryption.h"
#include "permissions.h"
#include "loginmanage.h"
#include "search.h"
#include "ip_restrict.h"
#include "functions.h"

using namespace std;
void view_messages(DB_Handler &);

void add_Post(string,string,DB_Handler &);
void display_dle(string,string,DB_Handler &);

	cgiparser cgi_utilities;
    LoginManage login;

int main()
{
    if (! check_ip() ) return 0;

    retreive_values variables;
    variables.parse_file(DATFILE);

    DB_Handler database;
    database.create_connection(variables.value("db_login"),variables.value("db_password"),variables.value("database"));

    if (cgi_utilities.METHOD()==GET )
    {
		if (cgi_utilities.cgivalue("SESSIONID") == "")
		{
            login.logout();
			display_login("Please Log in");
		}else
		{
           if (login.check_session(cgi_utilities.cgivalue("SESSIONID"))==VALID)
            {
                        view_messages(database);                
                   
            }
           else
            {
                        login.logout();
			            display_login("You have been logged out");
            }

	  }
	  
    }
    
    
    return 0;
}


void
view_messages(DB_Handler &database)
{
        html html_controls;
        Query query;
            rijndael_encryption encryptor(login.group_key());
        int start= atoi ( cgi_utilities.cgivalue("list").c_str() ),num=atoi ( cgi_utilities.cgivalue("num").c_str() );
        if (num==0) num=10;
        
        if (login.permissions.permission("view_all_groups"))
        {
            if (cgi_utilities.cgi_value("restrict")=="group")
                query << "select title,caption from `logs` where gid='" << login.user.group_id << "' ";
            else if (cgi_utilities.cgi_value("restrict")=="account")
                query << "select title,caption from `logs` where account_id='" << login.user.account_id << "' ";  
            else
                query << "select title,caption from `logs` ";
        }
        else
        {
            if (cgi_utilities.cgi_value("restrict")=="user")
                query << "select title,caption from `logs` where account_id='" << login.user.account_id << "' ";
            else
                query << "select title,caption from `logs` where gid='" << login.user.group_id << "' ";                        

            if (cgi_utilities.form_data("range_begin").length() > 0)
              query << " and time_stamp > '" << cgi_utilities.form_data("range_begin") << "' ";

            if (cgi_utilities.form_data("range_end").length() > 0)
              query << " and time_stamp < '" << cgi_utilities.form_data("range_end") << "' ";
       
        }



        if (cgi_utilities.cgivalue("OB")=="caption")
          query << " ORDER BY caption";
        else
            query << " ORDER BY title";
        
        if (cgi_utilities.cgivalue("direction")=="up")
          query << " ASC";
        else
            query << " DESC ";

        query    << " LIMIT " << start << "," << num;
        Result_Set results(query.str());
        string list="";
        database.execute_query(query,results);
        html_controls.myTable.SetTable(1,results.number_rows()*3);
        if (results.number_rows() > 0)
        int i=-1;
        do
        {
                /// MYTABLE ROUTINE IS CURRENTLY BEING TESTED.      
                html_controls.myTable[0][++i]=results[0];
                string ms=cc_decrypt(results[1]);
                string us="";
                us=encryptor.decrypt_text(ms);
                html_controls.myTable[0][++i]=us;
                html_controls.myTable[0][++i]="&nbsp;<br>&nbsp;";

    //        string ml
                
/*
                list+="<div align=center><table><tr><td width=295><p></p></td><td width=295><p><o:p>&nbsp;</o:p></p></td></tr><tr><p>";
                list+=results[0];
                list+="</p></td><td width=295 ><p><o:p>&nbsp;</o:p></p></td></tr><tr><p>";
                string ms=cc_decrypt(results[1]);
                string us="";
                us=encryptor.decrypt_text(ms);
                list+=us;
                list+="</p></td></tr></table></div>";
*/
        
        }while(results++);
        
        html_controls.html_value("urlsess",cgi_utilities.cgivalue("SESSIONID"));
    	html_controls.html_value("list",html_controls.myTable.completeTable());

        html_controls.parsePage(MESSAGE_LIST);
}
