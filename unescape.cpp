#include <iostream>
#include <string>

using namespace std;

char 
convertHexToChar( const char* inString ) 
{ 
    //0x4f == 01001111 mask 
    register char digit; 
    digit = (inString[0] >= 'A' ? ((inString[0] & 0x4f) - 'A')+10 : (inString[0] - '0')); 
    digit *= 16; 
    digit += (inString[1] >='A' ? ((inString[1] & 0x4f) - 'A')+10 : (inString[1] - '0')); 
    return digit; 
} 

/* 
iE = inEnvironment 
*/ 
char 
*getEnv(const char *iE) 
{ 
//    cout << "printing environment " << iE << " " << getenv(iE) << endl;
    return (char*)(getenv(iE)!=NULL ? getenv(iE) : ""); 
} 
string 
unescapeString(char *hexstr ) 
{ 
    char catchString[2]; 
    string returnValue=""; 
    int check; 
    if ( hexstr == NULL || strlen(hexstr)== 0 ) 
        return ""; 
    register unsigned int x; 
    for ( x = 0; hexstr[x]; x++ ) 
    { 
        switch(hexstr[x]) 
        { 
            case 43 : returnValue+= ' '; 
                break; 
            case 37 : 
	    {
	    	int location=0;
		string fart="";
                while (location<3)
		{
			cout << strlen(hexstr) << " " << x+(location+1) << endl;
			if (strlen(hexstr) >=x+(location+1))
			{
					if ((hexstr[x+(location+1)] >=48 && hexstr[x+(location+1)] <=57) || (hexstr[x+(location+1)] >=97 && hexstr[x+(location+1)]<=102) )
						fart+=hexstr[x+(location+1)];
				//		catchString[location]=hexstr[x+(location+1)];
					else
					{
						cout << hexstr[x+location+1] << endl;
						break;
						//location--;
//						continue;
					}
			//strncpy(catchString,&hexstr[x+1],2); 
			}
			++location;
		}
		if (fart.length()<2)
		{
			fart+='f';
//			fart+='0';
//			fart[1]=fart[0];
//			fart[0]='0';
		}

		
		cout << fart << " " << location << endl;
                returnValue+= convertHexToChar(fart.c_str()); 
                x+=location; 
                break; 
                } 
        default : returnValue+= hexstr[x]; continue; 
                 
        } 
         
    } 
    return returnValue; 
} 

int main()
{
	//char *test="http%3a%2f%2fw3s.com%2wml";
//http%3a%2f%2fw3s.com%2wml
	char *test="12345";
	cout << unescapeString(test) << endl;
	return 0;
}
