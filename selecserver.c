    /*
    ** selectserver.c -- a cheezy multiperson chat server
    */

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
    
    #include <fstream>
	#include <string>
	#include <sstream>
	#define USE_MYSQL 1
	#include "cgiparser.h"
	#include "cookies.h"
	#include "html.h"
	#include "loginmanage.h"
	#include "ip_restrict.h"
	#include "string_functions.h"
//	#include "log.h"
    using namespace std;
    
    #define PORT "8989"   // port we're listening on
    // small linked list of connections
    
    class connections
    {
        public:
            connections(string ip="",unsigned long la=0,string un="",bool ac=false): ip_address(ip),last_action(la),username(un),active(ac)
            {}
        string ip_address;
        unsigned long last_action;
        string username;
        bool active;    };    


    
#include "functions.h"
#include "MetaPhone.h"
#include "spellcheck.h"
#define DATA "/home/phrocker/data_files/music.dat"
cgiparser cgi_utilities;	
    LoginManage login;
    #include "mm.h"
retreive_values variables;


	ifstream openFile;
    
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
        HashTable<int,connections>   connectionList;
    	connectionList.Set(&itoi,10,connections("",0,"",false));
        fd_set read_fds; // temp file descriptor list for select()
        struct sockaddr_in myaddr;     // server address
        struct sockaddr_in remoteaddr; // client address
        int fdmax;        // maximum file descriptor number
        int listener;     // listening socket descriptor
        int newfd;        // newly accept()ed socket descriptor
        char buf[256];    // buffer for client data
        char *packetSend;
        int nbytes;
        int yes=1;        // for setsockopt() SO_REUSEADDR, below
        int addrlen;
        int i, j;

        FD_ZERO(&master);    // clear the master and temp sets
        FD_ZERO(&read_fds);

        // get the listener
        if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            create_log("Cannot capture port %i. Could be in use.",port);            
//            perror("socket");
            exit(1);
        }

        // lose the pesky "address already in use" error message
        if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes,
                                                            sizeof(int)) == -1) {
            create_log("Cannot re-capture port %i. Could be in use.",port);
//            perror("setsockopt");
            exit(1);
        }

        // bind
        myaddr.sin_family = AF_INET;
        myaddr.sin_addr.s_addr = INADDR_ANY;
        myaddr.sin_port = htons(port);
        memset(&(myaddr.sin_zero), '\0', 8);
        if (bind(listener, (struct sockaddr *)&myaddr, sizeof(myaddr)) == -1) {
              create_log("Cannot bind to port %i",port);
//            perror("bind");
            exit(1);
        }
        else
        {
                    create_log("Server listening on port %i",port);
        }        

        // listen
        if (listen(listener, 10) == -1) {
            create_log("Cannot listen on port %i",port);          
//            perror("listen");
            exit(1);
        }

        // add the listener to the master set
        FD_SET(listener, &master);

        // keep track of the biggest file descriptor
        fdmax = listener; // so far, it's this one

        // main loop
        for(;;) {
            read_fds = master; // copy it
            if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
                  create_log("error selecting connection");
//                perror("select");
                exit(1);
            }

            // run through the existing connections looking for data to read
            for(i = 0; i <= fdmax; i++) {
                if (FD_ISSET(i, &read_fds)) { // we got one!!
                    if (i == listener) {
                        // handle new connections
                        addrlen = sizeof(remoteaddr);
                        if ((newfd = accept(listener, (struct sockaddr *)&remoteaddr,(socklen_t *)&addrlen)) == -1) { 
                            create_log("cannot accept connection");
//                            perror("accept");
                        } else {
                            // test to see if ip address is banned
                            if (ip_banned(inet_ntoa(remoteaddr.sin_addr)))
                                close(newfd);
                            else
                            {
	                            string def = "/windows/f//B/Blink 182/Blink 182.mp3";
								string fs="1061276";
								//if (! check_ip() ) return 0;
	    						//permissions permission("1");
 								
                                create_log("user %s has connected",inet_ntoa(remoteaddr.sin_addr));
                                create_log("user %s has connected",getEnv("REMOTE_ADDR"));
                                create_log("stream id %s",cgi_utilities.cgivalue("STREAMID").c_str());
                                FD_SET(newfd, &master); // add to master set
                                if (newfd > fdmax) {    // keep track of the maximum
                                    fdmax = newfd;
                                }
                                connectionList[newfd] = connections();
                                connectionList[newfd].GetData().active=true;
                                connectionList[newfd].GetData().ip_address=inet_ntoa(remoteaddr.sin_addr);
                                connectionList[newfd].GetData().last_action=time(NULL);
                            }    
                        }
                    } else {
                        // handle data from a client
                        if ((nbytes = recv(i, buf, sizeof(buf), 0)) <= 0) {
                            // got error or connection closed by client
                            if (nbytes == 0) {
                                // connection closed
                                create_log("user %s closed connection",connectionList[i].GetData().ip_address.c_str());
                                  connectionList[i].GetData().active=false;
                            } else {
                                create_log("receive error. nbytes=%i",nbytes);
//                                perror("recv");
                            }
                            close(i); // bye!
                            FD_CLR(i, &master); // remove from master set
                        } else {
                            // we got some data from a client
                            //length should be the first two bytes
                            if (nbytes>=2)
                            {
                                int size=buf[0];

                                char data[ size - 2];
                                for (int k=0; k< size-2; k++)
                                {
                                    if (buf[k+2]==0x30)
                                        {
                                            data[k]='\0';
                                            break;
                                        }else
                                            data[k]=buf[k+2];
                                            
                                }   
                                switch( buf[1] )
                                {
                                    case 'A': 
                                        {
                                        // authenticate
                                        connectionList[i].GetData().username=data;
                                    
                                    }    
                                        break;
                                    case 'a':
                                        {
                                            // compare username and password
                                            char *packet=NULL;
                                        //    cout << connectionList[i].GetData().username << " " << data << endl;
                                            if (connectionList[i].GetData().username.length() > 0)
                                                if (connectionList[i].GetData().username=="phrocker" && !strcmp(data,"password"))
                                                    {
                                                         packet = new char[2];
                                                         packet[0]=0x1;
                                                         packet[1]='l';
                                                         int length=2;
                                                         if (sendall(i, packet,&length) == -1) {
                                                         delete [] packet;
                                                         exit(1);
                                                        }
                                                        delete [] packet;
                                                    }
                                                    else
                                                    {
                                                         packet = new char[2];                                         char *packet = new char[2];
                                                         packet[0]=0x1;
                                                         packet[1]='n';
                                                         int length=2;
                                                         if (sendall(i, packet,&length) == -1) {
                                                             
                                                         delete [] packet;
                                                         exit(1);
                                                        }    
                                                        delete [] packet;
                                                    }    
                                        }    
                                    case 'c':
                                        {
                                            if (!strcmp(data,"shutdown"))
                                                {
                                                    printf("exiting");
                                                    return 0;
                                                }    
                                        }
                                        break;    
                                   default: create_log("User sent %s",buf); break;
                                };    
                            }    
                            for(j = 0; j <= fdmax; j++) {
                                // send to everyone!
                                if (FD_ISSET(j, &master)) {
                                    // except the listener and ourselves


                                    if (j != listener && j != i) {
                                        if (send(j, buf, nbytes, 0) == -1) {
                                            create_log("Send Error");
                                        }
                                    }
                                }
                            }
                        }
                    } // it's SO UGLY!
                }
            }
        }

        return 0;
    } 
