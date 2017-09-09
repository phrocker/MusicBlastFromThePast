/***************************************************************************
 *            efsFunctions.h
 *
 *  Tue Jun  7 22:18:08 2004
 *  Copyright  2004  User
 *  Email
 ****************************************************************************/

#ifndef _EFSFUNCTIONS_H 
#define _EFSFUNCTIONS_H

#include "String.h"

#define ENTERPIN "/home/marc/public_html/efs/pin.html"

#define IMAGEVIEW "/home/marc/public_html/efs/views/imageview.html"

#define TEXTVIEW "/home/marc/public_html/efs/views/textview.html"

#define USERMAIN "/home/marc/public_html/efs/userview.html"


#define DATAVIEW "/home/marc/public_html/efs/views/dataview.html"



void
enterPin(string sessionid,string message)
{
	cookies cookieMonster;
	cookieMonster.create_cookie("efs_current_DIR","");
	cookieMonster.create_cookie("efs_current_DIR","");
	cookieMonster.create_cookie("efs_session_PIN","");
    html html_controls;
    
	html_controls.html_value("urlsess",sessionid);
	
	html_controls.html_value("message",message);
 
    html_controls.parsePage(ENTERPIN);
}

void
enterPin(string sessionid)
{
	enterPin(sessionid,"");
}

void
userMain(string parentFID, string sessionid, string table)
{
	
    html html_controls;
    
//	if (parentFID == "/")
//		parentFID+='/';
	html_controls.html_value("parentFID",parentFID);
	
	html_controls.html_value("urlsess",sessionid);
	
	html_controls.html_value("filetable",table);
 
    html_controls.parsePage(USERMAIN);
}


string
fileExtension(string filename)
{
	int l=0;
	string dot =".";
	for (; l >= 0; l--)
	{
		if (filename.at(l) == '.')
			break;
	}
	while ( filename.find(dot,0) != string::npos )
	{
		filename = filename.substr( filename.find(dot,0)+1,( filename.find(dot,0)+1)-filename.length());
	}
	
	return filename;
}

string
mime(string filename)
{
	int l=0;
	string dot =".";
	for (; l >= 0; l--)
	{
		if (filename.at(l) == '.')
			break;
	}
	while ( filename.find(dot,0) != string::npos )
	{
		filename = filename.substr( filename.find(dot,0)+1,( filename.find(dot,0)+1)-filename.length());
	}
	
	string ext = filename;
	string text = "/srv/www/htdocs/images/efs/" + ext;
	text+=".gif";
	ifstream image(text.c_str());
	if (image)
	{
		return ext;
		image.close();
	}
	else
		return "unknown";
}

string
getExtension(string filename)
{
		int l=0;
	string dot =".";
	for (; l >= 0; l--)
	{
		if (filename.at(l) == '.')
			break;
	}
	while ( filename.find(dot,0) != string::npos )
	{
		filename = filename.substr( filename.find(dot,0)+1,( filename.find(dot,0)+1)-filename.length());
	}
	
	string ext = filename;
	return ext;
}
void
showView(string hash,EncryptedLFAT *myfat)
{
	
}

