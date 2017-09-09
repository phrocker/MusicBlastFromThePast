/***************************************************************************
 *            testelfat.c
 *
 *  Sat Jun  4 18:00:46 2004
 *  Copyright  2004  User
 *  Email
 ****************************************************************************/

#include <iostream>
#include <string>
using namespace std;

#include "elfat.h"





int main()
{
		cout << "Welcome to the test elfat system " << endl;
		cout << "Please enter your pin: " << endl;
		string pin;
		cin >> pin;
		
		EncryptedLFAT fileTable("",pin);
		if (fileTable.accessFAT() == true )
		{	
			cout << "Thank you. Accessing files..." << endl;
			cout << "Listing Files: " << endl;
			register int i=0;
			while( fileTable.getNext() )
			{
				elfatFileData &m = fileTable.getCurrentFATData();
				cout << i+1 << ": " <<  m.filename << endl;
				i++;
			 //break;
			}
			if (i == 0) cout << "Directory Empty..." << endl;
			unsigned int options=0;
			while( options != 6)
			{
				cout << "Please choose from the following options" << endl;
				cout << "1) Add file" << endl;
				cout << "2) Remove file" << endl;
				cout << "3) List Files" << endl;
				cout << "6) Exit" << endl;
				cin >> options;
				switch( options )
				{
					case 1: 
						{
							cout << " Enter name of new file: ";
							string filename;
							cin >> filename;
							string data;
							cout << "Now, enter data for new file: " << endl;
							cin >> data;
							if (!fileTable.createFile(filename,(char*)data.c_str(),data.length()))
								cout << "Could not create file" << endl;

						}
						break;
					
					case 3:
					{
						cout << "Listing Files: " << endl;
						register int i=0;
						fileTable.beginListFS();
						while( fileTable.getNext() )
						{
							elfatFileData &m = fileTable.getCurrentFATData();
							cout << i+1 << ": " <<  m.filename << endl;
							i++;
						 //break;
						}
						if (i == 0) cout << "Directory Empty..." << endl;
					}
					case 2:
						{
							if (options == 3) break;
							cout << "Enter the number of the file you wish to delete" << endl;								
						}
						break;
					
					default:
						break;
				};
			}
			
		}
		else
			cout << "Could not access Files; Exiting..." << endl;
	
		
		return 0;
}
