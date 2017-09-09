#ifndef MAIN_H
#define MAIN_H
#define SETTINGS_MAIN "/home/phrocker/public_html/mm/account_settings.html"
#define MESSAGE_MAIN "/home/phrocker/public_html/mm/message_main.html"
#include <fstream>
#include "string_functions.h"
using namespace std;
void
settings(string sessionid,LoginManage &login)
{
	
       html html_controls;
       html_controls.html_value("urlsess",sessionid);
       if (login.permission.permission("ip_restrictions"))
       {
       		if (cgi_utilities.METHOD()==POST)
		{
			ofstream write("/home/phrocker/data_files/iplist.dat");
			if (write)
			{
			write<<cgi_utilities.form_data("ip_1_1") << " ";
			write<<cgi_utilities.form_data("ip_1_2") << " ";
			write<<cgi_utilities.form_data("ip_2_1") << " ";
			write<<cgi_utilities.form_data("ip_2_2") << " ";
			write<<cgi_utilities.form_data("ip_3_1") << " ";
			write<<cgi_utilities.form_data("ip_3_2") << " ";
			write<<cgi_utilities.form_data("ip_4_1") << " ";
			write<<cgi_utilities.form_data("ip_4_2") << " ";
			write.close();
			html_controls.html_value("message","IP LIST CHANGED");
			}
			
		}
		else
			html_controls.html_value("message","IP RESTRICTION LIST");
       	html_controls.html_value("submit","<input type=\"submit\" name=submit>");
		ifstream load("/home/phrocker/data_files/iplist.dat");
		string LOAD="";
		if (load)
		{
			load>>LOAD;
				html_controls.html_value("ip_1_1",LOAD);
			load>>LOAD;
				html_controls.html_value("ip_1_2",LOAD);
			load>>LOAD;
				html_controls.html_value("ip_2_1",LOAD);
			load>>LOAD;
				html_controls.html_value("ip_2_2",LOAD);
			load>>LOAD;
				html_controls.html_value("ip_3_1",LOAD);
			load>>LOAD;
				html_controls.html_value("ip_3_2",LOAD);
			load>>LOAD;
				html_controls.html_value("ip_4_1",LOAD);
			load>>LOAD;
				html_controls.html_value("ip_4_2",LOAD);
		load.close();	
		}
		
	}
	else
	{
	html_controls.html_value("message","See those blank fields below?<br>It means you don't have permission!");
	}
        // add something else above if you wish to use images or links for the admin panel
       html_controls.parsePage(SETTINGS_MAIN);
}

void
message_main(string sessionid,LoginManage &login,string action, string id, retreive_values& variables)
{
	
	if (! login.permission.permission("message_system"))
	{
		main_panel(cgi_utilities.cgivalue("SESSIONID"),login,variables);
		return;
	}
       Query query;
       DB_Handler database;       
    //          retreive_values variables;
    //variables.parse_file(DATFILE);

           database.create_connection(variables.value("db_login"),variables.value("db_password"),variables.value("database"),variables.value("host"));   

        if (action=="ADD")
       {
       		if (login.user_exist( cgi_utilities.form_data("recipient") ))
		{
		string_functions sF;
		string session_id=sF.random_string(15);
			query << "insert into messages (mess_id,sender_name,recipient_acc,subject,message) values('" << session_id << "','" << login.user.username << "','" << cgi_utilities.form_data("recipient") << "','" << cgi_utilities.form_data("subject") << "','" << cgi_utilities.form_data("text") << "')";
//	cout << "Content-type: text/html\n\n";
//	cout << action << " " << id << " " << query.str() << endl;	
       			database.execute_query(query);
		}
       }
              html html_controls;
       html_controls.html_value("urlsess",sessionid);



        query.clear();
    	query << "select username from users";
	Result_Set results1(query.str());
	database.execute_query(query,results1);
	string users="";
	if (results1.number_rows() > 0)
	{
		do
		{
		users+= results1[0] + "<br>";
		}while(results1++);
	}
	html_controls.html_value("users",users);
       if (action=="DELETE" && id.length() >0)
       {
       	query.clear();
       	query << "delete from messages where mess_id='" << id << "'";
//	cout << "Content-type: text/html\n\n";
//	cout << action << " " << id << " " << query.str() << endl;	
       	database.execute_query(query);
       }
       
      
	
       query.clear();
       query << "select mess_id,subject,sender_name from messages where recipient_acc='" << login.user.username << "'";
       Result_Set results(query.str());
   //     cout << "cracka";
      //DB_Handler database;
      
    
   stringstream list;
   list << "";
    	if (database.execute_query(query,results))
   	{
	list << "<table>";
		if (results.number_rows() > 0)
		{
			do
			{
			list << "<tr><td>" << results[2] << "</td><td>" << results[1] << "</td><td>VIEW</td><td><a href=\"http://phrocker.no-ip.info:8080/cgi-bin/mm.cgi?SESSIONID=" << sessionid << "&action=messages&MESS_ACTION=DELETE&MESS_ID=" << results[0] << "\">DELETE</a></td></tr>";
			} while (results++);
		}
	        // add something else above if you wish to use images or links for the admin panel
		list << "</table>";
       	
	}
	if (list.str().length() ==0)
		list << "No messages";
	html_controls.html_value("list",list.str());
	html_controls.parsePage(MESSAGE_MAIN);

}
#endif