string
showDirectory(string sessionid, string mainDirectory,string currentDirectory,EncryptedLFAT *myfat)
{
	//string table = "<table width= \"75%\" bgcolor=">";
	register int i=0; // rolls over at 4
	string table = "";
	vector< string > names;
	//string table ="<tr><td>";
	/*
	table += "<a href=\"/cgi-bin/efs.cgi?SESSIONID=";
	table += sessionid;
	table+="\"><center><img src=\"/images/efs/home.gif\" border=\"0\"/></center></a></td>";
	
	string home = "<a href=\"/cgi-bin/efs.cgi?SESSIONID=";
	home +=sessionid;
	home +="\">Home</a>";
	names.push_back( home );
	*/
	/*
	
	if (mainDirectory == currentDirectory)
		table +="<td>. (this is your main directory)</td>";
	else
		table +="<td>.</td>";
	table +="<td></td>  </tr>";
	*/
	
	myfat->beginListFS();
	
	int total=0;
	string  dot=".";
	while( myfat->getNext() )
	{
		
		elfatFileData &m = myfat->getCurrentFATData();
		
		if (i==0)
			table+="<tr>";
		table+="<td width=\"25%\"><center>";
		table +="<a href=\"/cgi-bin/efs.cgi?SESSIONID=";
		
		table+=sessionid;
		
		if (m.type != ELDIRECTORY)
			table+="&action=view&type=stat&fid=";
		else
			table+="&action=CD&type=stat&fid=";
		
		table+=m.filenameHash;
		
		table+="\"";
			if (m.type != ELDIRECTORY)
		table +=" target =\"_blank\"";
		table+=">\n";

		table+="<img src=\"/images/efs/";
		
		//cout << table;
		//cout << m.type;
		
		if (m.type != ELDIRECTORY)
			table +=mime(m.filename);
		else
			table +="directory";
		
		table +=".gif\" border=\"0\"/></a></center></td>";
		
		string link = "";
		//if (m.type != ELDIRECTORY)
		{
			char t[3];
			sprintf(t,"%i",total);
			link+="<input type=\"checkbox\" name=\"";
			link+=t;
			link+="\" value=\"";
			link+=m.filenameHash;
			link+="\" />";
		}
		link +="<br><a href=\"/cgi-bin/efs.cgi?SESSIONID=";
		link+=sessionid;
		if (m.type != ELDIRECTORY)
			link+="&action=view&type=stat&fid=";
		else
			link+="&action=CD&type=stat&fid=";
		link+=m.filenameHash;
		link+="\"";
			if (m.type != ELDIRECTORY)
		link +=" target =\"_blank\"";
		link+=">\n";
		
		if (m.type != ELDIRECTORY)
		{
			if (m.filename.find(dot,0) > 6)
			{
				link+=m.filename.substr(0,6);
				link+="~." + getExtension(m.filename);
			}
					else
			link+=m.filename;

		}
		else
			link+=m.filename;
		link+="</a>";
		names.push_back( link );
		//cout << i+1 << ": " <<  m.filename << endl;
		
		if ( (++i % 4) == 0)
		{
			i = 0;
			table+="<tr>";
			for (int j=0; j < 4; j++)
			{
				string name = names.at(j);
				table+="<td width=\"25%\"><center>";
				table+=name;
				table+="</center></td>";
			}
			names.clear();
			names.resize(0);
			table+="</tr>";
		}
		total++;
	}
	
	if (i > 0)
	{
		for (int j=i; j < 4; j++)
		{
			//string name = names.pop_front();
			table+="<td><img src=\"/images/efs/blank.gif\" /></td>";
		}
		table+="</tr><tr>";
		for (int j=0; j < 4; j++)
		{
			string name = "    ";
			if ( j < names.size())
				name = names.at(j);
			table+="<td width=\"25%\"><center>";
			table+=name;
			table+="</center></td>";
		}
		table+="</tr>";
	}
	string hidden = "<input type=\"hidden\" name=\"numchecks\" value=\"";
	char t[3];
	sprintf(t,"%i",total);
	hidden+=t;
	hidden+="\">";
	//hidden +=total;
	table = hidden + table;
	if (i == 0)
	table+="Directory is empty...";
	
	return table;
}


// view files


