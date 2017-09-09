#ifndef LIB_H
#define LIB_H

#define LOG_LOCATION "/var/log/binarystream/streamingServer.log"
#define BAN_FILE "/home/marc/data_files/ban.dat"
#include <stdarg.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
void
create_log(char *instring,...)
{
	string query_handler = "";
    //char *to;
	const	char *get;
	unsigned long int iget = 0;
	double dget = 0.0;
	string sget = "";
	va_list ap;
	va_start(ap, instring);
	char *to_string = "";

	while (*instring)
	{
		if (*instring == 37)
		{
			switch (*(instring+1))
			{
				case 83 :
					get = va_arg(ap,const char *);
					query_handler += get;
//					*query_handler << get;
					get = "";
					break;
				case 115 :
					get = va_arg(ap,const char *);
					query_handler += get;
//					*query_handler << to_string;
					get = "";
					break;
				case 99 :
					get = va_arg(ap,const char *);
					query_handler += get;
//					*query_handler << to_string;					
					get = "";
					break;
				case 105:
				    {
					iget = va_arg(ap,unsigned long int);
                      char *to= new char[sizeof(int)*8];
                    sprintf(to,"%i",iget);
					query_handler += to;
                    delete [] to;
					//query_handler << iget;
					iget = 0;
                	}				
					break;
				case 100:
					dget = va_arg(ap,double);
                    char *to= new char[sizeof(double)*8];
                    sprintf(to,"%d.2f",dget);
					query_handler += to;
                    delete [] to;
					//query_handler << dget;
					dget = 0;
					break;
			}
			instring= instring+2; continue;
		}
		query_handler += *instring;
//		*query_handler << *instring;
		instring++;
	}
		va_end(ap);
	
    ofstream file_write(LOG_LOCATION,ios::app);
    if (file_write)
    {
        file_write << time(NULL) << ": " << query_handler << endl;
        file_write.close();
    }    

}

unsigned int
itoi(int &d){ return d; }
bool
ip_banned(string ip_address)
{
    return false;
    /*
    ifstream ban_file(BAN_FILE);
    while(ban_file)
    {
        
    } 
    */   
}   
#endif
