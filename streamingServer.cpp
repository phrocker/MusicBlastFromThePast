#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "HashTable.h"
#include "definitions.h"
#include "lib.h"
#include "md5.h"

#include <string>
#include <sstream>
#define USE_MYSQL 1
#include "cgiparser.h"
#include "cookies.h"
#include "html.h"
#include "loginmanage.h"
#include "ip_restrict.h"
#include "string_functions.h"

using namespace std;

#include "functions.h"
#include "streamingServer.h"

int main(int argc,char *argv[])
{

	
	short unsigned int port=atoi(PORT);
    if (argc > 1)
	{
		// capture port number from argument list
		if (atoi(argv[1]) > 1024)
			port=atoi(argv[1]);
	}    

	fd_set master;   // master file descriptor list
	connectionList.Set(&itoi,MAXCONNECTIONS,NULL);
	fd_set read_fds; // temp file descriptor list for select()
	struct sockaddr_in myaddr;     // server address
	struct sockaddr_in remoteaddr; // client address
	int fdmax;        // maximum file descriptor number
	int listener;     // listening socket descriptor
	int newfd;        // newly accept()ed socket descriptor
	char buf[1024];    // buffer for client data
	char *packetSend;
	int nbytes;
	int yes=1;        // for setsockopt() SO_REUSEADDR, below
	int addrlen;
	int i, j;

	FD_ZERO(&master);    // clear the master and temp sets
	FD_ZERO(&read_fds);

	if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		create_log("Cannot capture port %i. Could be in use.",port);            
		exit(1);
	}

	if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes,
														sizeof(int)) == -1) {
		create_log("Cannot re-capture port %i. Could be in use.",port);
		exit(1);
	}

	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = INADDR_ANY;
	myaddr.sin_port = htons(port);
	memset(&(myaddr.sin_zero), '\0', 8);
	if (bind(listener, (struct sockaddr *)&myaddr, sizeof(myaddr)) == -1) {
		create_log("Cannot bind to port %i",port);
		exit(1);
	}
	else
	{
				create_log("Server listening on port %i",port);
	}        

	
	if (listen(listener, MAXCONNECTIONS) == -1) {
		create_log("Cannot listen on port %i",port);          
		exit(1);
	}

	FD_SET(listener, &master);

	
	fdmax = listener; // so far, it's this one
	create_log("Server starting at port %i...",port);
	
	// main loop
	for(;;) 
	{
		read_fds = master; // copy it
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
        	create_log("error selecting connection");
            exit(1);
        }
	
		
        // run through the existing connections looking for data to read
        for(i = 0; i <= fdmax; i++) 
		{
			if (FD_ISSET(i, &read_fds)) 
			{ // we got one!!
				if (i == listener)
				{
					// handle new connections
					
					addrlen = sizeof(remoteaddr);
					
					if ((newfd = accept(listener, (struct sockaddr *)&remoteaddr,(socklen_t *)&addrlen)) == -1) 
					{
						create_log("cannot accept connection");
					}
					else 
					{
						ip_restrict ip_check;
						if (!ip_check.test(inet_ntoa(remoteaddr.sin_addr)))
						{
								close(newfd);
								create_log("%s is banned",inet_ntoa(remoteaddr.sin_addr));
						}
						else
						{
							
							create_log("user %s has connected with FD %i, listener is %i",inet_ntoa(remoteaddr.sin_addr),newfd,listener);
							
							
							connectionList[newfd] = new connections();
							connectionList[newfd].GetData()->active=true;
							connectionList[newfd].GetData()->ip_address=inet_ntoa(remoteaddr.sin_addr);
							pthread_create( &connectionList[newfd].GetData()->localThread, NULL,server_connection, (void*)&newfd);
							
							
						}    
					}
                        
                          
				}
			} // it's SO UGLY!
		}
	}
	
	return 0;
}
