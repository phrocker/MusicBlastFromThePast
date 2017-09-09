#ifndef CGIPARSER_HPP 
#define CGIPARSER_HPP
#include <string> 
#include <cstdio>
#include "HashTable.h"
#include "hash.h"
//#include "cgiparser.h"
#include "env_functions.h"
#include "string_functions.h"
using namespace std;

typedef unsigned int UINT; 

enum DATA_TYPES{ ENV=0, FORM};
enum METHOD_TYPE{ GET=0,POST};
enum ENCODE_TYPE{ GETV=0,GIVE};
#define MAXPART 10741824
class cgiparser 
{ 
public: 
    cgiparser();
    ~cgiparser()
	{
		if (filePointer != NULL)
			delete [] filePointer;
		}
	bool haveFile(){ return isFile; }
	string getFileName(){ return filename; }
	char *getFile(){ return filePointer; }
	unsigned int getFileSize(){ return fileSize; }
	string getFileType(){ return file_contenttype; }
    string cgivalue( string key ){ return env_data[key].GetData(); } 
    string encoded_cgivalue( string key,char type=GIVE )
    {
        string g = (GETV ? cgivalue(key) : key);
        char *d = new char[ g.length()+1 ];
        strcpy(d,g.c_str());
        char *e = new char[strlen(d)*2];
        url_encode(d,e,strlen(d));
        string ret = e;
        delete [] e;
        return ret;
    }    
	
	void getPost();
	
    string form_data( string key){ 
        // cache it, in case it's used again
        string s=form_dat[key].GetData();
        char *d= new char[ s.length()+1 ];
        strcpy(d,s.c_str());
        return unescapeString(d);

        } 
    string encoded_form_data( string key )
    {
        string g = form_data(key);
        char *d = new char[ g.length() ];
        strcpy(d,g.c_str());
        char *e = new char[strlen(d)*2];
        url_encode(d,e,strlen(d));
        string ret = e;
        delete [] e;
        return ret;
    }        
    int METHOD(){ 
	    	if(strcmp(getEnv("REQUEST_METHOD"), "POST") == 0) 
		return POST;
		else 
		return GET; 
	}
    void cgiremoveKey(string);
    void formremoveKey(string);

private: 
    void cgiaddKey( string aKey, string aValue, int type=0); 
    HashTable<string,string> form_dat;
	char *filePointer;
	string filename;
	bool isFile;
	unsigned int fileSize;
	string file_contenttype;
    HashTable<string,string> env_data;  
    // we really shouldn't have too many files 
/*
    struct file_data 
    { 
        int file_number; 
        string file_name; 
        string temp_file_name; 
        string mime_type; 
        file_data *next_file; 
    }; 
*/
}; 


