#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#define LOGIN_LOCATION "/home/marc/public_html/login.html"
#define MAIN_PANEL "/home/marc/public_html/main.html"
#define DATFILE "/home/marc/data_files/general.dat"
#define MESSAGE_LIST "/home/marc/public_html/list.htm"
#include <sys/sysinfo.h>
void
add_bottom(html &html_controls)
{
    struct sysinfo test;
    sysinfo (&test);
    int minutes = (test.uptime/60)%60;
    html_controls.html_value("minutes",minutes);
    int seconds = (test.uptime%60);
    html_controls.html_value("seconds",seconds);
    int hours = (test.uptime/3600)%24;
    html_controls.html_value("hours",hours);
    int days = (test.uptime/(3600*24)%365);
    html_controls.html_value("days",days);
    html_controls.html_value("total_ram",test.totalram/(1024*1024));
    html_controls.html_value("avail_ram",(test.freeram+test.sharedram+test.bufferram)/(1024*1024));
}
void
display_login(string message)
{
    html html_controls;
	retreive_values variables;
    variables.parse_file(DATFILE);
	
	DB_Handler database;
	
	database.create_connection(variables.value("db_login"),variables.value("db_password"),variables.value("database"));
											
	Result_Set results;
	
	Query query;
	
	
	
	query << "select kid,sid from `keys` order by rand() limit 0,1";
	
	
	
	if (database.execute_query(query,results))
	{
		if (results.number_rows() > 0)
		{
			html_controls.html_value("special",results[0]);
			html_controls.html_value("sid",results[1]);
		}
		else
			html_controls.html_value("special","NAN");
	}
	else
		html_controls.html_value("special","NAN");
	
    html_controls.html_value("message",message);
	add_bottom(html_controls);
 
    html_controls.parsePage(LOGIN_LOCATION);
}



void
main_panel(string sessionid,LoginManage &login, retreive_values &variables)
{
	
    html html_controls;
    html_controls.html_value("urlsess",sessionid);
	DB_Handler database;
	
	database.create_connection(variables.value("db_login"),variables.value("db_password"),variables.value("database"));
											
	Result_Set results;
	
	Query query;
	
	permissions permission( login.user.group_id );
	
	query << "select active,mod_id,mod_url,mod_cgi,mod_name,mod_image,mod_command from modules";
	
	vector< string > names;
	

	if (database.execute_query(query,results))
	{
		if (results.number_rows() > 0)
		{

			string module_table = "<table width = \"500\">";
			string link = "";
			char in_row = 0x00;
			int mod_count=0;
			do
			{
				

				if (results[0] != "1")
					continue;

				if (permission.permission( results[6] ) == false)
					continue;

				if (mod_count==0)
					module_table+="<tr>";
				
				module_table+="<td width=\"50%\"><center>";
				
				link = "<a href=\"/" + results[3] + "/";
				link+= sessionid + "/" + results[2];
				link +="\">";
				module_table +=link;
				module_table += "<img src=\"";
				module_table += results[5];
				module_table +="\" border =\"0\">";
				module_table +="</a>";
				
				string nameadd = link;
				nameadd+=results[4];
				nameadd+="</a>";
				names.push_back( nameadd );
				//module_table+=sessionid;
				
				if ((++mod_count % 2) == 0)
				{
					mod_count = 0;
					module_table +="<tr>";
					for (int j=0; j < 2; j++)
					{
						string name = names.at(j);
						module_table+="<td width=\"50%\"><center>";
						module_table+=name;
						module_table+="</center></td>";
					}
					names.clear();
					names.resize(0);
					module_table +="</tr>";
				}
				
				
				
				
			} while (results++ );
			
			if (mod_count > 0)
			{
				for (int j=mod_count; j < 2; j++)
				{
					//string name = names.pop_front();
					module_table+="<td><img src=\"/images/efs/blank.gif\" /></td>";
				}
				module_table+="</tr><tr>";
				for (int j=0; j < 4; j++)
				{
					string name = "    ";
					if ( j < names.size())
						name = names.at(j);
					module_table+="<td width=\"50%\"><center>";
					module_table+=name;
					module_table+="</center></td>";
				}
				module_table+="</tr>";
			}
			
			module_table += "</table>";
			
			html_controls.html_value("panel",module_table);			
						
			html_controls.parsePage(MAIN_PANEL);		
		}
		else
			cout << "could not load modules";
	
	}
	else cout << "could not load modules";
		
	/*
       if (login.permission.permission("show_admin_panel"))
            html_controls.html_value("admin","System Settings");
	else
	    html_controls.html_value("admin","Account Settings");
	*/
	
        // add something else above if you wish to use images or links for the admin panel
       
	
	
}

bool
check_ip()
{
    ip_restrict ip_check;
    if (!ip_check.test(getEnv("REMOTE_ADDR")))
    {
        html html_controls;
        html_controls.htmlerror("Fatal Error: IP Address beyond range",111);
        return false;
    }
    return true;
}

#endif
