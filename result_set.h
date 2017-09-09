#ifndef RESULT_SET
#define RESULT_SET

#include <iostream>
#include <string>
#include <vector>
#include "HashTable.h"
using namespace std;
enum SET_STATUS{RACTIVE=0,FREE};
class Result_Set
{
    public:
        Result_Set(string = "");
        void SetQuery(string query){ my_query=query; hashActive=false; }
        string &operator[](string); // get K type and return reference to D of vector
        string &operator[](unsigned int);
        void freeData();
        bool captureSize();
        unsigned long number_rows(){ return numRows; }
        unsigned int number_columns(){ return numColumns; }
        bool operator++(int);
        bool operator--(int);
        bool SetActive()
        {
            if (status==RACTIVE) return false;
            status=RACTIVE;
            return true;
    	}
    private:
        SET_STATUS status;
        unsigned long numRows;
        unsigned int numColumns;
	   unsigned int currentRow;
    	vector<vector <string> > rows;
	   string my_query;
    	bool hashActive;
        string null;
    	HashTable<string,int> columns;
	   string columnSanctify(string);
        friend class DB_Handler;
    

       #ifdef USE_MYSQL
            #include "mysql_result_set.h"	
       #endif
};

Result_Set::Result_Set(string query)
{
	my_query=query;
	status=FREE; 
	hashActive=false;
    unsigned int numColumns=0;
    unsigned int numRows = 0;
    null="";
}

string
Result_Set::columnSanctify(string column)
{
   string::size_type select=    column.find("as ",0);
    if (select !=string::npos)
        return columnSanctify( column.substr(select+3,column.length()-(select+3)));
    int begin=0,end=column.length();
    bool atend=false;

       for (int i =0; i< column.length(); i++)
        {
            if (column[i]==32 && !atend )
                begin++;
            else if (column[i]==32 && atend)
                end=i-1;
            if ( (column[i]>=65 && column[i]<=90) || (column[i]>=97 && column[i]<=122) )
                atend=true;
         }
    return column.substr(begin,end);
}


       #ifdef USE_MYSQL
            #include "mysql_result_set.hh"	
       #endif
       

#endif
