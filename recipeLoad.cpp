#include <iostream>
#include <string>
using namespace std;
#define REMOVE 1
#define UPDATE 2

#define USE_MYSQL 1
#define DATA "/home/phrocker/data_files/music.dat"
#include "dbase.h"
#include "Query.h"
#include "result_set.h"
//#include "dbase.h"
#include "string_functions.h"

int main()
{
	ifstream load("recpes.txt");
	string title,recipe,garbage;
	Query query;
		retreive_values variables;
	    DB_Handler database;
	    variables.parse_file(DATA);
		database.create_connection(variables.value("db_user"),variables.value("db_password"),variables.value("database"));
		cout << "loading recipes" << endl;
	while (load)
	{
		
		getline(load,title);
		getline(load,recipe);
		getline(load,garbage);
		query.clear();
		query << "insert into recipes(Title,recipe,rating) values('" << database.escape(title) << "','" << database.escape(recipe) << "','0')";
		database.execute_query(query);
	}
	load.close();
}
