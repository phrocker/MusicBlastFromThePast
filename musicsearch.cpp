#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#define USE_MYSQL 1
#include "cgiparser.h"
#include "cookies.h"
#include "html.h"
#include "loginmanage.h"
#include "ip_restrict.h"
#include "string_functions.h"
using namespace std;

#include "functions.h"
#include "MetaPhone.h"
#include "md5.h"
#include "spellcheck.h"
#define DATA "/home/marc/data_files/music.dat"
cgiparser cgi_utilities;	
LoginManage login;
#include "mm.h"
retreive_values variables;

void searchmain()
{
	html html_controls;
	html_controls.html_value("searchtype",cgi_utilities.cgivalue("searchtype"));
	html_controls.html_value("urlsess",cgi_utilities.cgivalue("SESSIONID"));
	string mainTable = "";
	    DB_Handler database;
	database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"));
	Query query;
                query << "select size,downloads from statistics where id=1";
 	Result_Set resultss(query.str());
	            if (database.execute_query(query,resultss))
	            {
                  int tot=atoi(resultss[0].c_str());
                  char *d = new char[255];
                  sprintf(d,"%i downloads wasting %i MB of bandwidth",atoi(resultss[1].c_str()),tot/(1024*1024));
                  string totals = d;
                  delete [] d;
                html_controls.html_value("totals",totals);	                   
	            }   
	string songTable = "Search by Song name<br><table width=\"45%\">";
	for (int i=0x41; i <=0x5a; i++)
	{
		songTable +="<tr>";
		int jj=0;
		for (jj=0; jj < 5 && (i+jj)<=0x5a; jj++)
		{
			char weNeed=(char)(i+jj);
			songTable+="<td><a href=\"http://www.binarystream.net/music/" + cgi_utilities.cgivalue("SESSIONID");
			songTable+="/begin=0&searchWord=";
			songTable+=  weNeed;
			songTable += "&searchType=song\">";
			songTable += weNeed;
			songTable += "</a></td>";
		}
		songTable+="</tr>";
		i+=jj;
	}
	songTable+="</table>";
	html_controls.html_value("songTable",songTable);
	html_controls.parsePage( variables.value("searchmain").c_str() );
}

string suggestion="";

bool misspell=false;

string searchresults;

