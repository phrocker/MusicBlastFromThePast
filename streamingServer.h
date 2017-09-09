/***************************************************************************
 *            streamingServer.h
 *
 *  Sun Oct 30 17:54:52 2004
 *  Copyright  2004  User
 *  Email
 ****************************************************************************/


#ifndef STREAMINGSERVER_H
#define STREAMINGSERVER_H 1

#include <pthread.h>


#define MAXCONNECTIONS 100
#define CONTINUOUSQUEUE 0x00
#define RADIO 0x01
#define QUEUE 0x02
    
#define PORT "8989"   // port we're listening on
    // small linked list of connections

class connections
{
	public:
    connections(string ip="",bool ac=false): ip_address(ip), active(ac)
    {
		openFile = NULL;
	}
    string ip_address;
    bool active;    
    FILE *openFile;
	pthread_t localThread;
};  

#define SPEED

unsigned int totalConnections=0;

string d="";

HashTable<int,connections*>   connectionList;


#define DATA "/home/marc/data_files/music.dat"

retreive_values variables;


FILE *GetFileRef(string filepath,int i)
{
	if (connectionList[i].GetData()->active==true)
	{
		return fopen(filepath.c_str(), "rb");
		
	}else
	{ 
		return NULL; 
	}
}




bool
streamData(FILE *charlie,string filesize,string filename,int i,bool reverse)
{
		if (!charlie) return false;
		string s="";
		s="HTTP/1.0 200 OK\r\nServer: WebSource Streaming Data Server\r\nContent-Disposition: attachment ; filename=" + filename + "\r\nContent-type: mpeg/audio\r\n\r\n";
		
		if (send(i, s.c_str(), s.length(), 0) == -1) return false;
			
  		long  size=atol(filesize.c_str());

		char *buffer;
		
		buffer= new char[size+1];
		
		long c = fread(buffer,1,size,charlie);
		
		sendall(i,buffer,c);
		
		delete [] buffer;
		
		fclose(charlie);
	return true;
}



bool playQueue(string &streamID,string &keyid,DB_Handler &database,Query &query,int &i,bool deleteWhenFinished,string &keyHash)
{
	query.clear();
	query << "select timestamp,location,filename,filesize,queueNum from stream where streamID='" << streamID << "' and timestamp >=" << time(NULL) << "  order by queueNum asc limit 0,1";		
	Result_Set results(query.str());
	Result_Set result(query.str());
	//	cout << "Content-type: text/html\n\n";
	
	query.clear();
	int loc=i;
	if (keyHash=="")
	query << "select timestamp,location,filename,filesize,queueNum from stream where streamID='" << streamID << "' and timestamp >= " << time(NULL) << " and queueNum >= " << ++loc << "  order by queueNum asc limit 0,1";
	else
	{
		query << "select timestamp,location,filename,filesize,queueNum from stream where streamID='" << streamID << "' and timestamp >= " << time(NULL) << " and `keyHash` = '" << keyHash << "'  order by queueNum asc limit 0,1";
	}
		
	create_log("about to execute %s",query.str().c_str());
	if (database.execute_query(query,results))
	{
		if (results.number_rows() > 0)
		{
			do
			{
				string filename= results["location"] + results["filename"];
				query.clear();
//				if (deleteWhenFinished==true)
					query << "update stream set timestamp=" << time(NULL)+7200 << " where queueNum=" << results[4] << " and streamID='" << streamID << "'";
//				else	
				if (!database.execute_query(query)) create_log("database error");
				query.clear();
				
				query << "update statistics set size = size+" + results["filesize"] + ",downloads=downloads+1";
				if (!database.execute_query(query)) create_log("database error");
				create_log("playing %s",results["filename"].c_str());
				if (!streamData(GetFileRef(filename,i),results["filesize"],results["filename"],i,false)) return false;
				loc = atoi(results[4].c_str() );
			}while( results++ );
		}
		else
		{	
			return false;					
			

		}
	}
	else
	{
		return false;
	}	     	
	create_log("user %s has finished session",connectionList[i].GetData()->ip_address.c_str());   
	query.clear();
	return true;                 
}

bool playQueue(string &streamID,string &keyid,DB_Handler &database,Query &query,int &i,bool deleteWhenFinished)
{
	string emptyHash="";
	return playQueue(streamID,keyid,database,query,i,deleteWhenFinished,emptyHash);
}

bool playContinousQueue(string &streamID,string &keyid,DB_Handler &database,int &i,Query &query)
{
	return playQueue(streamID,keyid,database,query,i,false);
}

bool playSingleSession(string &streamID,string &keyid,DB_Handler &database,int &i,Query &query)
{
		return playQueue(streamID,keyid,database,query,i,true);
}

