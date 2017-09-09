#ifndef COOKIES_HPP
#define COOKIES_HPP
#include <string>
#include <vector>
#include "html.h"
#include "HashTable.h"
#include "time_functions.h"
#include "env_functions.h"
using namespace std;
/**
 * Cookies handles cookie operations. Responsible for creating and reading cookies.
 */
class cookies
{
	private:
		vector< string > logoutVector;
	public:
		cookies(bool type=true,string pfx="") // constructor
        {
            cando=type;
            prefix=pfx;	mycookies.Set(&stoi,10,""); handleCookies();
        }
		~cookies(){}
			
		// function to create cookies
        void create_cookie(string name, string value,string expiration="",string path="",string domain="");

		// returns the value of the cookie name, which is located within
		// the hashmap mycookies
		string cookieEntry(string name)
        {   
            return mycookies[ clean_cookie(name) ].GetData(); 
        }
		
		void clear_cookies();

        string clean_cookie(string);
		
    friend class html;
	private:
        bool cando;
        time_functions mytime;
		HashTable<string,string> mycookies;
		
        string prefix;
		void handleCookies();
};

void
cookies::create_cookie(string inname, string invalue, string expiration, string path,string domain)	
// type 1 allows the second field to be a character
{
	// need to get rid of the following from the invalue
	//  	cout << "Content-type: text/html\n\n";
	//semi-colon, comma and white space
	string myvalue ="";
	
	
	char *p = new char[invalue.length() ];
	
	const char hexdigits[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};        /* lookup table for output hexadecimal text */
	for (int i=0; i < invalue.length(); i++)
	{
		char pointer = invalue.at(i);
		if((isalnum(pointer))||(pointer=='.')||(pointer=='-')||(pointer=='_')||(pointer<0)) 
			
			{ 
				
				myvalue+=pointer;
				} /* if character is A-Z, a-z, 0-9, ., -, or _, then copy it to the output string unchanged */
		  else if(pointer==' ')				/* if is a space, compact any consecutive spaces and output them collectively as a single + character */
		{
			myvalue+="%20";
		}
		
		else
///		  else if(length+3<n)				/* otherwise, we need to url encode the character (if we have enough room remaining in the output string to) */
		{
		  myvalue+="%";
		  myvalue+=hexdigits[pointer>>4]; 
		  myvalue+=hexdigits[pointer&0x0F]; 
		}
		//p[i]=invalue.at(i);
	}
	
  if (cando==false) return;
	string Cookie = "Set-Cookie: " + clean_cookie(inname) + "=" + myvalue + "; ";
	if (expiration.length() > 0)
	{
		string expires= mytime.gm_time(expiration);
	//	Cookie+="expires=" + expires + " GMT; ";
	}
	if (path.length() > 0)
		Cookie+="path=" + path + "; ";
	else
		Cookie+="path=/; ";
	if (domain.length() > 0)
		Cookie+="domain=" + domain + "; ";
	Cookie+="\r\n";
	cout << Cookie;
}


void
cookies::clear_cookies()
{
	for (int i=0; i < logoutVector.size(); i++)
	{
		create_cookie(logoutVector.at(i),"");
	}
}



//convoluted code
void
cookies::handleCookies() {

	// set up some buffers
    //cout << "is the problem here?" << endl;
    //cout << getEnv("HTTP_COOKIE");
	char		sixteens;
	char		ones;

	logoutVector.resize( 0 );
    string httpcookie=getEnv("HTTP_COOKIE");
  //cout << "Content-type: text/html\n\nd";
    if (httpcookie.length()==0) return;
  //cout << httpcookie << endl;
    string traverse,name,value;
    traverse=name=value="";
    int ij=0,i,j;
    //cout << httpcookie.length() << " ";
    for (i=0; i< httpcookie.length(); i++)
    {
        ij=i;
//    cout << i << " ";        
        for (j=i; j < httpcookie.length(); j++)
        {
 //       cout << j << " ";        
            if (httpcookie[j]==61)
            {
                if (httpcookie[ij]==32)
                ij++;
                name=httpcookie.substr(ij,(j)-ij);
                ij=++j;
            }
            if (httpcookie[j]==59 || j+1==httpcookie.length())
            {
            if(j+1==httpcookie.length())
              value = httpcookie.substr(ij,(j)-(ij-1));
            else
              value = httpcookie.substr(ij,(j)-ij);
			
			// at this point we should extract the %hex values
			/*
			string space="%20";
			string semi="%3B";
			string comma="%2C";
			int found=0;
			string newspace=" ";
			string newsemi=";";
			string newcomma=",";
			while(( found = value.find(space,0))!=string::npos)
			{
				value.replace(found,3,newspace);
			}
			
			while(( found = value.find(semi,0))!=string::npos)
			{
				value.replace(found,3,newsemi);
			}
			
			while(( found = value.find(comma,0))!=string::npos)
			{
				value.replace(found,3,newcomma);
			}
			*/
			string myvalue = "";
			char *pointer = (char*)value.c_str();
			static char build;
//			cout << "Content-type: text/html\n\n";
	//		cout << value << " -- ";
			while(*pointer)
			{
				if(*pointer=='%')				/* decode escape sequences... */
				{
				pointer++;					/* look at the first character of the escape sequence... */
				if(isdigit(*pointer)) build=*pointer-'0';		/* if the first digit is a decimal number, then set build to the value 0-9 appropriately */
				else if((*pointer>='A')&&(*pointer<='F')) build=*pointer-'A'+10; /* if the first digit is a letter A-F, then set build to the value 10-15 appropriately */
				else if((*pointer>='a')&&(*pointer<='f')) build=*pointer-'a'+10; /* same for lowercase a-f */
				else continue;				/* if not a valid hex digit, then skip this character */
				pointer++; build<<=4;				/* look at the second character of the escape sequence (and update build to indicate that the first digit was in the 16's place)... */
				if(isdigit(*pointer)) build+=*pointer-'0';		/* if the first digit is a decimal number, then set build to the value 0-9 appropriately */
				else if((*pointer>='A')&&(*pointer<='F')) build+=*pointer-'A'+10; /* if the first digit is a letter A-F, then set build to the value 10-15 appropriately */
				else if((*pointer>='a')&&(*pointer<='f')) build+=*pointer-'a'+10; /* same for lowercase a-f */
				else { build>>=4; pointer--; }			/* if not a valid hex digit, then not to spec, but we will just use the one hex digit as the value */
					myvalue+=build;
				//*outpos=build; outpos++;			/* update the output stream to hold the unescaped character */
				}
				else
				{
					myvalue+=*pointer;
					
				}
				pointer++;
			}
//			cout << "Content-type: text/html\n\n";
	//		cout << myvalue;
		//	char *d = new char[value.length()];
	//		url_decode((char*)value.c_str(),d,value.length());
			//strncpy(d,d,strlen(d)-1);
			//value=d;
//			delete [] d;
			//value = value.substr(0,value.length()-1);
			
                mycookies[name]=myvalue;
    			logoutVector.push_back( name );
             //  cout << "|" << name << "---" << mycookies[name].GetData();
                break;
            }
            if (httpcookie[j]==43)
                httpcookie[j]==32;
        }
        i=j;
 


	

}

string
cookies::clean_cookie(string incookie)
{

	string cleanCookie=incookie;

    if (prefix.length() > 0)
    {
    	cleanCookie = prefix;
	   cleanCookie += "_";
    	cleanCookie += incookie;   
    }
    return cleanCookie;

}



#endif