string
&musicresults(string searchterm,bool sli=false,int begin=0,string searchType="")
{
    if (searchterm.length() ==0)
	{
		searchresults = "No Results";
		return searchresults;
	}
	spellChecker spellcheck;


    DB_Handler database;
	database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"));
	Query query;
	Result_Set results(query.str());
//	cout << "Content-type: text/html\n\n";

		query << "select song,artist,id from music where ";
         //filename like '%" << searchterm << "%' LIMIT " << begin << ",31";
         
        char *nm=new char[searchterm.length()+1];
	   sprintf(nm,"%s",searchterm.c_str());	
    	char *pch;
    	pch = strtok(nm," ");
	   int i=0;
//	   cout << variables.value("db_user") << " " << variables.value("database");
    	while(pch!=NULL)
    	{
		             string pc=pch;
		     //        cout << pc << endl;
                     if (!spellcheck.check(pc))
                     {
                        misspell=true;
                        
                        suggestion+="<b><em> " + spellcheck.singleSuggest + " </em></b>";
                 }else
                    suggestion+=" " + pc;
                     if (i > 0) query << " and ";  
		if (searchType=="song")
			query << "song like '" << database.escape(pch) << "%' or song like ' " << database.escape(pch) << "%'";
		else
  	          query << "filename like '%" << database.escape(pch) << "%'";
          
            i++;
		pch=strtok(NULL, " ");
	}
	delete [] nm;
	
	   query << " LIMIT " << begin << ",31";
    
			if (database.execute_query(query,results))
			{

				if (results.number_rows() > 0)
				{
					int i=-1;
					searchresults+="<form method=\"POST\" action=\""  + variables.value("domain")+ "/music/" + cgi_utilities.cgivalue("SESSIONID")+"/action=stream\">";
					
					char sj=1;
					do
					{
	               
						if (++i > 30) break;
						char *j = new char[4];
						sprintf(j,"%i",sj);
						searchresults += "<input type=\"checkbox\" name=\"";
						searchresults += j;
						searchresults += "\" value=\"";
						searchresults += results[2];
						searchresults += "\"><a href=\"" +variables.value("domain")+ "/music/" + cgi_utilities.cgivalue("SESSIONID")+"/key="+results[2]+"\" target=\"_blank\">"+results[1] +"-- "+results[0] + "</a><br>";
						delete [] j;
						sj++;
					} while (results++);
					searchresults+="<input type=\"hidden\" name=\"max\" value=\"";
					char *k = new char[4];
					sprintf(k,"%i",sj-1);
					searchresults+= k;
					delete [] k;
					searchresults+= "\">";
					searchresults+="<input type=\"hidden\" name=\"search\" value=\"" + searchterm +  "\"><input type=\"hidden\" name=\"begin\" value=\"";
					char bgn[25];
					sprintf(bgn,"%i",begin);
searchresults+=bgn;
searchresults += "\">";
					searchresults+="<input type=\"submit\" name=\"type\" value=\"Stream Selected\"><input type=\"submit\" name=\"type\" value=\"Add to Queue\"></form><center>";
 		     if (begin > 30)
					{
					searchresults+="<form method=\"POST\" action=\"" +variables.value("domain")+ "/music/"+cgi_utilities.cgivalue("SESSIONID")+"/&searchWord=" +searchterm.c_str();
                    char *d = new char[5];
                    sprintf(d,"%i",begin-31);
                    string ds=d;
                    searchresults  += "&begin="+ ds + "&searchType=" + searchType +  "\"><input type=\"hidden\" name=\"search\" value=\"" + searchterm.c_str() + "\"><input type=\"submit\" name=\"previous 30\" value=\"previous 30\"></form>";
                    delete [] d;
					}
					if (results.number_rows() > 30)
					{
					searchresults+="<form method=\"POST\" action=\"" +variables.value("domain")+ "/music/"+cgi_utilities.cgivalue("SESSIONID")+"/&searchWord=" +searchterm.c_str();
                    char *d = new char[5];
                    sprintf(d,"%i",begin+31);
                    string ds=d;
                    searchresults  += "&begin="+ ds + "&searchType=" + searchType + "\"><input type=\"hidden\" name=\"search\" value=\"" + searchterm.c_str() + "\"><input type=\"submit\" name=\"next 30\" value=\"next 30\"></form>";
                    delete [] d;
					}
					searchresults+="</center>";
				
				}else searchresults="No results found";
			}else searchresults="No results found";
	return searchresults;
}    

