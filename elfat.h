#ifndef ELFAT
#define ELFAT


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>

using namespace std;

#include <botan/botan.h>

#include "elfatTable.h"
#include "md5.h"
#include "string_functions.h"
#include "String.h"

#define KEYL 32 // 256 bit encryption
#define ELFATMARKER "ELFATBS"
class EncryptedLFAT
{
public:
	EncryptedLFAT( string file, string userPIN )
	{
		
		localFile = file;
		pin = userPIN;
		string extraPin = MD5String( (char*)(pin.c_str()));
		for(int i=0,j=-1; i<KEYL; i++)
		{
			if (i < pin.length() )
				PIN[i] = pin.at(i);
			else
				PIN[i] = extraPin.at( ++j );
		}
		localList = NULL;
		correctPIN=false;
		//key = new Botan::SymmetricKey(PIN,KEYL);
		ReadFAT();
		
		finalized=true;
	}
	
	bool createFile(string,string, char *, int );
	string createDirectory(string);
	
	bool accessFAT();
	
	bool getNext();
	
	elfatFileData &get_elFile( string h){ return fileTable.get_elFile( h ); }
	
	
	elfatFileData &getCurrentFATData();
	
	void ReadFAT();
	
	void beginListFS();
	
	bool deleteFAT(string);
	
	char *getFileData(string);
	
private:
	elfatTable fileTable;

	string offerFAT(string,string,unsigned int, char *, int );

	

	void writeFAT();

	bool finalized;
	string localFile;
	string pin;
	list< string > *localList;
	list<string>::iterator listiterator;

	//Botan::SymmetricKey *key; //(PIN,KEYL);   //instantiate key
    Botan::byte PIN[KEYL];
	void ParseFAT( String &);
    bool correctPIN;

	

};

char*
EncryptedLFAT::getFileData(string hash)
{
	elfatFileData &m = fileTable.get_elFile(hash);
	
	if (m.filenameHash != hash)
		return NULL;
	
	try
	{
		Botan::byte localKey[KEYL];
		for (int i=0; i < KEYL; i++)
		{
	//		cout << m.key[i];
				localKey[i] = m.key[i];	
		}
		string fle = "." + hash;
		fle= localFile+fle;
		ifstream readFile(fle.c_str() ,  ios::binary);
//		cout << fle;
		if (!readFile) return NULL;
		Botan::LibraryInitializer init("thread_safe use_engines");
		Botan::SymmetricKey key(localKey,KEYL);   //instantiate key
	
		Botan::Pipe pipe(Botan::get_cipher("AES/ECB", key, 
		Botan::DECRYPTION)); //create pipe !exeption here!

		pipe.start_msg();
		//pipe.write((const Botan::byte*) data,size);	
		//pipe.write(addString);
		readFile >> pipe;
		//pipe << readFile;
		pipe.end_msg();
		char *data = new char[ m.fileSizeAsInt ];
	//	cout << "successfully read";
		pipe.read((Botan::byte*)data,m.fileSizeAsInt);
//		cout << "successfully read";
		
//		file << pipe;
		readFile.close();
		return data;
		/*
		
		
		*/

	
	} catch (std::exception &e) 
	{
		cout << e.what();
		std::cerr << "exc caught: " << e.what() << std::endl;
		return NULL;
	}

	
}

bool
EncryptedLFAT::deleteFAT(string hash)
{
	elfatFileData &m = fileTable.get_elFile(hash);
	
	if (m.filenameHash != hash)
		return false;
	string filename = "." + hash;
	filename= localFile+filename;
	char *deletestring = new char[ filename.length() + 30 ];
	sprintf(deletestring,"unlink %s",filename.c_str());
	system( deletestring );
	delete [] deletestring;
	vector< string > *vec= NULL;
	if (m.type == ELDIRECTORY)
		vec = &fileTable.get_directoryVector();
	else
		vec = &fileTable.get_filesVector();
	vector<string>::iterator startIterator = vec->begin();
	for (int i=0; i < vec->size(); i++,startIterator++)
	{
		if ( ( (string)vec->at(i) ) == hash)
		{
			vec->erase(startIterator);
			break;
				
		}
	}
	writeFAT();
}

string
EncryptedLFAT::createDirectory(string filename)
{
	string fatFileName="";
	char *data="blah";
	//cout << "Content-type: text/html\n\n";
//	cout << "directory name is " << filename;
	if ((fatFileName = offerFAT(filename,"directory",ELDIRECTORY,data,0) ) != "" )
	{
			
		
			writeFAT();
		
			return fatFileName;
		}
		else
			return fatFileName;
		
	
	
	
}


