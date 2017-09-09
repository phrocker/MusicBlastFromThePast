#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <string>
#define PERMISSION_DAT "/home/marc/data_files/permissions.dat"
#include "dbase.h"
#include "Query.h"
#include "result_set.h"
#include "HashTable.h"
#include "retreive_values.h"
using namespace std;

class permissions
{
    public:
        permissions(){perdat.parse_file(PERMISSION_DAT);
	 database.create_connection(perdat.value("db_user"),perdat.value("db_password"),perdat.value("database"),perdat.value("host") );}
        permissions(string);
        bool permission(string);
        void Load_Permissions(string);
        unsigned int count_permissions(string);
    private:
      DB_Handler database;
      Query query;
        retreive_values perdat;
      Result_Set results;
     HashTable<string,bool> permission_hash;
};

permissions::permissions(string gid)
{
      perdat.parse_file(PERMISSION_DAT); database.create_connection(perdat.value("db_user"),perdat.value("db_password"),perdat.value("database"),perdat.value("host"));
    Load_Permissions(gid);
}

void
permissions::Load_Permissions(string groupID)
{
	permission_hash.Set(&stoi,count_permissions(groupID)+10,false);
    query.clear();
    query << "select name,value from Permissions where group_id='" << groupID << "'";
    results.SetQuery(query.str());
    database.execute_query(query,results);
    if (results.number_rows() > 0)
    {
    do
    {
        if (results[1]=="1")
            permission_hash[ results[0] ] = true;
        else
            permission_hash[ results[0] ] = false;        
    }while(results++);
    }
}

unsigned int
permissions::count_permissions(string groupID)
{
	query.clear();
    query << "select count(*) as permission_count from Permissions where group_id='" << groupID << "'";
    results.SetQuery(query.str());
    database.execute_query(query,results);
//    cout << atoi( results[0].c_str() );
    return atoi( results[0].c_str() );
}

bool
permissions::permission(string name)
{
    return permission_hash[name].GetData();
}

#endif
