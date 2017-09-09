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
#include "md5.h"

#include "elfat.h"
#include "string_functions.h"
#include "efsFunctions.h"
#define DATA "/home/marc/data_files/efs.dat"
cgiparser cgi_utilities;	
    LoginManage login;
    #include "mm.h"
retreive_values variables;

int main()
{
	if (! check_ip() ) return 0;
    
 	variables.parse_file(DATA);
	cookies cookieMonster;
	
	if (cgi_utilities.cgivalue("SESSIONID") == "")
		{
            login.logout();
			display_login("Please Log in");
		}else
		{
           if (login.check_session(cgi_utilities.cgivalue("SESSIONID"))==VALID)
            {
	            
				permissions permission( login.user.group_id );
				if (permission.permission("use_efs") == false)
				{
					login.logout();
					display_login("Please Log in");
					return 0;
				}
				string userDirectory = "";
				string parentDirectory="",parentFID="/",myFID="";
				string encryptedHome="/";
				// obtain user directory whether pin is correct or not
				 DB_Handler database;
				database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"));
				
				string getDir="";
				getDir = cookieMonster.cookieEntry("efs_current_DIR");
				if (getDir == "")
					getDir ="/";
				
				
				Query query;
				
                query << "select dirName,parent from efs_dirs where account_id='" << login.user.account_id << "' and an = '" << getDir << "'";
				Result_Set results(query.str());
	            if (database.execute_query(query,results))
				{
					if (results.number_rows() > 0)
					{
						userDirectory = variables.value("base_user_dir");
						userDirectory += results[0];
						userDirectory +="/";
						parentDirectory = results[0];
						if (results[1] != "")
						{
							query.clear();
				
                			query << "select an from efs_dirs where account_id='" << login.user.account_id << "' and dirName = '" << results[1] << "'";
							//Result_Set results(query.str());
							myFID = results[1];
	            			if (database.execute_query(query,results))
							{
								if (results.number_rows() > 0)
								{
									
									parentFID = results[0];
									
								}
								else
								{
									
									parentDirectory = "";
								}
							}
						}
					}
					else
					{
						string_functions sf;
						string dir = sf.random_string(20);
						query.clear();
						
						query << "insert into efs_dirs (account_id,an,dirName) values('" << login.user.account_id << "','" << encryptedHome << "','" << dir << "')";
						if (database.execute_query(query))
						{
							userDirectory = variables.value("base_user_dir");
							userDirectory += dir;
							userDirectory +="/";
							char *c = new char[20 + userDirectory.length()];
							sprintf(c,"mkdir %s",userDirectory.c_str());
							
							system(c);
							delete [] c; 
						}
						else
						{
							enterPin(cgi_utilities.cgivalue("SESSIONID"));
							return 0;
						}
					
					}
				}
				else
				{
					enterPin(cgi_utilities.cgivalue("SESSIONID"));
					return 0;
				}
				if (cookieMonster.cookieEntry("efs_session_PIN").length()==0)
				{
					
					if (cgi_utilities.METHOD()==GET )
					{
						enterPin(cgi_utilities.cgivalue("SESSIONID"));
						return 0;
					}else
					{
						if (cgi_utilities.cgivalue("action")=="pinconnect")
						{
							// user is attempting to login
							string pin = cgi_utilities.form_data("efspin");
							string extraPin = MD5String( (char*)(pin.c_str()));
							for(int i=pin.length(),j=-1; i<32; i++)
							{
								pin+=extraPin[++j];
							}
								//cout << "Content-type: text/html\n\n";
							
							EncryptedLFAT fileTable(userDirectory,pin);
							if (fileTable.accessFAT() == true )
							{
								//cout << "Content-type: text/html\n\n";
								
								
								try
								{
									Botan::LibraryInitializer init("thread_safe use_engines");
									Botan::byte PIN[32];
									
									if (cgi_utilities.cgivalue("SESSIONID").length()!=32)
									{
										login.logout();
										display_login("Please Log in");	
										return 0;
									}
									
									for (int i=0; i < 32; i++)
									{
										PIN[i] = cgi_utilities.cgivalue("SESSIONID").at(i);
									}
									
									Botan::SymmetricKey key(PIN,32);   //instantiate key
									Botan::Pipe pipe(Botan::get_cipher("AES/ECB", key, 
									Botan::ENCRYPTION));
									pipe.start_msg();
									pipe.write(pin);
									pipe.end_msg();
									string session_pin = pipe.read_all_as_string(0);
									char t[10];
									sprintf(t,"%i",time(NULL)+(10*60));
//									cout << "Content-type: text/html\n\n";
	//								cout << session_pin.length();
									cookieMonster.create_cookie("efs_session_PIN",session_pin,t,"/","binarystream.net");

								} catch (std::exception &e) 
								{
									enterPin(cgi_utilities.cgivalue("SESSIONID"),e.what());
									return 0;				
								}
								
								userMain(parentFID,cgi_utilities.cgivalue("SESSIONID"),showDirectory(cgi_utilities.cgivalue("SESSIONID"),userDirectory,userDirectory,&fileTable));
								cout << "user directory is " << userDirectory;
								cout << " " << cgi_utilities.form_data("efspin") << " " <<  pin << " " << extraPin;
								return 0;
							}
							else
							{
								enterPin(cgi_utilities.cgivalue("SESSIONID"));
								return 0;						
							}
						}
					}
				}
				else
				{
					if (cgi_utilities.METHOD()==GET )
    				{
						string pin = "";
//													cout << "Content-type: text/html\n\n";
	//				cout << "we have the pin, it is";
		//			cout << cookieMonster.cookieEntry("efs_session_PIN") << " ";;
						try
						{
							Botan::LibraryInitializer init("thread_safe use_engines");
							Botan::byte PIN[32];
							
							if (cgi_utilities.cgivalue("SESSIONID").length()!=32)
							{
								login.logout();
								display_login("Please Log in");	
								return 0;
							}
							
							for (int i=0; i < 32; i++)
							{
								PIN[i] = cgi_utilities.cgivalue("SESSIONID").at(i);
							}
							
							Botan::SymmetricKey key(PIN,32);   //instantiate key
							Botan::Pipe pipe(Botan::get_cipher("AES/ECB", key, 
							Botan::DECRYPTION));
							string encryptedpin=cookieMonster.cookieEntry("efs_session_PIN");
							pipe.start_msg();
							pipe.write(encryptedpin);
							pipe.end_msg();
							pin = pipe.read_all_as_string(0);


						} catch (std::exception &e) 
						{
					
//
							cout << "yo " << e.what() << " " << cookieMonster.cookieEntry("efs_session_PIN").length();
							enterPin(cgi_utilities.cgivalue("SESSIONID"));
							return 0;
						}
						
						EncryptedLFAT fileTable(userDirectory,pin);
						if (fileTable.accessFAT() == true )
						{
							if (cgi_utilities.cgivalue("action")=="CD" || cgi_utilities.cgivalue("action")=="view" && cgi_utilities.cgivalue("fid") != "")
							{
								
								if (cgi_utilities.cgivalue("fid") == "/")
								{
									DB_Handler database;
										database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"));
										
										Result_Set results;
										Query query;
										
										query << "select dirName from efs_dirs where account_id='" << login.user.account_id << "' and an='" << cgi_utilities.cgivalue("fid") << "' and parent=''";								
										
										
										if (database.execute_query(query,results))
										{
											
											userDirectory = variables.value("base_user_dir");
											userDirectory += results[0];
											userDirectory +="/";
											if (cgi_utilities.cgivalue("fid")!="/")
												cookieMonster.create_cookie("efs_current_DIR",cgi_utilities.cgivalue("fid"));
											else
												cookieMonster.create_cookie("efs_current_DIR","");
											EncryptedLFAT newDirectory(userDirectory,pin);
											if (newDirectory.accessFAT() == true )
												userMain(parentFID,cgi_utilities.cgivalue("SESSIONID"), showDirectory(cgi_utilities.cgivalue("SESSIONID"),userDirectory,userDirectory,&newDirectory));								
											else
												enterPin(cgi_utilities.cgivalue("SESSIONID"));
											return 0;	
										}else
										{
											enterPin(cgi_utilities.cgivalue("SESSIONID"));
											return 0;	
										}
									userMain(parentFID,cgi_utilities.cgivalue("SESSIONID"), showDirectory(cgi_utilities.cgivalue("SESSIONID"),userDirectory,userDirectory,&fileTable));								
								}else if (cgi_utilities.cgivalue("action")=="CD" )
								{
									
										DB_Handler database;
										database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"));
										
										string myfid = cgi_utilities.cgivalue("fid");
										if (myfid == "")
											myfid = "/";
										Result_Set results;
										Query query;
										
										query << "select dirName,parent from efs_dirs where account_id='" << login.user.account_id << "' and  an='" << myfid << "' and parent='" << parentDirectory << "'";																
										cout << "Content-type: text/html\n\n";
									cout << query.str();
										
										if (database.execute_query(query,results))
										{
											if (results.number_rows() > 0)
											{												
												userDirectory = variables.value("base_user_dir");
												userDirectory += results[0];
												userDirectory +="/";
												
												string localFID="";
												query.clear();
												query << "select an from efs_dirs where account_id='" << login.user.account_id << "' and dirName='" << results[1] << "'";								
												if (database.execute_query(query,results))
												{
													if (results.number_rows() > 0)
													{
														localFID = results[0];
														
													}
											
											
												}
												if (cgi_utilities.cgivalue("fid")!="/")
													cookieMonster.create_cookie("efs_current_DIR",cgi_utilities.cgivalue("fid"));
												else
													cookieMonster.create_cookie("efs_current_DIR","");
												EncryptedLFAT newDirectory(userDirectory,pin);
												if (newDirectory.accessFAT() == true )
													userMain(localFID,cgi_utilities.cgivalue("SESSIONID"), showDirectory(cgi_utilities.cgivalue("SESSIONID"),userDirectory,userDirectory,&newDirectory));								
												else
													enterPin(cgi_utilities.cgivalue("SESSIONID"));
												return 0;	
											}
											else

											{
												enterPin(cgi_utilities.cgivalue("SESSIONID"));
												return 0;	
											}
										}else
										{
											enterPin(cgi_utilities.cgivalue("SESSIONID"));
											return 0;	
										}
								}
								else
								
									{
									elfatFileData &m = fileTable.get_elFile(  cgi_utilities.cgivalue("fid") );
									if (m.type == ELDIRECTORY)
									{
										
										DB_Handler database;
										database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"));
										
										Result_Set results;
										Query query;
										
										query << "select dirName,parent from efs_dirs where account_id='" << login.user.account_id << "' and an='" << cgi_utilities.cgivalue("fid") << "' and parent='" << parentDirectory << "'";
										
										
										if (database.execute_query(query,results))
										{
											userDirectory = variables.value("base_user_dir");
											userDirectory += results[0];
											userDirectory +="/";
											string localFID="";
											query.clear();
											query << "select an from efs_dirs where account_id='" << login.user.account_id << "' and dirName='" << results[1] << "'";								
											if (database.execute_query(query,results))
											{
												if (results.number_rows() > 0)
												{
													localFID = results[0];
													
												}
										
										
											}
											
											if (cgi_utilities.cgivalue("fid")!="/")
												cookieMonster.create_cookie("efs_current_DIR",cgi_utilities.cgivalue("fid"));
											else
												cookieMonster.create_cookie("efs_current_DIR","");
											EncryptedLFAT newDirectory(userDirectory,pin);
											if (newDirectory.accessFAT() == true )
												userMain(localFID,cgi_utilities.cgivalue("SESSIONID"), showDirectory(cgi_utilities.cgivalue("SESSIONID"),userDirectory,userDirectory,&newDirectory));								
											else
												enterPin(cgi_utilities.cgivalue("SESSIONID"),"Cannot access FAT");
											return 0;	
										}else
										{
											enterPin(cgi_utilities.cgivalue("SESSIONID"),"Not a directory");
											return 0;	
										}
									}
									else
									{
										if (cgi_utilities.cgivalue("type")=="stat")
										{
											string image = "image",text="text";
											if (m.contentType.find(image,0) != string::npos)
												view_image_file(m,cgi_utilities.cgivalue("fid"),cgi_utilities.cgivalue("SESSIONID"));
											else if (m.contentType.find(text,0) != string::npos)
											{
												char *p = fileTable.getFileData( cgi_utilities.cgivalue("fid") );
												view_text_file(m,cgi_utilities.cgivalue("fid"),cgi_utilities.cgivalue("SESSIONID"),cgi_utilities.cgivalue("hightlight"),p);
												delete [] p;
											}
											else
											{
												
												view_data_file(m,cgi_utilities.cgivalue("fid"),cgi_utilities.cgivalue("SESSIONID"));
												
												/*
												char *p = fileTable.getFileData( cgi_utilities.cgivalue("fid") );
											string content = m.contentType;
											string d = "text/";
											
											database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"));
											
											Result_Set results;
											Query query;
											
											query << "select mime from mime_types where ext='" << fileExtension(m.filename) << "'";
										
											
											if (database.execute_query(query,results))
											{
												if (results.number_rows() > 0)
												{
													content=results[0];
												}
											
											}
											if (content.find(d,0) != string::npos)
												content="text/html";
											
											
			//								string o = "dat";
				//							if (m.filename.find(o,0) != string::npos)
					//							content="text/html";
											cout << "Content-type: " << content << "\n";
											cout << "Content-Disposition: attachment; filename=" << m.filename << "\n";
											cout << "Content-ID: " << m.filename << "\n\n";
											//cout << m.fileSizeAsInt;
											//p[m.fileSizeAsInt] = '\0';
											cout.write(p,m.fileSizeAsInt);
											delete [] p;
											*/
											}
										}
										else
										{
											char *p = fileTable.getFileData( cgi_utilities.cgivalue("fid") );
											string content = m.contentType;
											string d = "text/";
											
											database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"));
											
											Result_Set results;
											Query query;
											
											query << "select mime from mime_types where ext='" << fileExtension(m.filename) << "'";
										
											
											if (database.execute_query(query,results))
											{
												if (results.number_rows() > 0)
												{
													content=results[0];
												}
											
											}
											if (content.find(d,0) != string::npos)
												content="text/html";
											
											
			//								string o = "dat";
				//							if (m.filename.find(o,0) != string::npos)
					//							content="text/html";
											cout << "Content-type: " << content << "\n";
											cout << "Content-Disposition: attachment; filename=" << m.filename << "\n";
											cout << "Content-ID: " << m.filename << "\n\n";
											//cout << m.fileSizeAsInt;
											//p[m.fileSizeAsInt] = '\0';
											cout.write(p,m.fileSizeAsInt);
											delete [] p;
										}
											
									}
								}
							}
							else
							{
								userMain(parentFID,cgi_utilities.cgivalue("SESSIONID"), showDirectory(cgi_utilities.cgivalue("SESSIONID"),userDirectory,userDirectory,&fileTable));								
							}
							return 0;
						}
						else
						{
							enterPin(cgi_utilities.cgivalue("SESSIONID"));
							return 0;				
						}
						
					}
					else
					{
						if (cgi_utilities.cgivalue("action")=="pinconnect")
						{
							// user is attempting to login
							string pin = cgi_utilities.form_data("efspin");
							string extraPin = MD5String( (char*)(pin.c_str()));
							for(int i=pin.length(),j=-1; i<32; i++)
							{
								pin+=extraPin[++j];
							}
							EncryptedLFAT fileTable(userDirectory,pin);
							if (fileTable.accessFAT() == true )
							{
								
								try
								{
									Botan::LibraryInitializer init("thread_safe use_engines");
									Botan::byte PIN[32];
									
									if (cgi_utilities.cgivalue("SESSIONID").length()!=32)
									{
										login.logout();
										display_login("Please Log in");	
										return 0;
									}
									
									for (int i=0; i < 32; i++)
									{
										PIN[i] = cgi_utilities.cgivalue("SESSIONID").at(i);
									}
									
									Botan::SymmetricKey key(PIN,32);   //instantiate key
									Botan::Pipe pipe(Botan::get_cipher("AES/ECB", key, 
									Botan::ENCRYPTION));
									pipe.start_msg();
									pipe.write(pin);
									pipe.end_msg();
									string session_pin = pipe.read_all_as_string(0);
									char t[10];
									sprintf(t,"%i",time(NULL)+(10*60));
									cookieMonster.create_cookie("efs_session_PIN",session_pin,t);

								} catch (std::exception &e) 
								{
									enterPin(cgi_utilities.cgivalue("SESSIONID"));
									return 0;				
								}
								
								userMain(parentFID,cgi_utilities.cgivalue("SESSIONID"),showDirectory(cgi_utilities.cgivalue("SESSIONID"),userDirectory,userDirectory,&fileTable));
								return 0;
							}
							else
							{
								enterPin(cgi_utilities.cgivalue("SESSIONID"));
								return 0;						
							}
						}
						else
						{
							if (cgi_utilities.cgivalue("action")=="selected")
							{
								// perform an action on the selected elements
								
								string pin = "";
//													cout << "Content-type: text/html\n\n";
	//				cout << "we have the pin, it is";
		//			cout << cookieMonster.cookieEntry("efs_session_PIN") << " ";;
								try
								{
									Botan::LibraryInitializer init("thread_safe use_engines");
									Botan::byte PIN[32];
									
									if (cgi_utilities.cgivalue("SESSIONID").length()!=32)
									{
										login.logout();
										display_login("Please Log in");	
										return 0;
									}
									
									for (int i=0; i < 32; i++)
									{
										PIN[i] = cgi_utilities.cgivalue("SESSIONID").at(i);
									}
									
									Botan::SymmetricKey key(PIN,32);   //instantiate key
									Botan::Pipe pipe(Botan::get_cipher("AES/ECB", key, 
									Botan::DECRYPTION));
									string encryptedpin=cookieMonster.cookieEntry("efs_session_PIN");
									pipe.start_msg();
									pipe.write(encryptedpin);
									pipe.end_msg();
									pin = pipe.read_all_as_string(0);
		
		
								} catch (std::exception &e) 
								{
							
		//
									cout << "yo " << e.what() << " " << cookieMonster.cookieEntry("efs_session_PIN").length();
									enterPin(cgi_utilities.cgivalue("SESSIONID"));
									return 0;
								}
								
								EncryptedLFAT fileTable(userDirectory,pin);
								if (fileTable.accessFAT() == true )
								{
									if (cgi_utilities.form_data( "actionperform" ) == "delete")
									{
										
										int checks = atoi( cgi_utilities.form_data("numchecks").c_str() );
										
										string hash="";
		//								cout << cgi_utilities.form_data( "actionperform" );
										for (int i=0; i < checks; i++)
										{
											char t[3];
											sprintf(t,"%i",i);
											
									//		cout << t << " -- ";
											if ( (hash = cgi_utilities.form_data(t) ) != "")
											{
												elfatFileData &m = fileTable.get_elFile(  hash );		
												fileTable.deleteFAT( hash );
												if (m.type == ELDIRECTORY)
												{
													DB_Handler database;
													database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"));
													
													Result_Set results;
													Query query;
													
													query << "select dirName from efs_dirs where account_id='" << login.user.account_id << "' and an='" << hash << "' and parent='" << parentFID << "'";															
													
													if (database.execute_query(query,results))
													{
														
														
														query.clear();
														query << "delete from efs_dirs where an='" << hash << "' and parent='" << parentFID << "' and account_id='" << login.user.account_id << "'";																
													
													if (database.execute_query(query))
														{
															
															
															userDirectory = variables.value("base_user_dir");
															userDirectory +=  results[0];
															userDirectory +="/";
															char *c = new char[20 + userDirectory.length()];
															sprintf(c,"rm -rf %s",userDirectory.c_str());
															system(c);
															delete [] c; 
															
													
														}
														else
														{
															enterPin(cgi_utilities.cgivalue("SESSIONID"));
															return 0;
														}
														
												
													}
													else
													{
														enterPin(cgi_utilities.cgivalue("SESSIONID"));
														return 0;
													}
													
													
												}
												// ok, we have to delete this
												
											}
											//cout << cgi_utilities.form_data(t) << " -- ";
										}
									}
									userMain(parentFID,cgi_utilities.cgivalue("SESSIONID"), showDirectory(cgi_utilities.cgivalue("SESSIONID"),userDirectory,userDirectory,&fileTable));
									return 0;
								}
							}
								else if (cgi_utilities.cgivalue("action")=="addFile")
								{
									string pin = "";
	//													cout << "Content-type: text/html\n\n";
		//				cout << "we have the pin, it is";
			//			cout << cookieMonster.cookieEntry("efs_session_PIN") << " ";;
									try
									{
										Botan::LibraryInitializer init("thread_safe use_engines");
										Botan::byte PIN[32];
										
										if (cgi_utilities.cgivalue("SESSIONID").length()!=32)
										{
											login.logout();
											display_login("Please Log in");	
											return 0;
										}
										
										for (int i=0; i < 32; i++)
										{
											PIN[i] = cgi_utilities.cgivalue("SESSIONID").at(i);
										}
										
										Botan::SymmetricKey key(PIN,32);   //instantiate key
										Botan::Pipe pipe(Botan::get_cipher("AES/ECB", key, 
										Botan::DECRYPTION));
										string encryptedpin=cookieMonster.cookieEntry("efs_session_PIN");
										pipe.start_msg();
										pipe.write(encryptedpin);
										pipe.end_msg();
										pin = pipe.read_all_as_string(0);
			
			
									} catch (std::exception &e) 
									{
								
			//
										cout << "yo " << e.what() << " " << cookieMonster.cookieEntry("efs_session_PIN").length();
										enterPin(cgi_utilities.cgivalue("SESSIONID"));
										return 0;
									}
									
									EncryptedLFAT fileTable(userDirectory,pin);
									if (fileTable.accessFAT() == true )
									{
										if (cgi_utilities.haveFile())
										{
											fileTable.deleteFAT( MD5String( (char*)cgi_utilities.getFileName().c_str() ) );
											fileTable.createFile(cgi_utilities.getFileName(),cgi_utilities.getFileType(),cgi_utilities.getFile(),cgi_utilities.getFileSize());
											
												//cout << cgi_utilities.form_data(t) << " -- ";
											}
									}
										userMain(parentFID,cgi_utilities.cgivalue("SESSIONID"), showDirectory(cgi_utilities.cgivalue("SESSIONID"),userDirectory,userDirectory,&fileTable));
										return 0;
								
								
							}
							else if (cgi_utilities.cgivalue("action")=="addDirectory")
								{
									string pin = "";
	//													cout << "Content-type: text/html\n\n";
		//				cout << "we have the pin, it is";
			//			cout << cookieMonster.cookieEntry("efs_session_PIN") << " ";;
									try
									{
										Botan::LibraryInitializer init("thread_safe use_engines");
										Botan::byte PIN[32];
										
										if (cgi_utilities.cgivalue("SESSIONID").length()!=32)
										{
											login.logout();
											display_login("Please Log in");	
											return 0;
										}
										
										for (int i=0; i < 32; i++)
										{
											PIN[i] = cgi_utilities.cgivalue("SESSIONID").at(i);
										}
										
										Botan::SymmetricKey key(PIN,32);   //instantiate key
										Botan::Pipe pipe(Botan::get_cipher("AES/ECB", key, 
										Botan::DECRYPTION));
										string encryptedpin=cookieMonster.cookieEntry("efs_session_PIN");
										pipe.start_msg();
										pipe.write(encryptedpin);
										pipe.end_msg();
										pin = pipe.read_all_as_string(0);
			
			
									} catch (std::exception &e) 
									{
								
			//
										cout << "yo " << e.what() << " " << cookieMonster.cookieEntry("efs_session_PIN").length();
										enterPin(cgi_utilities.cgivalue("SESSIONID"));
										return 0;
									}
									
									EncryptedLFAT fileTable(userDirectory,pin);
									if (fileTable.accessFAT() == true )
									{
										//if (cgi_utilities.haveFile())
										{
											//fileTable.deleteFAT( MD5String( (char*)cgi_utilities.getFileName().c_str() ) );
											string hash = fileTable.createDirectory( cgi_utilities.form_data("directory") );
											
											string_functions sf;
											string dir = sf.random_string(20);
											//hash = login.user.account_id + hash + ;
											//hash = MD5String( (char*)hash.c_str() );
											DB_Handler database;
											database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"));
											
								
											Query query;
											query << "insert into efs_dirs (account_id,an,dirName,parent) values('" << login.user.account_id << "','" << hash << "','" << dir << "','" << parentDirectory << "')";								
											if (database.execute_query(query))
											{
												
												userDirectory = variables.value("base_user_dir");
												userDirectory += dir;
												userDirectory +="/";
												char *c = new char[20 + userDirectory.length()];
												sprintf(c,"mkdir %s",userDirectory.c_str());
												
												system(c);
												delete [] c; 
												
												
												userMain(parentFID,cgi_utilities.cgivalue("SESSIONID"), showDirectory(cgi_utilities.cgivalue("SESSIONID"),userDirectory,userDirectory,&fileTable));
												return 0;
											}
											else
											{
												enterPin(cgi_utilities.cgivalue("SESSIONID"));
											}
											//fileTable.deleteFAT( MD5String( (char*)cgi_utilities.getFileName().c_str() ) );
											//fileTable.createFile(cgi_utilities.getFileName(),cgi_utilities.getFileType(),cgi_utilities.getFile(),cgi_utilities.getFileSize());
											
												//cout << cgi_utilities.form_data(t) << " -- ";
											}
									}
										userMain(parentFID,cgi_utilities.cgivalue("SESSIONID"), showDirectory(cgi_utilities.cgivalue("SESSIONID"),userDirectory,userDirectory,&fileTable));
										return 0;
								
								
							}
							else
							{
								enterPin(cgi_utilities.cgivalue("SESSIONID"));
								return 0;	
							}
						}
					}
				}
			}
			else
			{
				 login.logout();
				 display_login("Please Log in");
			}
		}
		
		/*
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
				if (permission.permission("use_efs") == false)
				{
					login.logout();
					display_login("Please Log in");
					return 0;
				}
				if (cookieMonster.cookieEntry("efs_session_PIN").length()==0)
				{
					enterPin(cgi_utilities.cgivalue("SESSIONID"));
					return 0;
				}
			}
			else
			{
				 login.logout();
				 display_login("Please Log in");
			}
		}
	}
	else
	{
		
	}
	*/
}
