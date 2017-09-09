#include <iostream>
#include <string>

//#include "HashTable.h"
//#include "hash.h"

//#include "html.h"

using namespace std;
//#include <sqlplus.hh> 


//unsigned int stoi(string&);

int main(int argc, char *argv[])
{
  system("clear");
/*
    html html_controls;
    html_controls.create_layer("/var/www/html/success_dat.htm","my_paste");
    html_controls.layer("my_paste")["username"]="phrocker";
    html_controls.layer("my_paste")["domain"]="phrocker.no-ip.info";
    html_controls.finalize_layer("my_paste");
    html_controls.parsePage("/var/www/html/test.htm");
*/
//    retreive_values catchs;
//    catchs.parse_file("doggy.txt");
    
/*
  HashTable<string,long> SHash(&stoi,4,0);
    //cout << SHash.SearchKey("large").GetKey() << endl;
    
    cout << "Adding entries..." << endl;
cout << SHash["dog"].GetData() << endl;
    SHash["dog"] = 11;
    cout << SHash["dog"].GetData() << endl;
    SHash["dog"] = 1;
    cout << SHash["dog"].GetData() << endl;
   
   SHash["cat"] = 135;
   SHash["lima"] = 235;

   SHash["poop"] = 147631887;
   SHash["frog"] = 147631887;
   SHash["izzard,eddie"] = 427637911;
 if (SHash["frog"] ==   SHash["poop"])
    cout << "They is equal" << endl;
else
    cout << "They are not equal" << endl;
    cout << "Added four string entries. Attempting to regurgitate now." << endl;
    cout <<  SHash["poop"].GetData() << " "  << SHash["izzard,eddie"].GetData() << " " << SHash["dog"].GetData() << " " <<   SHash["cat"].GetData() << " " << SHash["lima"].GetData() << " " << SHash["frog"].GetData()  <<  endl;
/* 
	if (SHash["poodp"].GetStatus() == EMPTY)
		cout << SHash["poodp"].GetKey() << " is empty" << endl; 
    cout << SHash["poodp"].GetStatus() << endl;
//	cout << SHash["poop"];Man
*/
return 0;
}




