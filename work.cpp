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
using namespace std;
void view_messages(DB_Handler &);
void show_message(string,DB_Handler &);
void add_Post(string,string,DB_Handler &);
void display_dle(string,string,DB_Handler &);
void display_login(string);
	cgiparser cgi_utilities;
    LoginManage login;
int main()
{
  //  html html_controls;

//    display_dle("marc","lskjdflsdj",database);
  //  return 0;

    ip_restrict ip_check;
    if (!ip_range.test(getEnv("REMOTE_ADDR")))
    {
        html html_controls;
   //     html_controls.dump_core("Fatal Error: IP Address beyond allowed range",1);
        html_controls.htmlerror("Fatal Error: IP Address beyond range",1);
        return 0;
    }
    DB_Handler database;
    database.create_connection("root","","test");
    //permissions permission("1");
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
                    html html_controls;
                    html_controls.html_value("urlsess",cgi_utilities.cgivalue("SESSIONID"));
                    html_controls.parsePage("/home/marc/public_html/work/src/mid.html");
            }
           else
            {
                        login.logout();
			            display_login("You have been logged out");
            }

	  }
	  
    }
    
    else
    {
    	
   		if (cgi_utilities.cgivalue("action")=="LOGIN")
		{
            string usl =login.login(cgi_utilities.form_data("username"),cgi_utilities.form_data("password"));
            if (usl.length()==0)
            display_login("Please log in with proper credentials");
            else
            {
                        html html_controls;
                    html_controls.html_value("urlsess",usl);
                    html_controls.parsePage("/home/marc/public_html/work/src/mid.html");
            }
		}			
        else
		{

            if (login.check_session(cgi_utilities.cgivalue("SESSIONID"))!=VALID)
            {
                        login.logout(cgi_utilities.cgivalue("SESSIONID"));
			            display_login("You have been logged out");
            }
            else
            {
                if (cgi_utilities.cgivalue("action")=="NewLog")
                {
                     if( !login.permission.permission("add_new"))
                    {
                            html html_controls;
                            html_controls.html_value("urlsess",cgi_utilities.cgivalue("SESSIONID"));
                            html_controls.parsePage("/home/marc/public_html/work/src/mid.html");
    
                    }
                    else
                        if (cgi_utilities.cgivalue("tpsReport")=="B")
                            add_Post(login.user.username,"",database);
                        else
                            display_dle(login.user.username,"",database);
                }
                else
                if (cgi_utilities.cgivalue("action")=="view")
                {
                    if (cgi_utilities.cgivalue("view")=="2")
                        view_messages(database);
                
                }
            }

		}		

    }
    return 0;
/*
	string d="dd";
	cout << "Content-type: text/html\n\n";
	cookies cookieMonster;
	cout << "cookie ist" <<  cookieMonster.cookieEntry("login") << endl;

	if (cgi_utilities.cgivalue("action")=="login")
	{
        	verify_login(cgi_utilities);
	}
    	else
    	{
		verify_login(cgi_utilities);
    	}
    return 0; 
*/   
}

void
display_dle(string username,string workgroup,DB_Handler &database)
{




    html html_controls; 
//    cout << "Content-type: text/html\n\n";
///        int selectme=0;


    string selectme;

selectme="selected=true";
    time_t rawtime;
	struct tm *rC;
  time ( &rawtime );
  rC = localtime ( &rawtime );
  

    if (rC->tm_hour >= 7 &&  rC->tm_hour < 15)
        html_controls.html_value("1select",selectme);
    else if (rC->tm_hour >= 15 &&  rC->tm_hour < 23)
        html_controls.html_value("2select",selectme);
    else
        html_controls.html_value("3select",selectme);

//        cout << "username is " << username << " ime" << rC->tm_hour;

      html_controls.html_value("user",username);
      html_controls.html_value("workgroup",workgroup);

//        exit(1);
 //   cout << "waht about right here? ";
        Query query;
        query << "select title,caption,rid from logs";
        Result_Set results(query.str());
        int i=0;
//    cout << "waht about right here? ";
        if (database.execute_query(query,results))
        {
   rijndael_encryption encryptor("jsn39ghalq01957mbnsk3067hgnzadje");
            string list;
            list="<p align=center>";
            if (results.number_rows() > 0 )
            {
   // cout << "waht about right here? ";
            do
            {
            i++;
                list += results[0];
		
                list+= "<br>";
                string ms=cc_decrypt(results[1]);
                string us="";
                Query qry;
                bool dod=true;
                us=encryptor.decrypt_text(ms);

                if (results[2] != "") // && login.permission.permission("viewhidden"))
                {    

                    qry << "select text from hidText where rid='" << results[2] << "'";
                    Result_Set result(qry.str());
                    database.execute_query(qry,result);

                    if (result.number_rows() > 0)
                    {
                        hide_text hid;
                        hid.storing.resize(result.number_rows());
                        for (int i=0; i < hid.storing.size(); i++,result++)
                        {
                            string mic;
//                          mic="";
//                            mic=;
                            mic=cc_decrypt(result[0]);
                            string mmic = encryptor.decrypt_text(mic);
                            hid.storing[i]=mmic;
                        }

                            hid.cnt=result.number_rows();
                        us=hid.fill(us);

                
                    }
                }
           

                list+=us;
//                list += results[1];
                list+="<br><br>";
                if (i==10)
                {
    //cout << "waht about right here? ";
  //                  cout << "yepper" << list;
//                   exit(1);
                }
            }while (results++);
  //  cout << "waht about right here? ";
//	     exit(1);
            }
            list+="</p>";
//    cout << "waht about right here? " << list;
            html_controls.html_value("list",list);

        }
//            cout << "what about this? " << cgi_utilities.cgivalue("SESSIONID");
            html_controls.html_value("urlsess",cgi_utilities.cgivalue("SESSIONID"));	   
       html_controls.parsePage("/home/marc/public_html/work/src/Title.htm");
}