bool
EncryptedLFAT::createFile(string filename,string contenttype, char *data, int size)
{
	string fatFileName="";
	
	if ((fatFileName = offerFAT(filename,contenttype,ELFILE,data,size) ) != "" )
	{
		elfatFileData &m = fileTable.get_elFile(fatFileName);
		Botan::byte localKey[KEYL];
		for (int i=0; i < KEYL; i++)
		{
				localKey[i] = m.key[i];	
		}
		fatFileName="."+fatFileName;
		fatFileName=localFile+fatFileName;
		
		ofstream file(fatFileName.c_str(), ios::binary);
		if (file)
		{
			try
			{
				Botan::LibraryInitializer init("thread_safe use_engines");
				Botan::SymmetricKey key(localKey,KEYL);   //instantiate key
			
				Botan::Pipe pipe(Botan::get_cipher("AES/ECB", key, 
				Botan::ENCRYPTION)); //create pipe !exeption here!
				int multiplier = size / 16;
				int nextLowest = (multiplier*16)+16;
				int needToAdd = nextLowest- size;
				string addString="";
				for (int i = 0; i < needToAdd; i++)
					addString+="0";
				pipe.start_msg();
				pipe.write((const Botan::byte*) data,size);	
				pipe.write(addString);
				pipe.end_msg();
				file << pipe;
				file.close();
				/*
				
				
				*/

			
			} catch (std::exception &e) 
   			{
       			std::cerr << "exc caught: " << e.what() << std::endl;
				return false;
   			}
		
			writeFAT();
			finalized=true;	
			return true;
		}
		else
			return false;
		
	}
	
	return false;
}

void
EncryptedLFAT::writeFAT()
{
	try
	{
		
		
		Botan::LibraryInitializer init("thread_safe use_engines");
		Botan::SymmetricKey key(PIN,KEYL);   //instantiate key
		
		Botan::Pipe pipe(Botan::get_cipher("AES/ECB", key, 
		Botan::ENCRYPTION)); //create pipe !exeption here!
		string fl = localFile;
		fl +=".elfat";
		ofstream createELFat(fl.c_str());
		
		//localList = &fileTable.get_elfatList();
		
		localList = &fileTable.get_elfatList();
		
		listiterator = localList->begin();
		
		//list<string>::iterator iterator = localList->begin();
		//(listiterator==localList->end()) return false;
		
		string addString = ELFATMARKER;
		
		addString +=",";
		char t[2];
		while(listiterator!= localList->end())
		{
			
			elfatFileData &m = fileTable.get_elFile( ( (string)*listiterator) );
			addString += m.filename;
			addString += "|";
			addString += m.filenameHash;
			addString += "|";
			addString += m.fileSize;
			addString += "|";
			addString += m.key;
			addString += "|";
			sprintf(t,"%i",m.type);
			addString += t;
			addString += "|";
			addString += m.contentType;
			addString += ",";
			
			listiterator++;
		}
		//cout << addString << " ";
		listiterator = localList->begin();
		int multiplier = addString.length() / 16;
		int nextLowest = (multiplier*16)+16;
		int needToAdd = nextLowest- addString.length();
		for (int i = 0; i < needToAdd; i++)
			addString+="0";
		
		/*
		char *bs = new char[48];
		
		sprintf(bs,"%s,name|hash|filesize|key|extra",ELFATMARKER);
		for (int i=strlen(bs); i < 48; i++)
			bs[i]=0;
		*/
		pipe.start_msg();
		pipe.write(addString);
		
		pipe.end_msg();
		
		//cout << "aboot to pipe out";
		createELFat << pipe;
		createELFat.close();
		localList = &fileTable.get_elfatList();
		
		listiterator = localList->begin();

		finalized = true;
	} catch (std::exception &e) 
   {
	   //cout << "or " << e.what();
       std::cerr << "exc caught: " << e.what() << std::endl;
   }
  	
}

string 
EncryptedLFAT::offerFAT(string filename,string contenttype,unsigned int tpe, char *data, int size)
{
	
	string hash = MD5String(  ( (char*) filename.c_str() ) );
	elfatFileData lEL;
	
	lEL.filename = filename;
	lEL.filenameHash = hash;
	//char *sz = itoa ( size , int radi
	char sz[33];
	sprintf(sz,"%i",size);
	lEL.fileSize = sz;
	lEL.fileSizeAsInt = size;
	lEL.contentType=contenttype;
	string_functions keyGenerator;
	string key = keyGenerator.random_string(32);
	
	for (int i=0; i < 32, i < key.length(); i++)
		lEL.key[i]=key.at(i);
	lEL.type = tpe;
	fileTable.put_elFile( lEL );
	
	finalized = false;
	return hash;
}