int main()
{
 if (! check_ip() ) return 0;
    
 variables.parse_file(DATA);
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
	            
				permissions permission( login.user.group_id );
				if (permission.permission("view_music") == false)
				{
					login.logout();
					display_login("Please Log in");
					return 0;
				}
			
			if (cgi_utilities.cgivalue("key")!="")
			{
				Query query;
				Result_Set results(query.str());
				DB_Handler database;
				database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"));
				query << "select location,filename,filesize,artist,song,downloads from music where id='" << cgi_utilities.cgivalue("key") << "'";
				if (database.execute_query(query,results))
				{
					if (cgi_utilities.cgivalue("DOWNLOAD")=="true")
					{
					    query.clear();
						
					    query << "update music set downloads='" << atoi( results[5].c_str() )+1 << "' where id=" << cgi_utilities.cgivalue("key") << "";
	                    database.execute_query(query);
					char *buffer;

//					cout << "Content-type: audio/mpeg\n\n";
					if (cgi_utilities.cgivalue("stream")=="true")
					{
						//string streamData="Hello world";
						// create a timestamp that allows one hour of streaming
						// music for that song
					//	cout << "Content-type: text/html\n\n";
						query.clear();
						string_functions sf;
						string randi=sf.random_string(45);
						query << "insert into stream (`queueNum`,`account_id`,`streamID`,`IP`,`timestamp`,`key`,`location`,`filename`,`filesize`) VALUES(-1,'" << login.user.account_id << "','" << randi << "','" << getEnv("REMOTE_ADDR") << "','" << time(NULL)+3600 << "'," << cgi_utilities.cgivalue("key") << ",'" << results[0] << "','" << results[1] << "'," << results[2] << ")";
						database.execute_query(query);
						string streamData="#EXTM3U\n#EXTINF: ," + results[3] + "-" +results[4] + "[ Top 40 - 02:01 / 1.8Mb ]\n";
						streamData += variables.value("domain")+ "stream/" + randi;
						
						cout << "Content-type: application/octet-stream\n";
						cout << "Content-Length: " << streamData.length() << "\n";
						cout << "Content-Disposition: attachment ; filename=stream.m3u\n\n";	
						
						//cout << streamData << endl;
						
						cout.write(streamData.c_str(),streamData.length());
					}
					else
					{
					cout << "Content-type: application/octet-stream\n";
					if (cgi_utilities.cgivalue("clip")=="true")				
                        cout << "Content-Length: " << atoi(results[2].c_str())/8 << "\n";
                    else
					cout << "Content-Length: " << results[2] << "\n";
					cout << "Content-Disposition: attachment ; filename= " << results[1] << "\n\n";
	                string mloc = results[0];
                    unsigned int filesz = atoi ( results[2].c_str() );
					ifstream charlie;
					string ilename = results[0]+results[1];
					charlie.open(ilename.c_str(), ios::in|ios::binary|ios::ate );
					long size=0;
					   size = charlie.tellg();
                    if (cgi_utilities.cgivalue("clip")=="true")	
					   size=size/8;

					//while (charlie)
					//	cout << charlie;
					int div = size/8; // eights
					buffer= new char[div];
					//char buffer[size];
                    charlie.seekg(0);
                    
  				for (int i =0; i<= size; i+=div)
                    {
   				          if (login.check_session(cgi_utilities.cgivalue("SESSIONID"))!=VALID)
   				          {
			                 cout << '\0';
   				             break;				
   				         }    
         				if ((size-i) < div)
         				{
         				charlie.read(buffer,(size-i));
         					cout.write(buffer,(size-i));
         					break;
                         }  				
         				else
         				{
    				       charlie.read(buffer,div);
    				       	cout.write(buffer,div);
    				    }
					    


				
                	}	
                     			
					charlie.close();
				
   					    query.clear();
					    query << "select downloads,size from statistics where id=1";
	                    database.execute_query(query,results);
   					    query.clear();
					    query << "update statistics set size=" << atoi( results[1].c_str() )+filesz << ",downloads=" << atoi(results[0].c_str())+1 << " where id =1";
         	                    database.execute_query(query);
}
//					delete [] buffer;
					}
					else
					{
						html html_controls; 
						html_controls.html_value("urlsess",cgi_utilities.cgivalue("SESSIONID"));
						html_controls.html_value("filename",results[1]);
						char fsize[10];
						sprintf(fsize,"%.2f MB",atof(results[2].c_str())/(1024*1024));
			
						html_controls.html_value("filesize",fsize);
					
						html_controls.html_value("artist",results[3]);
	 					html_controls.html_value("song",results[4]);
						html_controls.html_value("key",cgi_utilities.cgivalue("key"));
						
						if (permission.permission("edit_song") == true)
						{
							string edit_table = "<table width=\"60%\"><tr><td width=\"50%\">";
							edit_table += "Artist</td><td width=\"50%\"><input type=\"text\" name=\"artist\" value=\"";
							edit_table += results[3];
							edit_table +="\"></td></tr><tr><td width=\"50%\">Song</td><td width=\"50%\"><input type=\"text\" name=\"song\" value=\"";
							edit_table +=results[4];
							edit_table +="\"></td></tr><tr><td width=\"50%\"> </td><td width=\"50%\"><input type=\"submit\" name=\"edit\" value=\"change\"></td></tr></table>";
							html_controls.html_value("admin_form",edit_table);
							// $(admin_form)
						}
						
						html_controls.parsePage(variables.value("musicfound").c_str());
					}
				}
				else searchmain();
			}
			else if (cgi_utilities.cgivalue("action")=="stations")
			{
				long timestamp = time(NULL)+7200;
				
				Query query;
		            		 DB_Handler database;
					database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"));
					string_functions sf;
					string randi=sf.random_string(45);
					query.clear();
					query << "insert into stream (`streamID`,`IP`,`timestamp`,`key`,`location`,`filename`,`filesize`) VALUES('" << randi << "','" << getEnv("REMOTE_ADDR") << "','" << timestamp << "',132,'0','0',0)";
						//		            query << "select streamID from stream where account_id='" << login.user.account_id << "' and queueNum >0";
					if (!database.execute_query(query))
					{
						cout << "Content-type: text/html\n\n";
						cout << "error with: " << query.str();
					}
					string streamData="";
		            //			            query << "insert into stream (`streamID`,`IP`,`timestamp`,`key`,`location`,`filename`,`filesize`) VALUES('" << randi << "','" << getEnv("REMOTE_ADDR") << "','" << timestamp << "'," << cgi_utilities.form_data(cp) << ",'" << database.escape(results[0]) << "','" << database.escape(results[1]) << "'," << results[2] << ")";
					//database.execute_query(query);
					/*
					query.clear();
					query << "select id,filename from music";

					
					Result_Set resultset(query.str());
					if (!database.execute_query(query,resultset))
					{
						cout << "Content-type: text/html\\n\n";
						cout << "error with " << query.str();
					}
					*/

		           		cout << "Content-type: application/octet-stream\n";
					//cout << "Content-Length: " <<  << "\n";
					cout << "Content-Disposition: attachment ; filename=Music.m3u\n\n";	
//					do
//					{		
						streamData ="#EXTINF: [ Streaming Music ]\n";
						streamData += "http://stream.binarystream.net/rk/1983/" + randi + "\n";
			
						cout.write(streamData.c_str(),streamData.length());
//					}while(resultset++);
					return 0;
			}
			else if (cgi_utilities.cgivalue("action")=="queue")
			{
				Query query;
				
				DB_Handler database;
				database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"));
				string list="";
				string streamid;
//						string randi=sf.random_string(45);
//						query << "insert into stream (`queueNum`,`account_id`,`streamID`,`IP`,`timestamp`,`key`,`location`,`filename`,`filesize`) VALUES(-1,'" << login.user.account_id << "','" << randi << "','" << getEnv("REMOTE_ADDR") << "','" << time(NULL)+3600 << "'," << cgi_utilities.cgivalue("key") << ",'" << results[0] << "','" << results[1] << "'," << results[2] << ")";
				query << "select queueNum,filename,streamID from stream where account_id = '" << login.user.account_id << "' order by queueNum asc";
				Result_Set results(query.str());
				list +="<table width=\"50%\"><tr><td align=\"left\">";
				if (database.execute_query(query,results))
				{
					if (results.number_rows() > 0)
					{
						do
						{
						streamid=results[2];
						list += "<input type=\"checkbox\" name=\"" + results[0] +  "\" value=\"checked\">" + results[1] + "<br />";
						}while(results++);
						//<br><input type=\"submit\" name=\"type\" value=\"Listen to Music in Reverse\">
						list+="<center><input type=\"submit\" name=\"type\" value=\"Delete Selected\"><input type=\"submit\" name=\"type\" value=\"Listen to Music\"></center>";

					}else
					{
						list+="Queue is empty<br /><a href=\"/cgi-bin/music.cgi?SESSIONID=" + cgi_utilities.cgivalue("SESSIONID") + "\">Return to Music Station</a>";
					}
				}
				list+="</td></tr></table>";
                html html_controls;
                html_controls.html_value("urlsess",cgi_utilities.cgivalue("SESSIONID"));
                html_controls.html_value("list",list);
                if (cgi_utilities.cgivalue("mp3")!="true")
                {
	                html_controls.html_value("hide1","<!--");
	                html_controls.html_value("hide2","-->");
                }
                html_controls.html_value("stream",variables.value("domain")+ "queue/" + streamid);
                html_controls.parsePage(variables.value("queue").c_str());
			}
            else if (cgi_utilities.cgivalue("searchWord")!="" && cgi_utilities.cgivalue("begin")!="")
			{
			    		string results;
                results = musicresults(cgi_utilities.cgivalue("searchWord"),false,atoi(cgi_utilities.cgivalue("begin").c_str()),cgi_utilities.cgivalue("searchType"));
                html html_controls;
                html_controls.html_value("urlsess",cgi_utilities.cgivalue("SESSIONID"));
		if (cgi_utilities.cgivalue("searchType")=="song")
			html_controls.html_value("messages","Searching by song name");
                html_controls.html_value("search_results",results);
                html_controls.parsePage(variables.value("searchmain").c_str());
            }
			else
			searchmain();
				
            }
           else
            {
//                        login.logout();
			display_login("You have been logged out 2");

			cout << "check_session result " << login.check_session(cgi_utilities.cgivalue("SESSIONID"));
            }

	  }
	  
    } 
    else
    {
		
  		if (login.check_session(cgi_utilities.cgivalue("SESSIONID"))==VALID)
            {
	            				permissions permission( login.user.group_id );
				if (permission.permission("view_music") == false)
				{
					login.logout();
					display_login("Please Log in");
					return 0;
				}

	            if (cgi_utilities.cgivalue("action")=="stream")
	            {
		            Query query;
		            
		            char cp[5];
		            int max= atoi( cgi_utilities.form_data("max").c_str() );
		            string streamData="#EXTM3U\n";
		            DB_Handler database;
					database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"));
					Result_Set results(query.str());
					Result_Set result(query.str());
			        string_functions sf;
					string randi=sf.random_string(45);
					bool exceed=false;
		            for (int i=1; i <= max; i++)
		            {
			            sprintf(cp,"%i",i);
			            if (cgi_utilities.form_data(cp) != "")
			            {
				            long timestamp = time(NULL)+3600;
					        query.clear();
					        query << "select location,filename,filesize,artist,song from music where id='" << cgi_utilities.form_data(cp) << "'";
							results.SetQuery(query.str());
							database.execute_query(query,results);
							query.clear();
							if (cgi_utilities.form_data("type")=="Stream Selected")
							{
								randi=sf.random_string(45);
								string keyhash = MD5String((char*)cgi_utilities.form_data(cp).c_str());
//			cout << "Content-type: text/html\n\n";
								query << "insert into stream (`streamID`,`IP`,`timestamp`,`key`,`keyHash`,`location`,`filename`,`filesize`) VALUES('" << randi << "','" << getEnv("REMOTE_ADDR") << "','" << timestamp << "'," << cgi_utilities.form_data(cp) << ",'" << keyhash << "','" << database.escape(results[0]) << "','" << database.escape(results[1]) << "'," << results[2] << ")";
								database.execute_query(query);
								streamData +="#EXTINF: ," + results[3] + "-" +results[4] + "[ Streaming Music from BinaryStream ]\n";
								streamData += variables.value("domain")+ "stream/" + randi + "\n";
	
							}
							else
							{
								query << "select queueNum,streamID from stream where account_id='" << login.user.account_id << "' order by queueNum desc limit 0,1";
								result.SetQuery(query.str());

								database.execute_query(query,result);
								
								
								int max=0;
								max=atoi(result[0].c_str())+1;
								if (result.number_rows() > 0)
									randi=result[1];
								query.clear();
								query << "select count(*) as ct from stream where account_id='" << login.user.account_id << "'";
								result.SetQuery(query.str());
								database.execute_query(query,result);

								if (atoi(result[0].c_str()) >=30)
									{
										exceed=true;
										break;
									}
								query.clear();
								randi=sf.random_string(45);
								string keyhash = MD5String((char*)cgi_utilities.form_data(cp).c_str());
								query << "insert into stream (`queueNum`,`account_id`,`streamID`,`IP`,`timestamp`,`key`,`keyHash`,`location`,`filename`,`filesize`) VALUES(" << max << ",'" << login.user.account_id << "','" << randi << "','" << getEnv("REMOTE_ADDR") << "','" << timestamp << "'," << cgi_utilities.form_data(cp) << ",'" << keyhash << "','" << database.escape(results[0]) << "','" << database.escape(results[1]) << "'," << results[2] << ")";
								database.execute_query(query);
							}
						
			            }
			            
		            }
		            	
						if (cgi_utilities.form_data("type")=="Stream Selected")
						{
		            	cout << "Content-type: application/octet-stream\n";
						cout << "Content-Length: " << streamData.length() << "\n";
						cout << "Content-Disposition: attachment ; filename=stream.m3u\n\n";	
						
						//cout << streamData << endl;
						
						cout.write(streamData.c_str(),streamData.length());
						}
						else
						{
							string results;
							results+="<center>";
                			if (exceed)
                				results="Queue is full";
                			else
                				results="Songs added to queue";
                			results+="</center>";
                			html html_controls;
                			html_controls.html_value("urlsess",cgi_utilities.cgivalue("SESSIONID"));
                			html_controls.html_value("search_results",results);
                			html_controls.parsePage(variables.value("searchmain").c_str());
						}

	            }
	            else if (cgi_utilities.cgivalue("action")=="queue")
	            {
		            Query query;
		            Result_Set results(query.str());
		            								            DB_Handler database;
						database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"));
		            if (cgi_utilities.form_data("type")=="Listen to Music" || cgi_utilities.form_data("type")=="Listen to Music in Reverse")
		            {
						
						query << "select streamID,filename,`key` from stream where account_id='" << login.user.account_id << "' and queueNum >0";
						results.SetQuery(query.str());

						if (database.execute_query(query,results))
						{
			            //			            query << "insert into stream (`streamID`,`IP`,`timestamp`,`key`,`location`,`filename`,`filesize`) VALUES('" << randi << "','" << getEnv("REMOTE_ADDR") << "','" << timestamp << "'," << cgi_utilities.form_data(cp) << ",'" << database.escape(results[0]) << "','" << database.escape(results[1]) << "'," << results[2] << ")";
						//database.execute_query(query);
						string streamData="#EXTM3U\n";
						/*
							#EXTM3U
							#EXTINF:-1,Sarah McLachlan - Angel.mp3
						*/
							string mp3String=".mp3";
							
							do
							{
								string filename = results[1];
								long loc=0;
								if (( loc = filename.find(mp3String,0)) != string::npos)
								{
									filename = filename.substr(0,loc-1);
								}
								streamData +="#EXTINF: -1," + filename + "\n";
								if (cgi_utilities.form_data("type")=="Listen to Music in Reverse")
									streamData += "http://stream.binarystream.net/r/" + results[0] + "\n";
								else
									streamData += "http://stream.binarystream.net/qu/" + results[2] + "/" + results[0] + "\n";
							}while(results++);
					           	cout << "Content-type: application/octet-stream\n";
								cout << "Content-Length: " << streamData.length() << "\n";
								cout << "Content-Disposition: attachment ; filename=Music.m3u\n\n";	
							
							cout.write(streamData.c_str(),streamData.length());
							return 0;
						}
						else
						{
							                			html html_controls;
                			html_controls.html_value("urlsess",cgi_utilities.cgivalue("SESSIONID"));
                			html_controls.html_value("search_results","<center>Deadly Error</center>");
                			html_controls.parsePage(variables.value("searchmain").c_str());
                			return 0;
							}
		            }
		            else if (cgi_utilities.form_data("type")=="Delete Selected")
		            {
			            
			         	query << "select queueNum from stream where account_id='" << login.user.account_id << "'";
						results.SetQuery(query.str());

						if (database.execute_query(query,results))
						{
							if (results.number_rows() > 0)
							{
								do
								{
									if (cgi_utilities.form_data(results[0])=="checked")
									{
										query.clear();
										query << "delete from stream where account_id='" << login.user.account_id << "' and queueNum=" << results[0];
										database.execute_query(query);
									}
								}while(results++);
							}
						}
		            }
		            
		            	
				
				string list="";
				string streamid;
//						string randi=sf.random_string(45);
//						query << "insert into stream (`queueNum`,`account_id`,`streamID`,`IP`,`timestamp`,`key`,`location`,`filename`,`filesize`) VALUES(-1,'" << login.user.account_id << "','" << randi << "','" << getEnv("REMOTE_ADDR") << "','" << time(NULL)+3600 << "'," << cgi_utilities.cgivalue("key") << ",'" << results[0] << "','" << results[1] << "'," << results[2] << ")";
query.clear();
				query << "select queueNum,filename,streamID from stream where account_id = '" << login.user.account_id << "' order by queueNum asc";
//				Result_Set results(query.str());
				results.SetQuery(query.str());
				list +="<table width=\"100%\"><tr><td>";
				if (database.execute_query(query,results))
				{
					if (results.number_rows() > 0)
					{
						do
						{
						streamid=results[2];
						list += "<input type=\"checkbox\" name=\"" + results[0] +  "\" value=\"checked\">" + results[1] + "<br />";
						}while(results++);
						list+="<input type=\"submit\" name=\"type\" value=\"Delete Selected\"><input type=\"submit\" name=\"type\" value=\"Listen to Music\">";

					}else
					{
						list="Queue is empty<br /><a href=\"/cgi-bin/music.cgi?SESSIONID=" + cgi_utilities.cgivalue("SESSIONID") + "\">Return to Music Station</a>";
					}
				}
				list+="</td></tr></table>";
                html html_controls;
                html_controls.html_value("urlsess",cgi_utilities.cgivalue("SESSIONID"));
                html_controls.html_value("list",list);
                if (cgi_utilities.cgivalue("mp3")!="true")
                {
	                html_controls.html_value("hide1","<!--");
	                html_controls.html_value("hide2","-->");
                }
                html_controls.html_value("stream",variables.value("domain")+ "queue/" + streamid);
                html_controls.parsePage(variables.value("queue").c_str());

	            }
				else if (cgi_utilities.cgivalue("action") == "edit_song")
				{
					
					
					
					Query query;
					Result_Set results(query.str());
					DB_Handler database;
					database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"));
					
					string song = database.escape(cgi_utilities.form_data("song"));
					string artist = database.escape(cgi_utilities.form_data("artist"));
					
					query << "update `music` set `artist`='" << artist << "',`song`='" << song << "' where `id`=" << cgi_utilities.cgivalue("key");
	//				cout << "Content-type: text/html\n\n";
//					cout << query.str();
					database.execute_query(query);
						
					query.clear();
					query << "select location,filename,filesize,artist,song,downloads from music where id='" << cgi_utilities.cgivalue("key") << "'";
					if (database.execute_query(query,results))
					{
						if (cgi_utilities.cgivalue("DOWNLOAD")=="true")
						{
							query.clear();
							
							query << "update music set downloads='" << atoi( results[5].c_str() )+1 << "' where id=" << cgi_utilities.cgivalue("key") << "";
							database.execute_query(query);
						char *buffer;
	
	//					cout << "Content-type: audio/mpeg\n\n";
						if (cgi_utilities.cgivalue("stream")=="true")
						{
							//string streamData="Hello world";
							// create a timestamp that allows one hour of streaming
							// music for that song
						//	cout << "Content-type: text/html\n\n";
							query.clear();
							string_functions sf;
							string randi=sf.random_string(45);
							query << "insert into stream (`queueNum`,`account_id`,`streamID`,`IP`,`timestamp`,`key`,`location`,`filename`,`filesize`) VALUES(-1,'" << login.user.account_id << "','" << randi << "','" << getEnv("REMOTE_ADDR") << "','" << time(NULL)+3600 << "'," << cgi_utilities.cgivalue("key") << ",'" << results[0] << "','" << results[1] << "'," << results[2] << ")";
							database.execute_query(query);
							string streamData="#EXTM3U\n#EXTINF: ," + results[3] + "-" +results[4] + "[ Top 40 - 02:01 / 1.8Mb ]\n";
							streamData += variables.value("domain")+ "stream/" + randi;
							
							cout << "Content-type: application/octet-stream\n";
							cout << "Content-Length: " << streamData.length() << "\n";
							cout << "Content-Disposition: attachment ; filename=stream.m3u\n\n";	
							
							//cout << streamData << endl;
							
							cout.write(streamData.c_str(),streamData.length());
						}
						else
						{
						cout << "Content-type: application/octet-stream\n";
						if (cgi_utilities.cgivalue("clip")=="true")				
							cout << "Content-Length: " << atoi(results[2].c_str())/8 << "\n";
						else
						cout << "Content-Length: " << results[2] << "\n";
						cout << "Content-Disposition: attachment ; filename= " << results[1] << "\n\n";
						string mloc = results[0];
						unsigned int filesz = atoi ( results[2].c_str() );
						ifstream charlie;
						string ilename = results[0]+results[1];
						charlie.open(ilename.c_str(), ios::in|ios::binary|ios::ate );
						long size=0;
						   size = charlie.tellg();
						if (cgi_utilities.cgivalue("clip")=="true")	
						   size=size/8;
	
						//while (charlie)
						//	cout << charlie;
						int div = size/8; // eights
						buffer= new char[div];
						//char buffer[size];
						charlie.seekg(0);
						
					for (int i =0; i<= size; i+=div)
						{
							  if (login.check_session(cgi_utilities.cgivalue("SESSIONID"))!=VALID)
							  {
								 cout << '\0';
								 break;				
							 }    
							if ((size-i) < div)
							{
							charlie.read(buffer,(size-i));
								cout.write(buffer,(size-i));
								break;
							 }  				
							else
							{
							   charlie.read(buffer,div);
								cout.write(buffer,div);
							}
							
	
	
					
						}	
									
						charlie.close();
					
							query.clear();
							query << "select downloads,size from statistics where id=1";
							database.execute_query(query,results);
							query.clear();
							query << "update statistics set size=" << atoi( results[1].c_str() )+filesz << ",downloads=" << atoi(results[0].c_str())+1 << " where id =1";
									database.execute_query(query);
	}
	//					delete [] buffer;
						}
						else
						{
							html html_controls; 
							html_controls.html_value("urlsess",cgi_utilities.cgivalue("SESSIONID"));
							html_controls.html_value("filename",results[1]);
							char fsize[10];
							sprintf(fsize,"%.2f MB",atof(results[2].c_str())/(1024*1024));
				
							html_controls.html_value("filesize",fsize);
						
							html_controls.html_value("artist",results[3]);
							html_controls.html_value("song",results[4]);
							html_controls.html_value("key",cgi_utilities.cgivalue("key"));
							
							if (permission.permission("edit_song") == true)
							{
								string edit_table = "<table width=\"60%\"><tr><td width=\"50%\">";
								edit_table += "Artist</td><td width=\"50%\"><input type=\"text\" name=\"artist\" value=\"";
								edit_table += results[3];
								edit_table +="\"></td></tr><tr><td width=\"50%\">Song</td><td width=\"50%\"><input type=\"text\" name=\"song\" value=\"";
								edit_table +=results[4];
								edit_table +="\"></td></tr><tr><td width=\"50%\"> </td><td width=\"50%\"><input type=\"submit\" name=\"edit\" value=\"change\"></td></tr></table>";
								html_controls.html_value("admin_form",edit_table);
								// $(admin_form)
							}
							
							html_controls.parsePage(variables.value("musicfound").c_str());
						}
					}
					else searchmain();
				}
	            else
	            {
				
					string results;
                	results = musicresults(cgi_utilities.form_data("search"),false,atoi(cgi_utilities.cgivalue("begin").c_str()),cgi_utilities.cgivalue("searchType"));
                	html html_controls;
                
    				DB_Handler database;
					database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"));
					Query query;
                	query << "select size,downloads from statistics where id=1";
					Result_Set resultss(query.str());
				
	            	if (database.execute_query(query,resultss))
	            	{
					
                  	int tot=atoi(resultss[0].c_str());
														
                  	char *d = new char[255];
                  	sprintf(d,"%i downloads wasting %i MB of bandwidth",atoi(resultss[1].c_str()),tot/(1024*1024));
                  	string totals = d;
                  	delete [] d;
				
                	html_controls.html_value("totals",totals);	                   
				
	            	}    
		           
                	html_controls.html_value("urlsess",cgi_utilities.cgivalue("SESSIONID"));
                	html_controls.html_value("search_results",results);
                	//if (misspell)
                        	//html_controls.html_value("suggestions",suggestion);
				
                	html_controls.parsePage(variables.value("searchmain").c_str());
				}
            }
           else
            {
                        login.logout();
			display_login("You have been logged out");

            }
}
    //}
    return 0;
}