bool playRadio(string &streamID,string &keyid,DB_Handler &database,int &i,Query &query)
{

	
	query << "select timestamp from stream where streamID='" << streamID << "' and timestamp >=" << time(NULL);
	Result_Set results(query.str());	
		int loc=0;
	if (database.execute_query(query,results))
	{
		if (results.number_rows() > 0)
		{
			query.clear();
			query << "select id,location,filename,filesize from music order by rand() limit 0,1";
			//query << "select id,location,filename,filesize from music where id=" + keyid + " limit 0,1";
		//	create_log("query %s",query.str().c_str());
			//Result_Set result(query.str());	
			results.SetQuery(query.str());
			while(1)
			{   
				if (playSingleSession(streamID,keyid,database,i,query) == true)
					continue;
				query.clear();
			//	query << "select id,location,filename,filesize from music where id=" + keyid + " limit 0,1";
				query << "select id,location,filename,filesize from music order by rand() limit 0,1";
				results.SetQuery(query.str());
				//create_log("query %s",query.str().c_str());
				if (database.execute_query(query,results))
				{
						if (results.number_rows() > 0)
						{
							//cout << "file size is "<< results["filesize"]<<" " << results[3] << endl;
							string filepath = results["location"]+results["filename"];
							query.clear();
							query << "update statistics set size = size+" + results["filesize"] + ",downloads=downloads+1 where id=1";
							if (!database.execute_query(query)) create_log("database error");
							query.clear();
							query << "update stream set timestamp=" << time(NULL)+7200 << " where streamID='" << streamID << "'";
							if (!database.execute_query(query)) create_log("database error");
							string filesize = results[3];

							if (!streamData(GetFileRef(filepath,i),results["filesize"],results["filename"],i,false)) return false;
						}
						else
						{
							//create_log("number of rows is zero 2");
							return false;
						}
				}
			}
		}
		else
		{
			//create_log("number of rows is zero 1");
			return false;
		}
	}
}





bool
sendMusic(int &i,string &streamID,char &radio,string &keyid)
{
	Query query;
	variables.parse_file(DATA);		
	DB_Handler database;
	database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"));
	bool start=false;
	int loc=0;
	if (radio==RADIO)
	{
		playRadio(streamID,keyid,database,i,query);
	}
	else if (radio==QUEUE)
	{
		string hash = MD5String((char*)keyid.c_str());
		playQueue(streamID,keyid,database,query,i,true,hash);
	}
	else
	{
		while(1)
			if (!playContinousQueue(streamID,keyid,database,i,query)) break; // play single iteration of queue
	}
		return true;
		
} 





bool
sendMusic(int a,string b,char c)
{
	return sendMusic(a,b,c,d);
}    

pthread_mutex_t mainMutex;

void *server_connection(void *fd)
{
	
	
	int fileDescriptor = *((int*)fd);
	
	int numBytes=0;
	
	
	
	char buf[512]="";
	
	
	while( (numBytes = recv(fileDescriptor, buf, sizeof(buf), 0)) > 0)
	{
	
		if (numBytes>=63) // 57 should be up to the HTTP/1.0
		{
			create_log("User %s sent %s",connectionList[fileDescriptor].GetData()->ip_address.c_str(),buf);
			string keyID="";
			if (buf[0] == 'G' && buf[1] == 'E' && buf[2]=='T')
			{
				char *x = new char[256];
				for (int j =0;j < 256; j++)
					x[j]=buf[j];
				char *l=x;
				
				
				l+=4; // move 4 to the right
				char radio=CONTINUOUSQUEUE;
				if (*l=='/')
				{
					if (*(l+1)=='r' && *(l+2)=='k' && *(l+3) == '/')
					{
						radio=RADIO;
						l+=4;
						int a = 0;
						while( *(l+a) >= 48 && *(l+a) <= 57)
						{
							keyID+= *(l+a);
							l++;
						}	

						create_log("User has sent key %s", keyID.c_str() );		
	
					}
					else  if (*(l+1)=='q' && *(l+2)=='u' && *(l+3) == '/')
					{
						radio=QUEUE;
				    	l+=4;
						int a = 0;
						while( *(l+a) >= 48 && *(l+a) <= 57)
						{
							keyID+= *(l+a);
							l++;
						}	
					
						create_log("User has sent key %s", keyID.c_str() );		
					}
				
					l++;
					string streamID="";
			    	for (int j =0; j <45; j++,l++)
			    		streamID+=*l;
			    
					create_log("User %s send stream id %s",connectionList[fileDescriptor].GetData()->ip_address.c_str(),streamID.c_str());

					if (!sendMusic(fileDescriptor,streamID,radio,keyID))
					{
						create_log("Could not send data to %s; user could have closed connection",connectionList[fileDescriptor].GetData()->ip_address.c_str());
			    		close(fileDescriptor); // bye!
						connectionList[fileDescriptor].GetData()->active=false;
						delete connectionList[fileDescriptor].GetData();
						connectionList[fileDescriptor].GetData()=NULL;
						return NULL;
					}
				
				}
			}
			else
			{
				create_log("user %s sent malformed packet for queue. Killing connection",connectionList[fileDescriptor].GetData()->ip_address.c_str());
		    	connectionList[fileDescriptor].GetData()->active=false;
		    	close(fileDescriptor); // bye!
				delete connectionList[fileDescriptor].GetData();
				connectionList[fileDescriptor].GetData()=NULL;
				return NULL;
			}
		}
		else
		{
			create_log("user %s sent malformed packet. Killing connection",connectionList[fileDescriptor].GetData()->ip_address.c_str());
			connectionList[fileDescriptor].GetData()->active=false;
			close(fileDescriptor); // bye!
			delete connectionList[fileDescriptor].GetData();
			connectionList[fileDescriptor].GetData()=NULL;
			return NULL;
		}
	}
	
	if (numBytes == 0) 
	{
		create_log("user %s closed connection",connectionList[fileDescriptor].GetData()->ip_address.c_str());
	}
	else 
	{
		create_log("receive error. nbytes=%i",numBytes);
		
	}
	
	connectionList[fileDescriptor].GetData()->active=false;
	
	delete connectionList[fileDescriptor].GetData();
	
	connectionList[fileDescriptor].GetData()=NULL;
	
	close(fileDescriptor); // bye!

}









#endif
