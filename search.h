#ifndef SEARCH_H
#define SEARCH_H
#include <sstream>

#include <cstdlib>
#include "dbase.h"
#include "Query.h"
#include "result_set.h"
#include "rijndael_encryption.h"
using namespace std;

char *blacklist[]=
{
"about",
"and",
"as",
"at",
"from",
"in",
"inside",
"into",
"off",
"on",
"over",
"the",
"to",
"up",
"upon",
"with",
"within"
};
using namespace std;
class Search
{
    public:
        Search()
        {
            ksize = 0;
            keywords.resize(0);
        }
        string SearchFor(string ,unsigned int,unsigned int);
        void AddKeyWords(string ,string);
        void CreateKeyWords(string );
        struct bd
        {
            string gid;
            string body;
            
        };
    private:
        DB_Handler database;
        Query query;
        vector<string> keywords;
        vector<string> results;
        int ksize;
};

string
Search::SearchFor(string searchTerm, unsigned int begin = 0, unsigned int end =  10)
{
    if (end-begin < 0) return "No Results";
    unsigned int left=end-begin;
    
     CreateKeyWords(searchTerm);
//    DB_Handler db;
//db.create_connection("root","password","test");
  //  Query sq;
    //Result_Set res;
    if (keywords.size() == 0) return "No Results";
    query << "select distinct gid from Keywords where ";
    for (int i =0; i < keywords.size(); i++)
    {
            if (i!=0)
                query << " or ";
        query << " term like '%" << keywords[i] << "%' ";


        
    }
    query  << " or ";
       for (int i =0; i < keywords.size(); i++)
    {
            if (i!=0)
                query << " or ";
        query << " term like '%" << keywords[i] << "%' ";


        
    }
    string nots;
    nots=" and ";
    query << " limit " << begin << "," << end;
    Result_Set exact(query.str());
    //cout << query.str() << endl;
    database.execute_query(query,exact);
    
    DB_Handler db;
    db.create_connection(variables.value("db_login"),variables.value("db_password"),variables.value("database"));
    stringstream returnstring;
    
    Result_Set rs;
    Query myquery;
    
        //cout << "---Exact Matches: " << exact.number_rows() << " row(s) returned"  << endl;

    int st=0;
    if (exact.number_rows() > 0)
    {
    
        do
        {   
            if (
//            res.clear();
  //          res << "select title,caption from logs where 
            nots += " gid != '" ;
            nots += exact[0];
            nots += "' and ";
            //cout << "    " << exact[0] << endl;
		  myquery.clear();
		  myquery << "select title,caption from logs where messid='" << exact[0] << "'";
		  rs.SetQuery(myquery.str());
		  db.execute_query(myquery,rs);
		  returnstring << "----<b>" << rs[0] << "</b><br>";
    	   rijndael_encryption encryptor("jsn39ghalq01957mbnsk3067hgnzadje");
               string ms=cc_decrypt(rs[1]);
                string us="";                                
                us=encryptor.decrypt_text(ms);

		   returnstring << us << "<br>";
        }while (exact++);
    }
    string qry = query.str();
    query.clear();
    query << "select distinct gid from Keywords where (term like '";
    for (int i =0; i < keywords.size(); i++)
    {
        if (keywords[i].length() > 3)
        {
            query << "%" << keywords[i].substr(0,3);
            query << "%'";
            if (i!=keywords.size()-1)
                query << " or term like '";
        }
        else
        {
            query << "%" << keywords[i] << "%' ";
            if (i!=keywords.size()-1)
                query << " or term like '";
        }
    }
    query << " or alias like '";
    for (int i =0; i < keywords.size(); i++)
    {
        if (keywords[i].length() > 3)
        {
            query << "%" << keywords[i].substr(0,3);
            query << "%'";
            if (i!=keywords.size()-1)
                query << " or alias like '";
        }
        else
        {
            query << "%" << keywords[i] << "%' ";
            if (i!=keywords.size()-1)
                query << " or alias like '";
        }
    }
    query << ") ";
    if (exact.number_rows() > 0)
        query << nots.substr(0,nots.length()-5);
    query << " limit " << begin << "," << end;
    //cout << query.str() << endl;
    Result_Set fuzzy(query.str());
    database.execute_query(query,fuzzy);
    //cout << endl << "---Other Matches: " << fuzzy.number_rows() << " row(s) returned"  << endl;
    if (fuzzy.number_rows() > 0)
    {
        do
        {
	   		myquery.clear();
		  myquery << "select title,caption from logs where messid='" << fuzzy[0] << "'";
		  rs.SetQuery(myquery.str());
		  db.execute_query(myquery,rs);
returnstring << "----<b>" << rs[0] << "</b><br>";
               string ms=cc_decrypt(rs[1]);
                string us="";                                
                us=encryptor.decrypt_text(ms);

		   returnstring << us << "<br>";
            //cout << "    " << fuzzy[0] << endl;
        }while (fuzzy++);
    }
    
    return returnstring.str();

}

void
Search::CreateKeyWords(string searchTerm)
{
    char *cp = new char[searchTerm.length()];
    strcpy(cp,searchTerm.c_str());

  char *pch;
  pch = strtok (cp," ");
  while (pch!=NULL)
  {
    if (strlen(pch) >= 3) 
    {
        bool add=true;
        for (int i =0; i<=15; i++)
        {
            if (!strcmp(pch,blacklist[i]))
            {
                 add=false;
                 break;
            }
        }
        if (add)
        {
           keywords.resize(ksize+1);
            keywords[ksize]=pch;
            ksize++;
        }
    }
    pch = strtok (NULL, " ");
  }
    delete [] cp;
}

void
Search::AddKeyWords(string message,string gid)
{
    CreateKeyWords(message);

    for (int c=0; c < ksize; c++)
    {
        query.clear();
        query << "insert into `Keywords` (term,gid) values('" << keywords[c] << "','" << gid << "');";
        database.execute_query(query);    
    }

}

#endif