void
add_Post(string username,string workgroup,DB_Handler &database)
{
  html html_controls;
        rijndael_encryption encryptor("jsn39ghalq01957mbnsk3067hgnzadje");
        Query query;
        string_functions sf;
        string messid=sf.random_string(15);
        string text=cgi_utilities.form_data("caption");
        if ( text.length() > 0)
        {
            string ttext=text;
            Search sef;
    //        cout << text << endl;
            sef.AddKeyWords(cgi_utilities.form_data("caption"),messid);
            //cout << text << endl;

            hide_text hid;
            hid.find_encrypted(text);  
      
            string rid;
            if (hid.storing.size() > 0 )
                rid=sf.random_string(20);
            else
                rid="";

            query << "insert into `logs` (title,caption,rid,messid,account_id) values ('" << cgi_utilities.form_data("title") << "','" <<   cc_encrypt(encryptor.encrypt_text(text)) << "','" << rid << "','" << messid << "','" << login.user.account_id << "');";
            database.execute_query(query);
            unsigned long did = database.insert_ID();
            for (int i=0; i <hid.storing.size(); i++)
            {
                query.clear();  
//            string std

                query << "insert into `hidText` (rid,number,text) values('" << rid << "'," << i << ",'" <<             cc_encrypt(encryptor.encrypt_text(hid.storing[i])) << "');";
//                query << "insert into hidText (rid,number,text) values('" << rid << "'," << i << ",'" << hid.storing[i] << "')";
    //            cout << query.str();
                database.execute_query(query);
              }
        }
        show_message(messid,database);
    //        html_controls.html_value("user",cgi_utilities.cgivalue("user"));
  //    html_controls.html_value("workgroup",cgi_utilities.cgivalue("workgroup"));
        
//       html_controls.parsePage("/home/marc/public_html/Title.htm");
}

void
show_message(string did,DB_Handler &database)
{

          html html_controls(false);
        Query query;

        query << "select title,caption from logs where messid='" << did << "';";
        Result_Set results(query.str());
	   
        if (database.execute_query(query,results))
        {
        string title;
            rijndael_encryption encryptor("jsn39ghalq01957mbnsk3067hgnzadje");
            string list;
            list="<p align=center>";
        
            do
            {
                title = results[0];
		
                string ms=cc_decrypt(results[1]);
                string us="";
                Query qry;
                bool dod=true;
                us=encryptor.decrypt_text(ms);

                if (results[2] != "") // && login.permission.permission("viewhidden"))
                {    

                    qry << "select text from hidText where rid='" << results[2] << "'";
                    Result_Set result(qry.str());
                    database.execute_query(qry,result);

                    if (result.number_rows() > 0)
                    {
                        hide_text hid;
                        hid.storing.resize(result.number_rows());
                        for (int i=0; i < hid.storing.size(); i++,result++)
                        {
                            string mic;
//                          mic="";
//                            mic=;
                            mic=cc_decrypt(result[0]);
                            string mmic = encryptor.decrypt_text(mic);
                            hid.storing[i]=mmic;
                        }

                            hid.cnt=result.number_rows();
                        us=hid.fill(us);

                
                    }
                }

                list+=us;
//                list += results[1];
            }while (results++);

            list+="</p>";
            
            html_controls.html_value("title",title);        
            html_controls.html_value("caption",list);
        }

       html_controls.html_value("urlsess",cgi_utilities.cgivalue("SESSIONID"));

       html_controls.parsePage("/home/marc/public_html/work/src/read.htm");

        
}

void
view_messages(DB_Handler &database)
{
        html html_controls;
        Query query;
            rijndael_encryption encryptor("jsn39ghalq01957mbnsk3067hgnzadje");
        int start= atoi ( cgi_utilities.cgivalue("list").c_str() ),num=atoi ( cgi_utilities.cgivalue("num").c_str() );
        if (num==0) num=10;
        
        query << "select title,caption from `logs` where account_id='" << login.user.account_id << "' ";

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
        if (results.number_rows() > 0)
        do
        {
    //        string ml
                list+="<div align=center><table><tr><td width=295><p></p></td><td width=295><p><o:p>&nbsp;</o:p></p></td></tr><tr><p>";
                list+=results[0];
                list+="</p></td><td width=295 ><p><o:p>&nbsp;</o:p></p></td></tr><tr><p>";
                string ms=cc_decrypt(results[1]);
                string us="";
                us=encryptor.decrypt_text(ms);
                list+=us;
                list+="</p></td></tr></table></div>";
        }while(results++);
	          html_controls.html_value("urlsess",cgi_utilities.cgivalue("SESSIONID"));
	          html_controls.html_value("list",list);

       html_controls.parsePage("/home/marc/public_html/work/src/list.htm");
}
