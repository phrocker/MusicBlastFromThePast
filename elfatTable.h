/***************************************************************************
 *            elfatTable.h
 *
 *  Sun May 29 15:42:17 2004
 *  Copyright  2004  User
 *  Email
 ****************************************************************************/

#ifndef _ELFATTABLE_H
#define _ELFATTABLE_H

#include <vector>
#include <functional>      // For greater<int>( )
#include <algorithm>
#include <list>
#include <string>
using namespace std;

#include "HashTable.h"

#include "elfatFileData.h"




class elfatTable
{

	public:
		elfatTable();
		void put_elFile(elfatFileData);
		// string should be a hash of the filename
		elfatFileData &get_elFile(string);
	
		list<string > &get_elfatList();
	
		vector< string > &get_directoryVector(){ return directories; }
		
		vector< string > &get_filesVector(){ return files; }
	private:
		// hash for directories and files
		vector< string > directories;
		vector< string > files;
		list < string  > tempList;
		HashTable<string,elfatFileData>   fatList;
		void quicksort(vector< string> &,int,int);
		int partition(vector <string> &array, int top, int bottom);
		bool CheckName( vector<string>::iterator&,vector<string>::iterator&);
		

};

int 
elfatTable::partition(vector <string> &array, int top, int bottom)
{
     string x = ((elfatFileData &)fatList[ array[top] ].GetData()).filename;
     int i = top - 1;
     int j = bottom + 1;
     string temp;
     do
     {
		           do  
         {
                 i++;
          } while (x > ((elfatFileData &)fatList[ array[i] ].GetData()).filename);
		  
           do     
           {
			   
                  j--;
           }while (x < ((elfatFileData &)fatList[ array[j] ].GetData()).filename);



          if (i < j)
         { 
                 temp =array[i];    // switch elements at positions i and j
			 array[i] = array[j];
			 array[j] = temp;
                 //((elfatFileData &)fatList[ array[i] ].GetData()).filenameHash = ((elfatFileData &)fatList[ array[j] ].GetData()).filenameHash;
                 //((elfatFileData &)fatList[ array[j] ].GetData()).filenameHash = temp;
         }
     }while (i < j);    
     return j;           // returns middle index
}

void 
elfatTable::quicksort(vector< string> &f, int top, int bottom)
{
      // uses recursion-the process of calling itself
     int middle;
     if(top<bottom)
    {
		
          middle = partition(f, top, bottom);
          quicksort(f, top, middle);   // sort top partition
          quicksort(f, middle+1, bottom);    // sort bottom partition
     }
     return;
}



elfatTable::elfatTable()
{
	fatList.Set(&stoi,10,elfatFileData());
}
bool
elfatTable::CheckName(vector<string>::iterator& one, vector<string>::iterator& two)
{
	 return ( ((elfatFileData &)fatList[ *one ].GetData()).filename > ((elfatFileData &)fatList[ *two ].GetData()).filename );
}
void
elfatTable::put_elFile(elfatFileData inO)
{
	string fileNameAsHash = inO.filenameHash;
	fatList[ fileNameAsHash ] = inO;
	if ( inO.type == ELFILE )
		files.push_back( fileNameAsHash );
	else
		directories.push_back( fileNameAsHash );
	
}

elfatFileData &
elfatTable::get_elFile( string inH)
{
	return fatList[ inH ].GetData();
}
	

list< string > &
elfatTable::get_elfatList(  )
{
	vector<string>::iterator fileIterator = files.begin();
	tempList.resize( 0 );
	//vector<string>::iterator directoryIterator = directories.begin();
 
 	//for( directoryIterator = directories.begin(); directoryIterator != directories.end(); directoryIterator++ )    
	quicksort(directories,0,directories.size()-1);
	for (int i=0; i < directories.size(); i++)
	{
		tempList.push_back( directories.at(i) );
   	}
	
//	vector<string>::iterator fileIterator = files.begin();
 	//cout << "Content-type: text/html\n\n";
	quicksort(files,0,files.size()-1);
	//sort( files.begin( ), files.end( ), CheckName );
 //	for( fileIterator = files.begin(); fileIterator != files.end(); fileIterator++ )    
	for (int i=0; i < files.size(); i++)
	{
		tempList.push_back(  files.at(i)  );
   	}
	
	

	// tempList should contain directories followed by files at this pointer
	
	return tempList;
}




#endif /* _ELFATTABLE_H */
