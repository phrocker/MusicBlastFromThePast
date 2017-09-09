#ifndef IP_RESTRICT_H
#define IP_RESTRICT_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <arpa/inet.h>
//#include "string_functions.h"
using namespace std;
#include "dbase.h"
#include "Query.h"
#include "result_set.h"
#define DATFILE "/home/marc/data_files/general.dat"

class ip_restrict
{
    private:
        class ips
        {
            public:
            short int &operator[](int ndx)
            {
                if (ndx < 8)
                    return allow[ndx];
                else
                    return allow[7];
            }   
            private:
            short int allow[8];
        };
        ips bowel;
        unsigned int size;
        void load_ips(string fileName)
        {
            ifstream fopen(fileName.c_str());
            string octet="";
            int l=0;
            while(fopen)
            {
                if (l >= ipVec.size())
                {
                    ipVec.resize(l+1);
                }            
                fopen >> ipVec[l][0]>> ipVec[l][1]>> ipVec[l][2]>> ipVec[l][3]>> ipVec[l][4]>> ipVec[l][5]>> ipVec[l][6]>> ipVec[l][7];
             l++;

            }
            size=ipVec.size()-1;
            fopen.close();
        }
    public:
        bool test(const char *ip)
        {
	        if (strlen(ip) == 0) return false;
            char *cp = new char[strlen(ip)*2];
            retreive_values variables;
			variables.parse_file(DATFILE);
            Query query;
			query << "select `name` from `blockList` where `min` <= " << (unsigned long)inet_network(ip) << " and `max` >=" << (unsigned long)inet_network(ip);
			Result_Set results(query.str());
			DB_Handler database;
			
//					cout << "Content-type: text/html\n\n";
//			cout << query.str() << "  << " << ip;
//			exit(1);
			database.create_connection(variables.value("db_login"),variables.value("db_password"),variables.value("database"));
			if (database.execute_query(query,results))
			{
				//cout << "Hello";
				if (results.number_rows() > 0)
				{
					cout << results.number_rows();
						return false;
					}
			}
			else
			{
				//cout << "poop";
				return false;
			}
			database.disconnect();
			
            strcpy(cp,ip);

            char *pch;
            pch = strtok (cp,".");
            int octet = 0;
            bool found=false;
            int i=0;
            while (pch!=NULL)
            {
                 int si=atoi(pch);
                found=false;
                for (i =0; i < size; i++)
                {
    //                cout << size << " called " << " " << ipVec[i][octet] << " " << ipVec[i][octet+1] << " ";
                    if (si >=ipVec[i][octet] && si <=ipVec[i][octet+1])
                    {
  //                      cout << endl << " ---" << si << " " << ipVec[i][octet] << " " << ipVec[i][octet+1]<< endl;
                        found=true;
                        break;
                    }

                }
//                    cout << endl << i << endl;
                if( i ==size ) return false;

//      cout << endl;
        octet+=2;
        pch = strtok (NULL, ".");
         }
     delete [] cp;
    if (octet<8)
    {
       return found;
    }
//*/
        return found;

     }


        ip_restrict()
        {
            size=1;
            for (int i=0; i < 8; i++)
                bowel[i]=256;
            ipVec.resize(1);
            load_ips("/home/marc/data_files/iplist.dat");
        }

        ips &operator[](int ndx)
        {
            if (ndx < ipVec.size())
                return ipVec[ndx];
            else
            {
                ipVec.resize(ndx+1);
                return ipVec[ndx];
            }
        }
        
        int
        operator++()
        {
            ipVec.resize( ++size );
            return size;
        }
    public:
    vector< ips > ipVec;

    /*
    allow[0]=0;
    allow[1]=255; 
    allow[2]=0;
    allow[3]=255;
    allow[4]=0;
    allow[5]=255;
    allow[6]=0;
    allow[7]=255;
*/
};


#endif
