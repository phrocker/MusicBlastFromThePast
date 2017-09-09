/***************************************************************************
 *            elfatFileData.h
 *
 *  Fri Jun  3 22:34:56 2004
 *  Copyright  2004  User
 *  Email
 ****************************************************************************/

#ifndef _ELFATFILEDATA_H
#define _ELFATFILEDATA_H

#include <iostream>
#include <string>

using namespace std;
enum ELFILETYPE{ELFILE=0,ELDIRECTORY=1};

class elfatFileData
{
	public:
		elfatFileData()
		{
			filename = filenameHash = fileSize = "";
			contentType="text/html";
			fileSizeAsInt=type=ELFILE;
			memset(key,32,32);
			
		}
		string filename;
		string filenameHash;
		string fileSize;
		string contentType;
		int fileSizeAsInt;
		char key[32];
		int type;
	friend ostream &operator<<(ostream &,elfatFileData &);
};

ostream &operator<<(ostream &d ,elfatFileData &e)
{
	d << e.filename << " | " << e.filenameHash << " | " << e.fileSize << " | " << e.type;
}

#endif /* _ELFATFILEDATA_H */