void view_text_file(elfatFileData &m,string fid, string sessionid,string highlight, char* p)
{
    html html_controls;
    
	html_controls.html_value("urlsess",sessionid);
	
	//html_controls.html_value("message",message);
	html_controls.html_value("filename",m.filename);
	string filesize = "";
	char c[10];
	if (m.fileSizeAsInt > 1024)
	{
		if (m.fileSizeAsInt > 1024*1024)
		{
			sprintf(c,"%i",	m.fileSizeAsInt/(1024*1024));
			filesize = c;
			filesize += "MB";
		}
		else
		{
			sprintf(c,"%i",	m.fileSizeAsInt/(1024));
			filesize = c;
			filesize += "kB";
		}
		
	}
	else
	{
		filesize = m.fileSize + "B";
	}
	string data = "";
	char *x = p;
	int g=m.fileSizeAsInt-1;
	String *tempString = new String( p );
	
	char *split = new char[1];
	sprintf(split,"%c",0x0d);
	string bysplit = "\n";
	vector< String > strings = tempString->split( bysplit );
	for (int i=0; i < strings.size(); i++)
	{
		string currentString = strings.at ( i).toString();
		for (int j=0; j < currentString.length(); j++)
		{
			if (currentString.at(j) == 32)
				data+=" &nbsp;";
			else if (currentString.at(j) == '<')
				data+="&lt;";
			else if (currentString.at(j) == '>')
				data+="&gt;";
			else
				data+=currentString.at(j);
		}
	//	data+= ;
		data+="<br>";
	}
	/*
	while (*x && g >=0)
	{
		if (*x == 32)
			data+=" &nbsp;";
		else if (*x == 0xd)
			data+="<br>";
//		else if (*x == 0xa)
	//		data+="<br>";
		else if (*x == '<')
			data+="&lt;";
		else if (*x == '>')
			data+="&gt;";
		else
			data += *x;
		g--;
		x++;
	}
	*/
	html_controls.html_value("filesize",filesize);
	html_controls.html_value("filename",m.filename);
	
	html_controls.html_value("fid",fid);
	html_controls.html_value("data",data);
 
    html_controls.parsePage(TEXTVIEW);
}

void view_text_file(elfatFileData &m,string fid, string sessionid, char* p)
{
	view_text_file(m,fid,sessionid,"",p);	
}

void view_image_file(elfatFileData &m,string fid, string sessionid)
{
    html html_controls;
    
	html_controls.html_value("urlsess",sessionid);
	
	//html_controls.html_value("message",message);
	html_controls.html_value("filename",m.filename);
	string filesize = "";
	char c[10];
	if (m.fileSizeAsInt > 1024)
	{
		if (m.fileSizeAsInt > 1024*1024)
		{
			sprintf(c,"%i",	m.fileSizeAsInt/(1024*1024));
			filesize = c;
			filesize += "MB";
		}
		else
		{
			sprintf(c,"%i",	m.fileSizeAsInt/(1024));
			filesize = c;
			filesize += "kB";
		}
		
	}
	else
	{
		filesize = m.fileSize + "B";
	}
	html_controls.html_value("filesize",filesize);
	html_controls.html_value("filename",m.filename);
	
	html_controls.html_value("fid",fid);
 
    html_controls.parsePage(IMAGEVIEW);
}

void view_data_file(elfatFileData &m,string fid, string sessionid)
{
    html html_controls;
    
	html_controls.html_value("urlsess",sessionid);
	
	//html_controls.html_value("message",message);
	html_controls.html_value("filename",m.filename);
	string filesize = "";
	char c[10];
	if (m.fileSizeAsInt > 1024)
	{
		if (m.fileSizeAsInt > 1024*1024)
		{
			sprintf(c,"%i",	m.fileSizeAsInt/(1024*1024));
			filesize = c;
			filesize += "MB";
		}
		else
		{
			sprintf(c,"%i",	m.fileSizeAsInt/(1024));
			filesize = c;
			filesize += "kB";
		}
		
	}
	else
	{
		filesize = m.fileSize + "B";
	}
	html_controls.html_value("filesize",filesize);
	html_controls.html_value("filename",m.filename);
	
	html_controls.html_value("fid",fid);
 
    html_controls.parsePage(DATAVIEW);
}


#endif /* _EFSFUNCTIONS_H */
