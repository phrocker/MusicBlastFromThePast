#include "cgiparser.h"
#include "env_functions.h"
cgiparser::cgiparser() 
{ 
cout << "yoyo" << endl;
    env_data.Set(&stoi,10,0);
    form_dat.Set(&stoi,10,0);
    string contentType = getEnv("CONTENT_TYPE"); 
cout << "yoyo" << endl;     
    //char *contenttype=getEnv("CONTENT_TYPE"); 
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
            cgiaddKey( unescapeString( key ), unescapeString( value ),0 ); 
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
        char *temp2; 

        UINT size = atoi( getenv("CONTENT_LENGTH") ); 
        char *copycat; 
        if ( size > 0 ) 

        { 
            // get the post data from standard input 
            temp2 = new char[ size + 1 ]; 

            cin.getline( temp2, size + 1 ); 

            copycat = temp2; 

        } 
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
            cgiaddKey( unescapeString( key ), unescapeString(  value ),1 ); 
            key = copycat; 
        } 
    } 
cout << "yoyo" << endl;
} 

void 
cgiparser::cgiaddKey( string aKey, string aValue, int type) 
{ 
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