elfatFileData&
EncryptedLFAT::getCurrentFATData()
{
	return fileTable.get_elFile( ( (string)*listiterator) );
}

bool
EncryptedLFAT::getNext()
{
	
	if (localList == NULL)
	{
		
		// grab the list
		localList = &fileTable.get_elfatList();
		listiterator = localList->begin();
		
		if (localList->size() ==0) return false;
		//if (listiterator==localList->end()) return false;
			
		return true;
	}		
	
	if (++listiterator == localList->end()) return false;
		
	return true;
	
}

bool 
EncryptedLFAT::accessFAT()
{
	if (correctPIN == false) return false;
	return true;
}


void
EncryptedLFAT::beginListFS()
{
	localList = NULL;
}

void
EncryptedLFAT::ReadFAT()
{
	
	try 
	{
	
		Botan::LibraryInitializer init("thread_safe use_engines");
		string fl = localFile;
		fl +=".elfat";
		ifstream encryptedFile(fl.c_str());
		if (encryptedFile)
		{
			Botan::SymmetricKey key(PIN,KEYL);   //instantiate key
       		Botan::Pipe pipe(Botan::get_cipher("AES/ECB", key, 
			Botan::DECRYPTION)); //create pipe !exeption here!
		
			pipe.start_msg();
       		encryptedFile >> pipe;
       		pipe.end_msg();
			encryptedFile.close();
			String unEncryptedFAT = pipe.read_all_as_string(0);
			string str = ELFATMARKER;
			string::size_type pos = unEncryptedFAT.toCPPString().find (str,0);
			
			if (pos != 0) throw exception();
			ParseFAT( unEncryptedFAT );
	   		
//       		outfile << pipe;
			correctPIN = true;
		}
		else
		{

			string fl = localFile;
			fl +=".elfat";
			ofstream createELFat(fl.c_str());
			Botan::SymmetricKey key(PIN,KEYL);   //instantiate key
			Botan::Pipe pipe(Botan::get_cipher("AES/ECB", key, 
			Botan::ENCRYPTION)); //create pipe !exeption here!
			/*
			char *bs = new char[48];
	   		
			sprintf(bs,"%s,name|hash|filesize|key|extra",ELFATMARKER);
	   		for (int i=strlen(bs); i < 48; i++)
		   		bs[i]=0;
			
			pipe.start_msg();
			pipe.write((const Botan::byte*) bs,48);
			*/
			
			char *bs = new char[16];
	   		
			sprintf(bs,"%s,",ELFATMARKER);
	   		for (int i=strlen(bs); i < 16; i++)
		   		bs[i]=0;
			
			pipe.start_msg();
			pipe.write((const Botan::byte*) bs,16);
			
	   		delete [] bs;
		    pipe.end_msg();
			
			createELFat << pipe;
			createELFat.close();
			ReadFAT();

		}
		
   } catch (std::exception &e) 
   {
       //std::cerr << "exc caught: " << e.what() << std::endl;
   }
  
}

void
EncryptedLFAT::ParseFAT( String &ucFAT)
{
	vector< String > parts = ucFAT.split( "," ); // split by commas
	for (int i=0; i < parts.size(); i++)
	{
		String localPart = parts.at(i);
		vector< String > localSplitPart = localPart.split("|");
		if ( localSplitPart.size() != 6)
			continue; // skip this, as it could be corrupted or end
		elfatFileData lEL;
		
		lEL.filename = localSplitPart.at(0).toCPPString();
		lEL.filenameHash = localSplitPart.at(1).toCPPString();
		lEL.fileSize = localSplitPart.at(2).toCPPString();
		lEL.fileSizeAsInt = atoi( lEL.fileSize.c_str() );
		string key = localSplitPart.at(3).toCPPString();
		for (int i=0; i < 32, i < key.length(); i++)
			lEL.key[i]=key.at(i);
		lEL.type = atoi( localSplitPart.at(4).toCPPString().c_str() );
		lEL.contentType = localSplitPart.at(5).toCPPString().c_str();
		
		fileTable.put_elFile( lEL );
		
		
	}
	
}


#endif