cgiparser::cgiparser() 
{ 
	filePointer = NULL;
	isFile=false;
	filename="";
	//cout << "we try as we might" << endl;
    env_data.Set(&stoi,10,"");
    form_dat.Set(&stoi,10,"");
	
	
	
    string contentType = getEnv("CONTENT_TYPE"); 
	
    //cout << "Content-type: text/html\n\n";
  //       cout << getEnv("QUERY_STRING") << endl;
//        cout << getEnv("REQUEST_METHOD") << endl;
    //char *contenttype=getEnv("CONTENT_TYPE"); 
    //cout << "but did we try hard enough?" << endl;
	
    if (strlen(getEnv("QUERY_STRING")) > 0) 
    { 
        char* temp = NULL; 
        temp = new char[ strlen(getEnv("QUERY_STRING"))  + 1 ]; 
        strcpy( temp, getEnv("QUERY_STRING") ); 
        char* key = temp, * value; 
        while( key != NULL ) 
        { 
            if ( (temp = strchr(temp, '=')) != NULL ) 
                *(temp++) = '\0'; 
            else 
                break; 

            value = temp; 
            if ( (temp = strchr( temp, '&' )) != NULL ) 
                *(temp++) = '\0'; 
            cgiaddKey( unescapeString( key ), unescapeString( value ),ENV ); 
            key = temp; 
        } 
        delete [] temp; 
    } 

	
    if ( strcmp(getEnv("REQUEST_METHOD"), "POST") == 0 && contentType.length() > 0 ) 
    { 
		
    /*
        if (contentType == "multipart/form-data") 
             
        if (contentType == "application/x-www-form-urlencoded") 
        } else if (contenttype && !strncmp(contenttype, 
                ,19)) { 
            parseMultipart(); 
        } 
*/
		//cout << "Content-type: text/html\n\n";
        char *temp2; 
		//string contentType = getenv("CONTENT_TYPE");
		
        UINT size = atoi( getenv("CONTENT_LENGTH") ); 
		
		
		//exit(1);
		if (size > MAXPART)
		{
			cout << "Content-type: text/html\n\n";
			cout << "<html><head><title>Error 413, Document too long</title></head><body>Document too long FOOL</body></html>";
			exit(1);
		}
        char *copycat; 
	   
		string multi = "multipart/form-data";
        if ( size > 0 ) 

        { 
            // get the post data from standard input 
            temp2 = new char[ size + 1 ]; 
			if (contentType.find(multi,0) != string::npos)
			{
		//		cin.sync_with_stdio(true);			  
	//			cin.unsetf(ios::skipws) ;
				
            	cin.read( temp2, size + 1 ); 
//				cout << "Content-type: text/html\n\n";
	//			cout.write(temp2,size);
				
		//		cout << " ";
	//					for (int i=0; i < size; i++)
//										cout << temp2[i];
								if ( cin.rdstate() | (ifstream::failbit|ifstream::eofbit|ifstream::badbit))
							
				{
//					return;
				}

				/*
				cin.getline( temp2, size + 1 ); 
				cin.getline( temp2, size + 1 ); 
				cin.getline( temp2, size + 1 ); 
				cin.getline( temp2, size + 1 ); 
				*/
//				cin.sync();
			}
			else
				cin.getline( temp2, size + 1 ); 

            copycat = temp2; 
		
        } 
		
		if (cin.gcount() < size )
		{
			cout << "Content-type: text/html\n\n";
			cout << "<html><head><title>Error 413, Document too long</title></head><body>Document too long FOOL</body></html>";
			exit(1);
		}
		if (contentType.find(multi,0) != string::npos)
		{
		//	cout << "Content-type: text/html\n\n";

			//cout << contentType << "boundary should be: ";
			int bound=0;
			string bndry="boundary=";
			string boundary="";
			if ( (bound=contentType.find(bndry,0)) != string::npos)
			{
				bound+=bndry.length();
				boundary=contentType.substr(bound,contentType.length()-bound);
				
			}
			int cnt=0;
			
			boundary= "--" + boundary;
			int beginning_boundary=0,ending_boundary=0;
			char *pointer = copycat;
			//cout << boundary;
			char *pn=copycat;

//			cout << "                                 ";
//			cout << "actual size is " << cnt;
			string_functions sf;
			beginning_boundary = sf.find(pointer,(char*)boundary.c_str());
			beginning_boundary=0;
			/*
			{
				if (!strncmp(pointer,boundary.c_str(),boundary.length()))
				{
					beginning_boundary=0;
					break;
				}
			}
			*/
			
			//ending_boundary = sf.find(pointer,(char*)boundary.c_str());
			
			//cout << "ending boiundary " << ending_boundary;
			//cout << copycat << " size: " << size;
			if (ending_boundary > 0)
				pointer[ending_boundary-2] = '\0';
			int filename_boundary = sf.find(pointer,"filename=\"");
			for (int i=0; i < filename_boundary+strlen("filename=\""); i++,pointer++, beginning_boundary++);
				
			//size-= (filename_boundary+strlen("filename=\""));
			//string myfilename="";
			while(*pointer)
			{
				if (!strncmp(pointer,"\"",strlen("\"")))
				{
					pointer+=strlen("\"");
				//	size-=strlen("\"");
					beginning_boundary+=strlen("\"");
					
					break;
				}
				filename+=*pointer;
				beginning_boundary++;
				pointer++;
			//	size--;
			}
			if (filename=="")
			{
				delete [] temp2;
				return;
			}
			int content_type_boundary = sf.find(pointer,"Content-Type: ");
			for (int i=0; i < content_type_boundary+strlen("Content-Type: "); i++,pointer++,beginning_boundary++);
			//	size-=content_type_boundary+strlen("Content-Type: ");
				//cout << "at this point:" << pointer;
			string file_type="";
			while(*pointer)
			{
				if (*pointer == 0xd && *(pointer+1) == 0xa)
				{
					
					while(*pointer == 0xd && *(pointer+1) == 0xa)
					{
						pointer+=2;
						beginning_boundary+=2;
						//size--;
					}
					
				//	pointer+=3;
					break;
				}
				file_type+=*pointer;
				
				pointer++;
				beginning_boundary++;
			//	size--;
			}
			
			
		//	beginning_boundary+=file_type.length();
		//	cout << pointer << " " << size << " " << beginning_boundary;
			/*
			char *ptr = pointer;
			int end=0;
			while(*ptr)
			{
				
				if (*ptr=='\0') break;
				end++;
				ptr++;
			}
			*/
			
			//memcpy(filePointer,pointer,);

			for (int i =size-1; i >=0 ; i--)	
			{
				bool isfound=true;
				int j=i;
			//	cout << "                           ";
				int d=0;
				for (; j < size,d < boundary.length(); j++,d++)
				{
					//cout << copycat[j];
					if (copycat[j] != boundary.at(d))
					{
						isfound == false;
						break;
					}
				}
			//	cout << "                           ";
				if (isfound == true && d == boundary.length())
				{
					ending_boundary=i-3;
					break;
				}
			}
		//	cout << size << " " << " " << ending_boundary << " " << beginning_boundary;
			
			filePointer = new char[ ending_boundary-beginning_boundary ];
			int j=0;
			for (int i = beginning_boundary,j=0; i < size, j < ending_boundary-beginning_boundary; i++,j++)
			{
				//cout << copycat[i];
				filePointer[j] = copycat[i];
			}
					//	cout << "yo " << size;
			//memcpy(filePointer,pointer,size);
			fileSize=ending_boundary-beginning_boundary;
			fileSize--;
			//filename=myfilename;
			file_contenttype = file_type;
	//		cout << "Content-type: text/html\n\n";
//				cout.write(filePointer,fileSize);
			//cout << filename << " " << file_type << " ";
		//	for (int i=0; i < fileSize; i++)
	//			cout << filePointer[i];
			isFile = true;
//			delete [] filePointer;
			delete [] temp2;
			//exit(0);
		}
		else
		{
			
			char* key = copycat, * value; 
			while( key != NULL ) 
			{ 
				if ( (copycat = strchr(copycat, '=')) != NULL ) 
					*(copycat++) = '\0'; 
				else 
					break; 
				value = copycat; 
				if ( (copycat = strchr( copycat, '&' )) != NULL ) 
					*(copycat++) = '\0'; 
				// cout << key << " " << value << "  ";
				
				cgiaddKey( unescapeString( key ),  value ,FORM ); 
				// changed. both had unescapeString before
	//            cgiaddKey(key,value,FORM ); 
				key = copycat; 
			
		   }
		   delete [] temp2;
	   }
	   //exit(1); 
    }
    //cout << "did we possibly get here?" << endl;
} 
void
cgiparser::getPost()
{
	}
void 
cgiparser::cgiaddKey( string aKey, string aValue, int type) 
{ 
//    cout << "Content-type: text/html\n\n";
//cout << aValue;
    switch(type) 
    { 
        case ENV:  env_data[aKey]=aValue;
            break; 
        case FORM: form_dat[aKey]=aValue;
            break; 
        default: 
                env_data[aKey]=aValue;
    } 
} 

void 
cgiparser::cgiremoveKey( string aKey )
{ 
    env_data[aKey].SetStatus(EMPTY);
} 

void 
cgiparser::formremoveKey( string aKey)
{
   form_dat[aKey].SetStatus(EMPTY);
}

#endif
